#pragma once

/*
	COMPILE OPTIONS FOR VECTOR FUNCTIONS
*/
// #define VECTOR_COMPILE
#define VECTOR_INLINE
// #define VECTOR_ALWAYS_INLINE

/*
	COMPILE OPTIONS FOR MATRIX FUNCTIONS
*/
// #define MATRIX_COMPILE
#define MATRIX_INLINE
// #define MATRIX_ALWAYS_INLINE

/*
	ALIGN MAT3 STRUCT TO NEAREST 16 BYTE BOUNDARY (and use aligned loads and stores)
*/
// #define MAT3_ALIGN


#define INLINE inline __attribute__((__always_inline__))

#ifdef MATRIX_COMPILE
	#ifdef MAT_FUNC
		#undef MAT_FUNC
	#endif
	#define MAT_FUNC
#endif
#ifdef MATRIX_INLINE
	#ifdef MAT_FUNC
		#undef MAT_FUNC
	#endif
	#define MAT_FUNC inline
#endif
#ifdef MATRIX_ALWAYS_INLINE
	#ifdef MAT_FUNC
		#undef MAT_FUNC
	#endif
	#define MAT_FUNC INLINE
#endif

#ifdef VECTOR_COMPILE
	#ifdef VEC_FUNC
		#undef VEC_FUNC
	#endif
	#define VEC_FUNC
#endif
#ifdef VECTOR_INLINE
	#ifdef VEC_FUNC
		#undef VEC_FUNC
	#endif
	#define VEC_FUNC inline
#endif
#ifdef VECTOR_ALWAYS_INLINE
	#ifdef VEC_FUNC
		#undef VEC_FUNC
	#endif
	#define VEC_FUNC INLINE
#endif

#undef INLINE