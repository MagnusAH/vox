#include <stdint.h>

__attribute__((aligned (16))) const float mat4f_identity[16] = {1,0,0,0,
										  					  0,1,0,0,
										  					  0,0,1,0,
										  					  0,0,0,1};

__attribute__((aligned (16))) const float mat3f_identity[9] = {1,0,0,
										 					 0,1,0,
										 					 0,0,1};