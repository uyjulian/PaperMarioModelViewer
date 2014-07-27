/**
 * Copyright (c) 2009 Justin Aquadro
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef VMATH_VECMATH_H_
#define VMATH_VECMATH_H_

namespace vmath {

	const double PI = 3.14159265;

	template <class T>
	class Matrix3;

	template <class T>
	class Matrix4;

	template <class T>
	class Tuple2;

	template <class T>
	class Tuple3;

	template <class T>
	class Tuple4;

	template <class T>
	class Vector2;

	template <class T>
	class Vector2_T;

	template <class T>
	class Vector3;

	template <class T>
	class Vector3_T;

	template <class T>
	class Vector4;

	template <class T>
	class Vector4_T;

	/* Addition */

	template <typename T>
	inline Tuple2<T> operator+ (const Tuple2<T>& t1, const Tuple2<T>& t2) {
		Tuple2<T> op(t1);
		op.add(t2);
		return op;
	}

	template <typename T>
	inline Tuple3<T> operator+ (const Tuple3<T>& t1, const Tuple3<T>& t2) {
		Tuple3<T> op(t1);
		op.add(t2);
		return op;
	}

	template <typename T>
	inline Tuple4<T> operator+ (const Tuple4<T>& t1, const Tuple4<T>& t2) {
		Tuple4<T> op(t1);
		op.add(t2);
		return op;
	}

	template <typename T>
	inline Matrix3<T> operator+ (const Matrix3<T>& m1, const Matrix3<T>& m2) {
		Matrix3<T> op(m1);
		op.add(m2);
		return op;
	}

	template <typename T>
	inline Matrix4<T> operator+ (const Matrix4<T>& m1, const Matrix4<T>& m2) {
		Matrix4<T> op(m1);
		op.add(m2);
		return op;
	}

	template <typename T>
	inline Tuple2<T> operator+ (const Tuple2<T>& t, T s) {
		Tuple2<T> op(t);
		op.add(s);
		return op;
	}

	template <typename T>
	inline Tuple3<T> operator+ (const Tuple3<T>& t, T s) {
		Tuple3<T> op(t);
		op.add(s);
		return op;
	}

	template <typename T>
	inline Tuple4<T> operator+ (const Tuple4<T>& t, T s) {
		Tuple4<T> op(t);
		op.add(s);
		return op;
	}

	template <typename T>
	inline Matrix3<T> operator+ (const Matrix3<T>& m, T s) {
		Matrix3<T> op(m);
		op.add(s);
		return op;
	}

	template <typename T>
	inline Matrix4<T> operator+ (const Matrix4<T>& m, T s) {
		Matrix4<T> op(m);
		op.add(s);
		return op;
	}

	template <typename T>
	inline Tuple2<T> operator+ (T s, const Tuple2<T>& t) {
		Tuple2<T> op(s);
		op.add(t);
		return op;
	}

	template <typename T>
	inline Tuple3<T> operator+ (T s, const Tuple3<T>& t) {
		Tuple3<T> op(s);
		op.add(t);
		return op;
	}

	template <typename T>
	inline Tuple4<T> operator+ (T s, const Tuple4<T>& t) {
		Tuple4<T> op(s);
		op.add(t);
		return op;
	}

	/* Subtraction */

	template <typename T>
	inline Tuple2<T> operator- (const Tuple2<T>& t1, const Tuple2<T>& t2) {
		Tuple2<T> op(t1);
		op.sub(t2);
		return op;
	}

	template <typename T>
	inline Tuple3<T> operator- (const Tuple3<T>& t1, const Tuple3<T>& t2) {
		Tuple3<T> op(t1);
		op.sub(t2);
		return op;
	}

	template <typename T>
	inline Tuple4<T> operator- (const Tuple4<T>& t1, const Tuple4<T>& t2) {
		Tuple4<T> op(t1);
		op.sub(t2);
		return op;
	}

	template <typename T>
	inline Matrix3<T> operator- (const Matrix3<T>& m1, const Matrix3<T>& m2) {
		Matrix3<T> op(m1);
		op.sub(m2);
		return op;
	}

	template <typename T>
	inline Matrix4<T> operator- (const Matrix4<T>& m1, const Matrix4<T>& m2) {
		Matrix4<T> op(m1);
		op.sub(m2);
		return op;
	}

	template <typename T>
	inline Tuple2<T> operator- (const Tuple2<T>& t, T s) {
		Tuple2<T> op(t);
		op.sub(s);
		return op;
	}

	template <typename T>
	inline Tuple3<T> operator- (const Tuple3<T>& t, T s) {
		Tuple3<T> op(t);
		op.sub(s);
		return op;
	}

	template <typename T>
	inline Tuple4<T> operator- (const Tuple4<T>& t, T s) {
		Tuple4<T> op(t);
		op.sub(s);
		return op;
	}

	template <typename T>
	inline Matrix3<T> operator- (const Matrix3<T>& m, T s) {
		Matrix3<T> op(m);
		op.sub(s);
		return op;
	}

	template <typename T>
	inline Matrix4<T> operator- (const Matrix4<T>& m, T s) {
		Matrix4<T> op(m);
		op.sub(s);
		return op;
	}

	template <typename T>
	inline Tuple2<T> operator- (T s, const Tuple2<T>& t) {
		Tuple2<T> op(s);
		op.sub(t);
		return op;
	}

	template <typename T>
	inline Tuple3<T> operator- (T s, const Tuple3<T>& t) {
		Tuple3<T> op(s);
		op.sub(t);
		return op;
	}

	template <typename T>
	inline Tuple4<T> operator- (T s, const Tuple4<T>& t) {
		Tuple4<T> op(s);
		op.sub(t);
		return op;
	}

	/* Negation */

	template <typename T>
	inline Tuple2<T> operator- (const Tuple2<T>& t) {
		Tuple2<T> op(t);
		op.neg();
		return op;
	}

	template <typename T>
	inline Tuple3<T> operator- (const Tuple3<T>& t) {
		Tuple3<T> op(t);
		op.neg();
		return op;
	}

	template <typename T>
	inline Tuple4<T> operator- (const Tuple4<T>& t) {
		Tuple4<T> op(t);
		op.neg();
		return op;
	}

	/* Multiplication */

	template <typename T>
	inline Matrix3<T> operator* (const Matrix3<T>& m1, const Matrix3<T>& m2) {
		Matrix3<T> op(m1);
		op.mul(m2);
		return op;
	}

	template <typename T>
	inline Matrix4<T> operator* (const Matrix4<T>& m1, const Matrix4<T>& m2) {
		Matrix4<T> op(m1);
		op.mul(m2);
		return op;
	}

	template <typename T>
	inline Tuple2<T> operator* (const Tuple2<T>& t, T s) {
		Tuple2<T> op(t);
		op.scale(s);
		return op;
	}

	template <typename T>
	inline Tuple3<T> operator* (const Tuple3<T>& t, T s) {
		Tuple3<T> op(t);
		op.scale(s);
		return op;
	}

	template <typename T>
	inline Tuple4<T> operator* (const Tuple4<T>& t, T s) {
		Tuple4<T> op(t);
		op.scale(s);
		return op;
	}

	template <typename T>
	inline Matrix3<T> operator* (const Matrix3<T>& m, T s) {
		Matrix3<T> op(m);
		op.mul(s);
		return op;
	}

	template <typename T>
	inline Matrix4<T> operator* (const Matrix4<T>& m, T s) {
		Matrix4<T> op(m);
		op.mul(s);
		return op;
	}

	template <typename T>
	inline Tuple2<T> operator* (T s, const Tuple2<T>& t) {
		Tuple2<T> op(t);
		op.scale(s);
		return op;
	}

	template <typename T>
	inline Tuple3<T> operator* (T s, const Tuple3<T>& t) {
		Tuple3<T> op(t);
		op.scale(s);
		return op;
	}

	template <typename T>
	inline Tuple4<T> operator* (T s, const Tuple4<T>& t) {
		Tuple4<T> op(t);
		op.scale(s);
		return op;
	}

	template <typename T>
	inline Matrix3<T> operator* (T s, const Matrix3<T>& m) {
		Matrix3<T> op(m);
		op.mul(s);
		return op;
	}

	template <typename T>
	inline Matrix4<T> operator* (T s, const Matrix4<T>& m) {
		Matrix4<T> op(m);
		op.mul(s);
		return op;
	}

	template <typename T>
	inline Vector3<T> operator* (const Matrix3<T>& m, const Vector3<T>& v) {
		return m.mul(v);
	}

	template <typename T>
	inline Vector4<T> operator* (const Matrix4<T>& m, const Vector4<T>& v) {
		return m.mul(v);
	}

	template <typename T>
	inline Vector3_T<T> operator* (const Vector3_T<T>& vt, const Matrix3<T>& m) {
		Vector3_T<T> out;
		out.x = m._m[0] * vt.x + m._m[3] * vt.y + m._m[6] * vt.z;
		out.y = m._m[1] * vt.x + m._m[4] * vt.y + m._m[7] * vt.z;
		out.z = m._m[2] * vt.x + m._m[5] * vt.y + m._m[8] * vt.z;
		return out;
	}

	template <typename T>
	inline Vector4_T<T> operator* (const Vector4_T<T>& vt, const Matrix4<T>& m) {
		Vector4_T<T> out;
		out.x = m._m[0] * vt.x + m._m[4] * vt.y + m._m[8] * vt.z + m._m[12] * vt.w;
		out.y = m._m[1] * vt.x + m._m[5] * vt.y + m._m[9] * vt.z + m._m[13] * vt.w;
		out.z = m._m[2] * vt.x + m._m[6] * vt.y + m._m[10] * vt.z + m._m[14] * vt.w;
		out.w = m._m[3] * vt.x + m._m[7] * vt.y + m._m[11] * vt.z + m._m[15] * vt.w;
		return out;
	}

	template <typename T>
	inline T operator* (const Vector2_T<T>& vt, const Vector2<T>& v) {
		return vt.x * v.x + vt.y * v.y;
	}

	template <typename T>
	inline T operator* (const Vector3_T<T>& vt, const Vector3<T>& v) {
		return vt.x * v.x + vt.y * v.y + vt.z * v.z;
	}

	template <typename T>
	inline T operator* (const Vector4_T<T>& vt, const Vector4<T>& v) {
		return vt.x * v.x + vt.y * v.y + vt.z * v.z + vt.w * v.w;
	}

	template <typename T>
	inline Matrix3<T> operator* (const Vector3<T>& v, const Vector3_T<T>& vt) {
		Matrix3<T> out(v.x * vt.x, v.x * vt.y, v.x * vt.z,
					   v.y * vt.x, v.y * vt.y, v.y * vt.z,
					   v.z * vt.x, v.z * vt.y, v.z * vt.z);
		return out;
	}

	template <typename T>
	inline Matrix4<T> operator* (const Vector4<T>& v, const Vector4_T<T>& vt) {
		Matrix4<T> out(v.x * vt.x, v.x * vt.y, v.x * vt.z, v.x * vt.w,
					   v.y * vt.x, v.y * vt.y, v.y * vt.z, v.y * vt.w,
					   v.z * vt.x, v.z * vt.y, v.z * vt.z, v.z * vt.w,
					   v.w * vt.x, v.w * vt.y, v.w * vt.z, v.w * vt.w);
		return out;
	}

	/* Division */

	template <typename T>
	inline Tuple2<T> operator/ (const Tuple2<T>& m, T s) {
		Tuple2<T> op(m);
		op.scale(1.0 / s);
		return op;
	}

	template <typename T>
	inline Tuple3<T> operator/ (const Tuple3<T>& m, T s) {
		Tuple3<T> op(m);
		op.scale(1.0 / s);
		return op;
	}

	template <typename T>
	inline Tuple4<T> operator/ (const Tuple4<T>& m, T s) {
		Tuple4<T> op(m);
		op.scale(1.0 / s);
		return op;
	}

	template <typename T>
	inline Matrix3<T> operator/ (const Matrix3<T>& m, T s) {
		Matrix3<T> op(m);
		op.mul(1.0 / s);
		return op;
	}

	template <typename T>
	inline Matrix4<T> operator/ (const Matrix4<T>& m, T s) {
		Matrix4<T> op(m);
		op.mul(1.0 / s);
		return op;
	}

}

#include "Tuple2.h"
#include "Tuple3.h"
#include "Tuple4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include "Matrix4.h"

#endif /* VMATH_VECMATH_H_ */
