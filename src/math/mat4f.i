#include <immintrin.h>

extern const float mat4f_identity[16];

MAT_FUNC void mat4f_addf(mat4f_t* o, mat4f_t* a, float b)
{
	__m256
		broad = _mm256_broadcast_ss(&b);
	_mm256_store_ps(&o->f[0], _mm256_add_ps(_mm256_load_ps(&a->f[0]), broad));
	_mm256_store_ps(&o->f[8], _mm256_add_ps(_mm256_load_ps(&a->f[8]), broad));
}

MAT_FUNC void mat4f_subf(mat4f_t* o, mat4f_t* a, float b)
{
	__m256
		broad = _mm256_broadcast_ss(&b);
	_mm256_store_ps(&o->f[0], _mm256_sub_ps(_mm256_load_ps(&a->f[0]), broad));
	_mm256_store_ps(&o->f[8], _mm256_sub_ps(_mm256_load_ps(&a->f[8]), broad));
}

MAT_FUNC void mat4f_mulf(mat4f_t* o, mat4f_t* a, float b)
{
	__m256
		broad = _mm256_broadcast_ss(&b);
	_mm256_store_ps(&o->f[0], _mm256_mul_ps(_mm256_load_ps(&a->f[0]), broad));
	_mm256_store_ps(&o->f[8], _mm256_mul_ps(_mm256_load_ps(&a->f[8]), broad));
}

MAT_FUNC void mat4f_divf(mat4f_t* o, mat4f_t* a, float b)
{
	__m256
		broad = _mm256_broadcast_ss(&b);
	_mm256_store_ps(&o->f[0], _mm256_div_ps(_mm256_load_ps(&a->f[0]), broad));
	_mm256_store_ps(&o->f[8], _mm256_div_ps(_mm256_load_ps(&a->f[8]), broad));
}

MAT_FUNC void mat4f_iden(mat4f_t* o)
{
	_mm256_store_ps(&o->f[0], _mm256_load_ps(&mat4f_identity[0]));
	_mm256_store_ps(&o->f[8], _mm256_load_ps(&mat4f_identity[8]));
}

MAT_FUNC void mat4f_trans(mat4f_t* o, mat4f_t* a)
{
	// load input matrix
	__m128
		r0 = _mm_load_ps(&a->f[0]),
		r1 = _mm_load_ps(&a->f[4]),
		r2 = _mm_load_ps(&a->f[8]),
		r3 = _mm_load_ps(&a->f[12]);

	// compute transpose intermediary groups
	__m128
		tr01a = _mm_permute_ps(_mm_shuffle_ps(r0, r1, 0b01000100), 0b11011000),
		tr01b = _mm_permute_ps(_mm_shuffle_ps(r0, r1, 0b11101110), 0b11011000),
		tr23a = _mm_permute_ps(_mm_shuffle_ps(r2, r3, 0b01000100), 0b11011000),
		tr23b = _mm_permute_ps(_mm_shuffle_ps(r2, r3, 0b11101110), 0b11011000);

	// transpose matrix
	__m128
		o0 = _mm_shuffle_ps(tr01a, tr23a, 0b01000100),
		o1 = _mm_shuffle_ps(tr01a, tr23a, 0b11101110),
		o2 = _mm_shuffle_ps(tr01b, tr23b, 0b01000100),
		o3 = _mm_shuffle_ps(tr01b, tr23b, 0b11101110);

	// store result
	_mm_store_ps(&o->f[0], o0);
	_mm_store_ps(&o->f[4], o1);
	_mm_store_ps(&o->f[8], o2);
	_mm_store_ps(&o->f[12], o3);
}

