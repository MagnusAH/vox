#pragma once

// #define VECTOR_COMPILE
#define VECTOR_INLINE
// #define VECTOR_ALWAYS_INLINE

// #define MATRIX_COMPILE
#define MATRIX_INLINE
// #define MATRIX_ALWAYS_INLINE

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