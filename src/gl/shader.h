#pragma once

#include <stdint.h>

#include <GL/gl3w.h>

struct Shader
{
	uint32_t program;
	uint32_t* ptr;
};

typedef struct Shader shader_t;

uint32_t gls_initShader(shader_t* shader);
uint32_t gls_attachStage(shader_t* shader, const char* code, GLenum type);
uint32_t gls_linkStages(shader_t* shader);
void gls_destroyShader(shader_t* shader);

inline void gls_use(shader_t* shader)
{
	glUseProgram(shader->program);
}

inline int32_t gls_getUniformLoc(shader_t* shader, const char* uniform)
{
	return glGetUniformLocation(shader->program, uniform);
}