MAT_FUNC void mat4f_inv(mat4f_t* o, mat4f_t* a)
{
	// load input matrix
	__m128
		r0 = _mm_load_ps(&a->f[0]),
		r1 = _mm_load_ps(&a->f[4]),
		r2 = _mm_load_ps(&a->f[8]),
		r3 = _mm_load_ps(&a->f[12]);

	// permute into intermediary groups
	__m128
		i01 = _mm_permute_ps(r0, 0b00000001), i02 = _mm_permute_ps(r0, 0b01011010), i03 = _mm_permute_ps(r0, 0b10111111),
		i11 = _mm_permute_ps(r1, 0b00000001), i12 = _mm_permute_ps(r1, 0b01011010), i13 = _mm_permute_ps(r1, 0b10111111),
		i21 = _mm_permute_ps(r2, 0b00000001), i22 = _mm_permute_ps(r2, 0b01011010), i23 = _mm_permute_ps(r2, 0b10111111),
		i31 = _mm_permute_ps(r3, 0b00000001), i32 = _mm_permute_ps(r3, 0b01011010), i33 = _mm_permute_ps(r3, 0b10111111);

	// compute secondary intermediary groups
	__m128
		j0 = _mm_sub_ps(_mm_mul_ps(i22, i33), _mm_mul_ps(i23, i32)),
		j1 = _mm_sub_ps(_mm_mul_ps(i23, i31), _mm_mul_ps(i21, i33)),
		j2 = _mm_sub_ps(_mm_mul_ps(i21, i32), _mm_mul_ps(i22, i31)),
		j3 = _mm_sub_ps(_mm_mul_ps(i02, i13), _mm_mul_ps(i03, i12)),
		j4 = _mm_sub_ps(_mm_mul_ps(i03, i11), _mm_mul_ps(i01, i13)),
		j5 = _mm_sub_ps(_mm_mul_ps(i01, i12), _mm_mul_ps(i02, i11));

	// compute matrix of minors
	__m128
		m0 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(i11 , j0), _mm_mul_ps(i12 , j1)), _mm_mul_ps(i13, j2)),
		m1 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(i01 , j0), _mm_mul_ps(i02 , j1)), _mm_mul_ps(i03, j2)),
		m2 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(i31 , j3), _mm_mul_ps(i32 , j4)), _mm_mul_ps(i33, j5)),
		m3 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(i21 , j3), _mm_mul_ps(i22 , j4)), _mm_mul_ps(i23, j5));

	// calculate determinant intermediary, determinant is [0] - [1] + [2] - [3] of r0 * m0
	__m128
		di = _mm_mul_ps(r0, m0);

	// calculate alternating sign inverse determinant for rows 0 and 2
	__m128
		asid0 = _mm_rcp_ps(
					_mm_add_ps(
						_mm_sub_ps(_mm_permute_ps(di, 0b01000100), _mm_permute_ps(di, 0b00010001)),
						_mm_sub_ps(_mm_permute_ps(di, 0b11101110), _mm_permute_ps(di, 0b10111011))
						)
					);

	// compute alternating sign inverse determinant for rows 1 and 3
	__m128
		asid1 = _mm_permute_ps(asid0, 0b00010001);

	// calculate matrix of cofactors
	m0 = _mm_mul_ps(m0, asid0);
	m1 = _mm_mul_ps(m1, asid1);
	m2 = _mm_mul_ps(m2, asid0);
	m3 = _mm_mul_ps(m3, asid1);

	// compute transpose intermediary groups
	__m128
		tr01a = _mm_permute_ps(_mm_shuffle_ps(m0, m1, 0b01000100), 0b11011000), // [0], [1] of m0 and m1 => {m0[0], m1[0], m0[1], m1[1]}
		tr01b = _mm_permute_ps(_mm_shuffle_ps(m0, m1, 0b11101110), 0b11011000), // [2], [3] of m0 and m1 => {m0[2], m1[2], m0[3], m1[3]}
		tr23a = _mm_permute_ps(_mm_shuffle_ps(m2, m3, 0b01000100), 0b11011000), // [0], [1] of m2 and m3 => {m2[0], m3[0], m2[1], m3[1]}
		tr23b = _mm_permute_ps(_mm_shuffle_ps(m2, m3, 0b11101110), 0b11011000); // [2], [3] of m2 and m3 => {m2[2], m3[2], m2[3], m3[3]}

	// transpose matrix
	__m128
		o0 = _mm_shuffle_ps(tr01a, tr23a, 0b01000100), // m0[0], m1[0], m2[0], m3[0]
		o1 = _mm_shuffle_ps(tr01a, tr23a, 0b11101110), // m0[1], m1[1], m2[1], m3[1]
		o2 = _mm_shuffle_ps(tr01b, tr23b, 0b01000100), // m0[2], m1[2], m2[2], m3[2]
		o3 = _mm_shuffle_ps(tr01b, tr23b, 0b11101110); // m0[3], m1[3], m2[3], m3[3]

	// store result
	_mm_store_ps(&o->f[0], o0);
	_mm_store_ps(&o->f[4], o1);
	_mm_store_ps(&o->f[8], o2);
	_mm_store_ps(&o->f[12], o3);
}

