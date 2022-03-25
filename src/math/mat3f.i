#include <immintrin.h>

extern const float mat3f_identity[9];

MAT_FUNC void mat3f_addf(mat3f_t* o, mat3f_t* a, float b)
{
	__m128
		broad = _mm_broadcast_ss(&b);
	#ifdef MAT3_ALIGN
		_mm_store_ps(&o->f[0], _mm_add_ps(_mm_load_ps(&a->f[0]), broad));
		_mm_store_ps(&o->f[4], _mm_add_ps(_mm_load_ps(&a->f[4]), broad));
	#else
		_mm_storeu_ps(&o->f[0], _mm_add_ps(_mm_loadu_ps(&a->f[0]), broad));
		_mm_storeu_ps(&o->f[4], _mm_add_ps(_mm_loadu_ps(&a->f[4]), broad));
	#endif
	o->f[8] = a->f[8] + b;
}

MAT_FUNC void mat3f_subf(mat3f_t* o, mat3f_t* a, float b)
{
	__m128
		broad = _mm_broadcast_ss(&b);
	#ifdef MAT3_ALIGN
		_mm_store_ps(&o->f[0], _mm_sub_ps(_mm_load_ps(&a->f[0]), broad));
		_mm_store_ps(&o->f[4], _mm_sub_ps(_mm_load_ps(&a->f[4]), broad));
	#else
		_mm_storeu_ps(&o->f[0], _mm_sub_ps(_mm_loadu_ps(&a->f[0]), broad));
		_mm_storeu_ps(&o->f[4], _mm_sub_ps(_mm_loadu_ps(&a->f[4]), broad));
	#endif
	o->f[8] = a->f[8] - b;
}

MAT_FUNC void mat3f_mulf(mat3f_t* o, mat3f_t* a, float b)
{
	__m128
		broad = _mm_broadcast_ss(&b);
	#ifdef MAT3_ALIGN
		_mm_store_ps(&o->f[0], _mm_mul_ps(_mm_load_ps(&a->f[0]), broad));
		_mm_store_ps(&o->f[4], _mm_mul_ps(_mm_load_ps(&a->f[4]), broad));
	#else
		_mm_storeu_ps(&o->f[0], _mm_mul_ps(_mm_loadu_ps(&a->f[0]), broad));
		_mm_storeu_ps(&o->f[4], _mm_mul_ps(_mm_loadu_ps(&a->f[4]), broad));
	#endif
	o->f[8] = a->f[8] * b;
}

MAT_FUNC void mat3f_divf(mat3f_t* o, mat3f_t* a, float b)
{
	__m128
		broad = _mm_broadcast_ss(&b);
	#ifdef MAT3_ALIGN
		_mm_store_ps(&o->f[0], _mm_div_ps(_mm_load_ps(&a->f[0]), broad));
		_mm_store_ps(&o->f[4], _mm_div_ps(_mm_load_ps(&a->f[4]), broad));
	#else
		_mm_storeu_ps(&o->f[0], _mm_div_ps(_mm_loadu_ps(&a->f[0]), broad));
		_mm_storeu_ps(&o->f[4], _mm_div_ps(_mm_loadu_ps(&a->f[4]), broad));
	#endif
	o->f[8] = a->f[8] / b;
}

MAT_FUNC void mat3f_iden(mat3f_t* o)
{
	#ifdef MAT3_ALIGN
		_mm_store_ps(&o->f[0], _mm_load_ps(&mat3f_identity[0]));
		_mm_store_ps(&o->f[4], _mm_load_ps(&mat3f_identity[4]));
	#else
		_mm_storeu_ps(&o->f[0], _mm_load_ps(&mat3f_identity[0]));
		_mm_storeu_ps(&o->f[4], _mm_load_ps(&mat3f_identity[4]));
	#endif
	o->f[8] = mat3f_identity[8];
}

