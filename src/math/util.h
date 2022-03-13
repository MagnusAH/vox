#pragma once

#include <math/vec3f.h>
#include <math/mat4f.h>

void mat4f_setProjection(mat4f_t* o, float aspectRatio, float fov, float near, float far);

void mat4f_setTransform3f(mat4f_t* o, float x, float y, float z);
void mat4f_setTransform3v(mat4f_t* o, vec3f_t* a);

void mat4f_setScalef(mat4f_t* o, float a);
void mat4f_setScale3f(mat4f_t* o, float x, float y, float z);
void mat4f_setScale3v(mat4f_t* o, vec3f_t* a);

#include <math/util.i>