MAT_FUNC void mat4f_add(mat4f_t* o, mat4f_t* a, mat4f_t* b)
{
	_mm256_store_ps(&o->f[0], _mm256_add_ps(_mm256_load_ps(&a->f[0]), _mm256_load_ps(&b->f[0])));
	_mm256_store_ps(&o->f[8], _mm256_add_ps(_mm256_load_ps(&a->f[8]), _mm256_load_ps(&b->f[8])));
}

MAT_FUNC void mat4f_sub(mat4f_t* o, mat4f_t* a, mat4f_t* b)
{
	_mm256_store_ps(&o->f[0], _mm256_sub_ps(_mm256_load_ps(&a->f[0]), _mm256_load_ps(&b->f[0])));
	_mm256_store_ps(&o->f[8], _mm256_sub_ps(_mm256_load_ps(&a->f[8]), _mm256_load_ps(&b->f[8])));
}

MAT_FUNC void mat4f_mul(mat4f_t* o, mat4f_t* a, mat4f_t* b)
{
	// load matrix a
	__m128
		r0 = _mm_load_ps(&a->f[0]),
		r1 = _mm_load_ps(&a->f[4]),
		r2 = _mm_load_ps(&a->f[8]),
		r3 = _mm_load_ps(&a->f[12]);

	// load matrix b
	__m128
		b0 = _mm_load_ps(&b->f[0]),
		b1 = _mm_load_ps(&b->f[4]),
		b2 = _mm_load_ps(&b->f[8]),
		b3 = _mm_load_ps(&b->f[12]);

	// transpose matrix a
	__m128
		tr01a = _mm_permute_ps(_mm_shuffle_ps(r0, r1, 0b01000100), 0b11011000),
		tr01b = _mm_permute_ps(_mm_shuffle_ps(r0, r1, 0b11101110), 0b11011000),
		tr23a = _mm_permute_ps(_mm_shuffle_ps(r2, r3, 0b01000100), 0b11011000),
		tr23b = _mm_permute_ps(_mm_shuffle_ps(r2, r3, 0b11101110), 0b11011000);

	__m128
		t0 = _mm_shuffle_ps(tr01a, tr23a, 0b01000100),
		t1 = _mm_shuffle_ps(tr01a, tr23a, 0b11101110),
		t2 = _mm_shuffle_ps(tr01b, tr23b, 0b01000100),
		t3 = _mm_shuffle_ps(tr01b, tr23b, 0b11101110);

	// first row
	{
		__m128
			a0 = _mm_add_ps(
				_mm_mul_ps(_mm_permute_ps(t0, 0b00000000), b0),
				_mm_mul_ps(_mm_permute_ps(t2, 0b00000000), b2)
			),
			a1 = _mm_add_ps(
				_mm_mul_ps(_mm_permute_ps(t1, 0b00000000), b1),
				_mm_mul_ps(_mm_permute_ps(t3, 0b00000000), b3)
			);

		_mm_store_ps(&o->f[0], _mm_add_ps(a0, a1));
	}

	// second row
	{
		__m128
			a0 = _mm_add_ps(
				_mm_mul_ps(_mm_permute_ps(t0, 0b01010101), b0),
				_mm_mul_ps(_mm_permute_ps(t2, 0b01010101), b2)
			),
			a1 = _mm_add_ps(
				_mm_mul_ps(_mm_permute_ps(t1, 0b01010101), b1),
				_mm_mul_ps(_mm_permute_ps(t3, 0b01010101), b3)
			);

		_mm_store_ps(&o->f[4], _mm_add_ps(a0, a1));
	}

	// third row
	{
		__m128
			a0 = _mm_add_ps(
				_mm_mul_ps(_mm_permute_ps(t0, 0b10101010), b0),
				_mm_mul_ps(_mm_permute_ps(t2, 0b10101010), b2)
			),
			a1 = _mm_add_ps(
				_mm_mul_ps(_mm_permute_ps(t1, 0b10101010), b1),
				_mm_mul_ps(_mm_permute_ps(t3, 0b10101010), b3)
			);

		_mm_store_ps(&o->f[8], _mm_add_ps(a0, a1));
	}

	// fourth row
	{
		__m128
			a0 = _mm_add_ps(
				_mm_mul_ps(_mm_permute_ps(t0, 0b11111111), b0),
				_mm_mul_ps(_mm_permute_ps(t2, 0b11111111), b2)
			),
			a1 = _mm_add_ps(
				_mm_mul_ps(_mm_permute_ps(t1, 0b11111111), b1),
				_mm_mul_ps(_mm_permute_ps(t3, 0b11111111), b3)
			);

		_mm_store_ps(&o->f[12], _mm_add_ps(a0, a1));
	}
}