MAT_FUNC void mat3f_inv(mat3f_t* o, mat3f_t* a)
{
	// load 3x3 matrix
	__m128
	#ifdef MAT3_ALIGN
		ld0 = _mm_load_ps(&a->f[0]),
		ld1 = _mm_load_ps(&a->f[4]),
	#else
		ld0 = _mm_loadu_ps(&a->f[0]),
		ld1 = _mm_loadu_ps(&a->f[4]),
	#endif
		ld2 = _mm_broadcast_ss(&a->f[8]);
	
	// get rows into their own registers
	__m128
		r0 = ld0,
		r1 = _mm_permute_ps(_mm_shuffle_ps(ld0, ld1, 0b01001111), 0b00111000),
		r2 = _mm_shuffle_ps(ld1, ld2, 0b00001110);
	
	// permute into intermediary groups
	__m128
		i01 = _mm_permute_ps(r0, 0b00000001), i02 = _mm_permute_ps(r0, 0b00011010),
		i11 = _mm_permute_ps(r1, 0b00000001), i12 = _mm_permute_ps(r1, 0b00011010),
		i21 = _mm_permute_ps(r2, 0b00000001), i22 = _mm_permute_ps(r2, 0b00011010);
		
	// compute matrix of minors
	__m128
		m0 = _mm_sub_ps(_mm_mul_ps(i11, i22), _mm_mul_ps(i21, i12)),
		m1 = _mm_sub_ps(_mm_mul_ps(i01, i22), _mm_mul_ps(i21, i02)),
		m2 = _mm_sub_ps(_mm_mul_ps(i01, i12), _mm_mul_ps(i11, i02));

	// compute determinant intermediary
	__m128
		di = _mm_mul_ps(r0, m0);
		
	// permute determinant intermediary groups
	__m128
		di0 = _mm_permute_ps(di, 0b00000000),
		di1 = _mm_permute_ps(di, 0b01010101),
		di2 = _mm_permute_ps(di, 0b10101010);
	
	// second determinant intermediary
	di = _mm_add_ps(di0, di2);
		
	// inverse determinant positive and negative
	__m128 
		idetp = _mm_rcp_ps(_mm_sub_ps(di, di1)),
		idetn = _mm_rcp_ps(_mm_sub_ps(di1, di));
		
	// mix positive and negative inverse determinants
	__m128
		idetm = _mm_shuffle_ps(idetp, idetn, 0b000000000);
		
	// calculate alternating sign inverse determinant for rows 0 and 2 and for row 1
	__m128
		asid0 = _mm_permute_ps(idetm, 0b00001000),
		asid1 = _mm_permute_ps(idetm, 0b00100010);
		
	// calculate matrix of cofactors
	m0 = _mm_mul_ps(m0, asid0);
	m1 = _mm_mul_ps(m1, asid1);
	m2 = _mm_mul_ps(m2, asid0);
	
	// transpose
	__m128
		o0 = _mm_shuffle_ps(_mm_shuffle_ps(m0, m1, 0b01000100), _mm_shuffle_ps(m2, m0, 0b01010000), 0b10001000), // m0[0], m1[0], m2[0], m0[1]
		o1 = _mm_shuffle_ps(_mm_shuffle_ps(m1, m2, 0b01010101), _mm_shuffle_ps(m0, m1, 0b10101010), 0b10001000), // m1[1], m2[1], m0[2], m1[2]
		o2 = _mm_permute_ps(m2, 0b10101010); // m2[2], m2[2], m2[2], m2[2]
	
	// store
	#ifdef MAT3_ALIGN
		_mm_store_ps(&o->f[0], o0);
		_mm_store_ps(&o->f[4], o1);
	#else
		_mm_storeu_ps(&o->f[0], o0);
		_mm_storeu_ps(&o->f[4], o1);
	#endif
	_mm_store_ss(&o->f[8], o2);
}

MAT_FUNC void mat3f_trans(mat3f_t* o, mat3f_t* a)
{
	// last element doesn't have to be touched as it stays in the same place
	__m128
	#ifdef MAT3_ALIGN
		ld0 = _mm_load_ps(&a->f[0]), // m0[0], m0[1], m0[2], m1[0]
		ld1 = _mm_load_ps(&a->f[4]); // m1[1], m1[2], m2[0], m2[1]
	#else
		ld0 = _mm_loadu_ps(&a->f[0]),
		ld1 = _mm_loadu_ps(&a->f[4]);
	#endif
	
	// transpose intermediary
	__m128
		tri = _mm_shuffle_ps(ld0, ld1, 0b10011001); // ld0[1], ld0[2], ld1[1], ld1[2]
	
	// transpose
	__m128
		o0 = _mm_shuffle_ps(ld0, tri, 0b00111100), //ld0[0], ld0[3], ld1[2], ld0[1]
		o1 = _mm_shuffle_ps(ld1, tri, 0b10011100); //ld1[0], ld1[3], ld0[2], ld1[1]
	
	// store
	#ifdef MAT3_ALIGN
		_mm_store_ps(&o->f[0], o0);
		_mm_store_ps(&o->f[4], o1);
	#else
		_mm_storeu_ps(&o->f[0], o0);
		_mm_storeu_ps(&o->f[4], o1);
	#endif	
}

