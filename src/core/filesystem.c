#include "filesystem.h"

#include <stdlib.h>

#ifdef _WIN32
	#include <Windows.h>
	#include <direct.h>
	const wchar_t* fs_modes16[] = {
		L"rb",
		L"wb",
		L"ab",
		L"rb+",
		L"wb+",
		L"ab+"
	};
#else
	#include <unistd.h>
#endif

const char* fs_modes8[] = {
	"rb",
	"wb",
	"ab",
	"rb+",
	"wb+",
	"ab+"
};

uint64_t fs_readFile(char** buffer, uint64_t* length, string32_t* path)
{
	FILE* f;
	if (fs_openFile(&f, path, FS_MODE_READ)) {
		return FS_CRITICAL_ERROR;
	}

	fseek(f, 0, SEEK_END);
	uint64_t len = ftell(f);
	fseek(f, 0, SEEK_SET);

	*buffer = malloc(len);
	uint64_t read = fread(*buffer, 1, len, f);
	if (length)
		*length = read;
	fclose(f);
	if (read != len) {
		return FS_SAFE_ERROR;
	}

	return 0;
}

uint64_t fs_writeFile(char* buffer, uint64_t length, string32_t* path)
{
	FILE* f;
	if (fs_openFile(&f, path, FS_MODE_WRITE)) {
		return FS_CRITICAL_ERROR;
	}

	uint64_t written = fwrite(buffer, 1, length, f);

	fclose(f);

	if (written != length) {
		return FS_SAFE_ERROR;
	}

	return 0;

}

uint64_t fs_readFile8(string8_t* buffer, string32_t* path)
{
	FILE* f;
	if (fs_openFile(&f, path, FS_MODE_READ)) {
		return FS_CRITICAL_ERROR;
	}

	fseek(f, 0, SEEK_END);
	uint64_t length = ftell(f);
	fseek(f, 0, SEEK_SET);

	ss_ensize8(buffer, length+1);
	buffer->length = fread(buffer->c_str, 1, length, f);
	buffer->c_str[buffer->length] = 0;

	fclose(f);

	if (buffer->length != length) {
		return FS_SAFE_ERROR;
	}

	return 0;
}

uint64_t fs_writeFile8(string8_t* buffer, string32_t* path)
{
	FILE* f;
	if (fs_openFile(&f, path, FS_MODE_WRITE)) {
		return FS_CRITICAL_ERROR;
	}

	uint64_t written = fwrite(buffer->c_str, 1, buffer->length, f);

	fclose(f);

	if (written != buffer->length) {
		return FS_SAFE_ERROR;
	}

	return 0;
}

uint64_t fs_openFile(FILE** file, string32_t* path, const uint8_t mode)
{
	FILE* f;
	#ifdef _WIN32
		string16_t npath;
		ss_init16_s32(&npath, path);
		if (_wfopen_s(&f, npath.c_str, fs_modes16[mode])) {
	#else
		string8_t npath;
		ss_init8_s32(&npath, path);
		f = fopen(npath.c_str, fs_modes8[mode]);
		if (f == NULL) {
	#endif
			ssg_destroy(&npath);
			return FS_CRITICAL_ERROR;
		}
		ssg_destroy(&npath);
		*file = f;
	return 0;
}

uint64_t fs_getDirectories(string32_t* path, string32_t** directories, uint32_t* count);
uint64_t fs_getFiles(string32_t* path, string32_t** files, uint32_t* count);

uint64_t fs_getCWD(string32_t* cwd)
{
	ss_clear32(cwd);
	#ifdef _WIN32
		string16_t ncwd;
		ss_init16_s(&ncwd, 4096);
		_wgetcwd(ncwd.c_str, 1024);
		ss_sync16(&ncwd);
		ss_append32_s16(cwd, &ncwd);
	#else
		string8_t ncwd;
		ss_init8_s(&ncwd, 4096);
		getcwd(ncwd.c_str, 4096);
		ss_sync8(&ncwd);
		ss_append32_s8(cwd, &ncwd);
	#endif
	ssg_destroy(&ncwd);
	return 0;
}

uint64_t fs_setCWD(string32_t* cwd)
{
	#ifdef _WIN32
		string16_t ncwd;
		ss_init16_s32(&ncwd, cwd);
		if (_wchdir(ncwd.c_str)) {
	#else
		string8_t ncwd;
		ss_init8_s32(&ncwd, cwd);
		if (chdir(ncwd.c_str)) {
	#endif
		ssg_destroy(&ncwd);
		return FS_CRITICAL_ERROR;
	}
	ssg_destroy(&ncwd);
	return 0;
}

uint64_t fs_createFile(string32_t* path)
{
	#ifdef _WIN32
		string16_t npath;
		ss_init16_s32(&npath, path);
		FILE* f;
		if (_wfopen_s(&f, npath.c_str, L"wb")) {
	#else
		string8_t npath;
		ss_init8_s32(&npath, path);
		FILE* f = fopen(npath.c_str, "wb");
		if (f == NULL) {
	#endif
		ssg_destroy(&npath);
		return FS_CRITICAL_ERROR;
	}
	ssg_destroy(&npath);
	if (fclose(f) == EOF) {
		return FS_CRITICAL_ERROR;
	}
	return 0;
}

uint64_t fs_deleteFile(string32_t* path)
{
	#ifdef _WIN32
		string16_t npath;
		ss_init16_s32(&npath, path);
		if (_wremove(npath.c_str)) {
	#else
		string8_t npath;
		ss_init8_s32(&npath, path);
		if (remove(npath.c_str)) {
	#endif
		ssg_destroy(&npath);
		return FS_CRITICAL_ERROR;
	}
	ssg_destroy(&npath);
	return 0;
}

uint64_t fs_createDirectory(string32_t* path)
{
	#ifdef _WIN32
		string16_t npath;
		ss_init16_s32(&npath, path);
		if(_wmkdir(npath.c_str)) {
	#else
		string8_t npath;
		ss_init8_s32(&npath, path);
		if(mkdir(npath.c_str, 0777)) {
	#endif
		ssg_destroy(&npath);
		return FS_CRITICAL_ERROR;
	}
	ssg_destroy(&npath);
	return 0;
}

uint64_t fs_deleteDirectory(string32_t* path)
{
	#ifdef _WIN32
		string16_t npath;
		ss_init16_s32(&npath, path);
		if(_wrmdir(npath.c_str)) {
	#else
		string8_t npath;
		ss_init8_s32(&npath, path);
		if(rmdir(path->c_str)) {
	#endif
		ssg_destroy(&npath);
		return FS_CRITICAL_ERROR;
	}
	ssg_destroy(&npath);
	return 0;
}