MAT_FUNC void mat4f_div(mat4f_t* o, mat4f_t* a, mat4f_t* b)
{
	mat4f_inv(o, b);
	mat4f_mul(o, a, o);
}

MAT_FUNC void mat4f_mulv4f(vec4f_t* o, mat4f_t* a, vec4f_t* b)
{
	// load matrix a
	__m128
		r0 = _mm_load_ps(&a->f[0]),
		r1 = _mm_load_ps(&a->f[4]),
		r2 = _mm_load_ps(&a->f[8]),
		r3 = _mm_load_ps(&a->f[12]);

	// load vector b
	__m128
		v = _mm_load_ps(&b->f[0]);

	// transpose matrix a
	__m128
		tr01a = _mm_permute_ps(_mm_shuffle_ps(r0, r1, 0b01000100), 0b11011000),
		tr01b = _mm_permute_ps(_mm_shuffle_ps(r0, r1, 0b11101110), 0b11011000),
		tr23a = _mm_permute_ps(_mm_shuffle_ps(r2, r3, 0b01000100), 0b11011000),
		tr23b = _mm_permute_ps(_mm_shuffle_ps(r2, r3, 0b11101110), 0b11011000);

	__m128
		t0 = _mm_shuffle_ps(tr01a, tr23a, 0b01000100),
		t1 = _mm_shuffle_ps(tr01a, tr23a, 0b11101110),
		t2 = _mm_shuffle_ps(tr01b, tr23b, 0b01000100),
		t3 = _mm_shuffle_ps(tr01b, tr23b, 0b11101110);

	// permute column vector into rows
	__m128
		p0 = _mm_permute_ps(v, 0b00000000),
		p1 = _mm_permute_ps(v, 0b01010101),
		p2 = _mm_permute_ps(v, 0b10101010),
		p3 = _mm_permute_ps(v, 0b11111111);

	// do the math
	_mm_store_ps(&o->f[0],
		_mm_add_ps(
			_mm_add_ps(_mm_mul_ps(p0, t0), _mm_mul_ps(p1, t1)),
			_mm_add_ps(_mm_mul_ps(p2, t2), _mm_mul_ps(p3, t3))
		)
	);
}