MAT_FUNC void mat3f_add(mat3f_t* o, mat3f_t* a, mat3f_t* b)
{
	#ifdef MAT3_ALIGN
		_mm_store_ps(&o->f[0], _mm_add_ps(_mm_load_ps(&a->f[0]), _mm_load_ps(&b->f[0])));
		_mm_store_ps(&o->f[4], _mm_add_ps(_mm_load_ps(&a->f[4]), _mm_load_ps(&b->f[4])));
	#else
		_mm_storeu_ps(&o->f[0], _mm_add_ps(_mm_loadu_ps(&a->f[0]), _mm_loadu_ps(&b->f[0])));
		_mm_storeu_ps(&o->f[4], _mm_add_ps(_mm_loadu_ps(&a->f[4]), _mm_loadu_ps(&b->f[4])));
	#endif	
	o->f[8] = a->f[8] + b->f[8];
}

MAT_FUNC void mat3f_sub(mat3f_t* o, mat3f_t* a, mat3f_t* b)
{
	#ifdef MAT3_ALIGN
		_mm_store_ps(&o->f[0], _mm_sub_ps(_mm_load_ps(&a->f[0]), _mm_load_ps(&b->f[0])));
		_mm_store_ps(&o->f[4], _mm_sub_ps(_mm_load_ps(&a->f[4]), _mm_load_ps(&b->f[4])));
	#else
		_mm_storeu_ps(&o->f[0], _mm_sub_ps(_mm_loadu_ps(&a->f[0]), _mm_loadu_ps(&b->f[0])));
		_mm_storeu_ps(&o->f[4], _mm_sub_ps(_mm_loadu_ps(&a->f[4]), _mm_loadu_ps(&b->f[4])));
	#endif	
	o->f[8] = a->f[8] - b->f[8];
}

