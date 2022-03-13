#include "shader.h"

#include <stdlib.h>
#include <stdio.h>

uint32_t checkCompile(uint32_t shader);
uint32_t checkLink(uint32_t program);

uint32_t gls_initShader(shader_t* shader)
{
	shader->program = glCreateProgram();
	if (shader->program == 0) {
		return 1;
	}
	shader->ptr = malloc(sizeof(uint32_t) * 8);
	shader->ptr[0] = 0;
	return 0;
}

uint32_t gls_attachStage(shader_t* shader, const char* code, GLenum type)
{
	uint32_t stage = glCreateShader(type);
	if (stage == 0) {
		return 1;
	}
	glShaderSource(stage, 1, &code, NULL);
	glCompileShader(stage);
	if (checkCompile(stage)) {
		glDeleteShader(stage);
		return 1;
	}
	shader->ptr[0]++;
	shader->ptr[shader->ptr[0]] = stage;
	return 0;

}

uint32_t gls_linkStages(shader_t* shader)
{
	for (uint32_t i = 0; i < shader->ptr[0]; i++) {
		glAttachShader(shader->program, shader->ptr[i + 1]);
	}

	glLinkProgram(shader->program);
	if (checkLink(shader->program)) {
		return 1;
	}

	for (uint32_t i = 0; i < shader->ptr[0]; i++) {
		glDeleteShader(shader->ptr[i + 1]);
	}

	free(shader->ptr);
	shader->ptr = NULL;

	return 0;
}

void gls_destroyShader(shader_t* shader)
{
	if (shader->ptr) {
		for (uint32_t i = 0; i < shader->ptr[0]; i++) {
			glDeleteShader(shader->ptr[i + 1]);
		}
		free(shader->ptr);
	}
	if (shader->program) {
		glDeleteProgram(shader->program);
	}
}

uint32_t checkCompile(uint32_t shader)
{
	int32_t status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status) {
		GLint log_size;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
		char* log = (char*)malloc(log_size);
		glGetShaderInfoLog(shader, log_size, NULL, log);
		printf("%s\n", log);
		free(log);
		return 1;
	}
	return 0;
}

uint32_t checkLink(uint32_t program)
{
	int32_t status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status) {
		GLint log_size;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
		char* log = (char*)malloc(log_size);
		glGetProgramInfoLog(program, log_size, NULL, log);
		printf("%s\n", log);
		free(log);
		return 1;
	}
	return 0;
}