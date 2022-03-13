#include <stdint.h>

// __attribute__((aligned (32))) const int32_t mat4f_trans_idx[8] = {0, 4, 2, 6, 1, 5, 3, 7};

// __attribute__((aligned (32))) const float mat4f_inv_alt[4] = {1, -1, 1, -1};
// __attribute__((aligned (32))) const float mat4f_inv_negative1 = -1.0f;

__attribute__((aligned (32))) const float mat4f_identity[16] = {1,0,0,0,
										  					  0,1,0,0,
										  					  0,0,1,0,
										  					  0,0,0,1};

__attribute__((aligned (32))) const float mat3f_identity[9] = {1,0,0,
										 					 0,1,0,
										 					 0,0,1};