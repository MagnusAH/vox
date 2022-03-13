#pragma once

#include <stdint.h>

#include <core/simple_string.h>
#include <stdio.h>

#define FS_MODE_READ 0
#define FS_MODE_WRITE 1
#define FS_MODE_APPEND 2
#define FS_MODE_READ_UPDATE 3
#define FS_MODE_WRITE_UPDATE 4
#define FS_MODE_APPEND_UPDATE 5

#define FS_CRITICAL_ERROR 1
#define FS_SAFE_ERROR 2

uint64_t fs_readFile(char** buffer, uint64_t* length, string32_t* path);
uint64_t fs_writeFile(char* buffer, uint64_t length, string32_t* path);

uint64_t fs_readFile8(string8_t* buffer, string32_t* path);
uint64_t fs_writeFile8(string8_t* buffer, string32_t* path);

uint64_t fs_readFile16(string16_t* buffer, string32_t* path);
uint64_t fs_writeFile16(string16_t* buffer, string32_t* path);

uint64_t fs_readFile32(string32_t* buffer, string32_t* path);
uint64_t fs_writeFile32(string32_t* buffer, string32_t* path);

uint64_t fs_openFile(FILE** file, string32_t* path, const uint8_t mode);

uint64_t fs_getDirectories(string32_t* path, string32_t** directories, uint32_t* count);
uint64_t fs_getFiles(string32_t* path, string32_t** files, uint32_t* count);

uint64_t fs_getCWD(string32_t* cwd);
uint64_t fs_setCWD(string32_t* cwd);

uint64_t fs_createFile(string32_t* path);
uint64_t fs_deleteFile(string32_t* path);

uint64_t fs_createDirectory(string32_t* path);
uint64_t fs_deleteDirectory(string32_t* path);