MAT_FUNC void mat3f_mul(mat3f_t* o, mat3f_t* a, mat3f_t* b)
{
	__m128
	#ifdef MAT3_ALIGN
		lda0 = _mm_load_ps(&a->f[0]), // m0[0], m0[1], m0[2], m1[0]
		lda1 = _mm_load_ps(&a->f[4]), // m1[1], m1[2], m2[0], m2[1]
	#else
		lda0 = _mm_loadu_ps(&a->f[0]),
		lda1 = _mm_loadu_ps(&a->f[4]),
	#endif
		lda2 = _mm_broadcast_ss(&a->f[8]); // m2[2]
	
	__m128
	#ifdef MAT3_ALIGN
		ldb0 = _mm_load_ps(&b->f[0]), // m0[0], m0[1], m0[2], m1[0]
		ldb1 = _mm_load_ps(&b->f[4]), // m1[1], m1[2], m2[0], m2[1]
	#else
		ldb0 = _mm_loadu_ps(&b->f[0]),
		ldb1 = _mm_loadu_ps(&b->f[4]),
	#endif
		ldb2 = _mm_broadcast_ss(&b->f[8]); // m2[2]
	
	// transpose matrix a
	__m128
		t0 = _mm_shuffle_ps(lda0, lda1, 0b00101100), // lda0[0], lda0[3], lda1[2]
		t1 = _mm_permute_ps(_mm_shuffle_ps(lda0, lda1, 0b11000001), 0b00111000), // lda0[1], lda1[0], lda1[3]
		t2 = _mm_shuffle_ps(_mm_shuffle_ps(lda0, lda1, 0b00010010), lda2, 0b00001000); // lda0[2], lda1[1], lda2[0]
	
	// extract rows from matrix b
	__m128
		r0 = ldb0,
		r1 = _mm_permute_ps(_mm_shuffle_ps(ldb0, ldb1, 0b01001111), 0b00111000),
		r2 = _mm_shuffle_ps(ldb1, ldb2, 0b00001110);
		
	// output registers
	__m128
		o0,
		o1,
		o2;
		
	// first row
	{
		__m128
			a0 = _mm_add_ps(
				_mm_mul_ps(_mm_permute_ps(t0, 0b00000000), r0),
				_mm_mul_ps(_mm_permute_ps(t2, 0b00000000), r2)
			),
			a1 =
				_mm_mul_ps(_mm_permute_ps(t1, 0b00000000), r1);

		o0 = _mm_add_ps(a0, a1);
	}
	
	// second row
	{
		__m128
			a0 = _mm_add_ps(
				_mm_mul_ps(_mm_permute_ps(t0, 0b01010101), r0),
				_mm_mul_ps(_mm_permute_ps(t2, 0b01010101), r2)
			),
			a1 =
				_mm_mul_ps(_mm_permute_ps(t1, 0b01010101), r1);

		o1 = _mm_add_ps(a0, a1);
	}
	
	// third row
	{
		__m128
			a0 = _mm_add_ps(
				_mm_mul_ps(_mm_permute_ps(t0, 0b10101010), r0),
				_mm_mul_ps(_mm_permute_ps(t2, 0b10101010), r2)
			),
			a1 =
				_mm_mul_ps(_mm_permute_ps(t1, 0b10101010), r1);

		o2 = _mm_add_ps(a0, a1);
	}
	
	// shuffle into store intermediaries
	__m128
		st0 = _mm_shuffle_ps(o0, _mm_shuffle_ps(o0, o1, 0b00001010), 0b10000100), // o0[0] o0[1] o0[2] o1[0]
		st1 = _mm_shuffle_ps(o1, o2, 0b01001001), // o1[1] o1[2] o2[0] 02[1]
		st2 = _mm_permute_ps(o2, 0b10101010); // o2[2]
	
	#ifdef MAT3_ALIGN
		_mm_store_ps(&o->f[0], st0);
		_mm_store_ps(&o->f[4], st1);
	#else
		_mm_storeu_ps(&o->f[0], st0);
		_mm_storeu_ps(&o->f[4], st1);
	#endif
	_mm_store_ss(&o->f[8], st2);
}

MAT_FUNC void mat3f_div(mat3f_t* o, mat3f_t* a, mat3f_t* b)
{
	mat3f_inv(o, b);
	mat3f_mul(o, a, o);
}

MAT_FUNC void mat3f_mulv3f(vec3f_t* o, mat3f_t* a, vec3f_t* b)
{
	// load matrix a
	__m128
	#ifdef MAT3_ALIGN
		lda0 = _mm_load_ps(&a->f[0]), // m0[0], m0[1], m0[2], m1[0]
		lda1 = _mm_load_ps(&a->f[4]), // m1[1], m1[2], m2[0], m2[1]
	#else
		lda0 = _mm_loadu_ps(&a->f[0]),
		lda1 = _mm_loadu_ps(&a->f[4]),
	#endif
		lda2 = _mm_broadcast_ss(&a->f[8]); // m2[2]
	
	// load vector b
	__m128		
		ldb0 = _mm_broadcast_ss(&b->f[0]),
		ldb1 = _mm_broadcast_ss(&b->f[1]),
		ldb2 = _mm_broadcast_ss(&b->f[2]);
		
	// transpose matrix a
	__m128
		t0 = _mm_shuffle_ps(lda0, lda1, 0b00101100), // lda0[0], lda0[3], lda1[2]
		t1 = _mm_permute_ps(_mm_shuffle_ps(lda0, lda1, 0b11000001), 0b00111000), // lda0[1], lda1[0], lda1[3]
		t2 = _mm_shuffle_ps(_mm_shuffle_ps(lda0, lda1, 0b00010010), lda2, 0b00001000); // lda0[2], lda1[1], lda2[0]

	// do math
	__m128
		mul = _mm_add_ps(
			_mm_add_ps(_mm_mul_ps(ldb0, t0), _mm_mul_ps(ldb1, t1)),
			_mm_mul_ps(ldb2, t2)
		);
	
	// store
	_mm_store_ss(&o->f[0], _mm_permute_ps(mul, 0b00000000));
	_mm_store_ss(&o->f[1], _mm_permute_ps(mul, 0b01010101));
	_mm_store_ss(&o->f[2], _mm_permute_ps(mul, 0b10101010));
}