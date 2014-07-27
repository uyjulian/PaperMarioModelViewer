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

#ifndef VMATH_VECTOR4_H_
#define VMATH_VECTOR4_H_

#include <cmath>
#include "Vecmath.h"

namespace vmath {

	template <class T>
	class Vector4 : public Tuple4<T> {
	public:

		using Tuple4<T>::x;
		using Tuple4<T>::y;
		using Tuple4<T>::z;
		using Tuple4<T>::w;

		static const Vector4<T> UNIT_X;
		static const Vector4<T> UNIT_Y;
		static const Vector4<T> UNIT_Z;
		static const Vector4<T> UNIT_W;

	public:

		Vector4 ()
			: Tuple4<T>() {
		}

		Vector4 (T s)
			: Tuple4<T>(s) {
		}

		Vector4 (T fx, T fy, T fz, T wz)
			: Tuple4<T>(fx, fy, fz, wz) {
		}

		Vector4 (const Tuple3<T>& t3f, T s)
			: Tuple4<T>(t3f, s) {
		}

		Vector4 (const Tuple4<T>& t4f)
			: Tuple4<T>(t4f) {
		}

		Vector4 (const Tuple4<T>* t4f)
			: Tuple4<T>(t4f) {
		}

		Vector4 (const T* p)
			: Tuple4<T>(p) {
		}

		//virtual ~Vector4 () { }

		T angle (const Vector4<T>& v4f) const {
			Vector4<T> a(*this);
			Vector4<T> b(v4f);
			a.normalize();
			b.normalize();
			return acos(a.dot(b));
		}

		T dot (const Vector4<T>& v4f) const {
			return x * v4f.x + y * v4f.y + z * v4f.z + w * v4f.w;
		}

		T length () const {
			return sqrt (lengthSquared());
		}

		T lengthSquared () const {
			return x * x + y * y + z * z + w * w;
		}

		void normalize () {
			T mag = length();
			if (mag != 0) {
				x /= mag;
				y /= mag;
				z /= mag;
				w /= mag;
			}
		}

		Vector4<T> normalized () const {
			Vector4<T> op(*this);
			op.normalize();
			return op;
		}

		Vector4_T<T> transpose () const {
			Vector4_T<T> op(*this);
			return op;
		}
	};

	template <typename T>
	const Vector4<T> Vector4<T>::UNIT_X(1, 0, 0, 0);

	template <typename T>
	const Vector4<T> Vector4<T>::UNIT_Y(0, 1, 0, 0);

	template <typename T>
	const Vector4<T> Vector4<T>::UNIT_Z(0, 0, 1, 0);

	template <typename T>
	const Vector4<T> Vector4<T>::UNIT_W(0, 0, 0, 1);

	typedef Vector4<double> Vector4d;
	typedef Vector4<float> Vector4f;

	/**
	 * Transpose vector, useful for some matrix arithmetic.
	 */

	template <class T>
	class Vector4_T : public Tuple4<T> {
	public:

		Vector4_T ()
			: Tuple4<T>() {
		}

		Vector4_T (T s)
			: Tuple4<T>(s) {
		}

		Vector4_T (T fx, T fy, T fz)
			: Tuple4<T>(fx, fy, fz) {
		}

		Vector4_T (const Tuple4<T>& t3f)
			: Tuple4<T>(t3f) {
		}

		Vector4_T (const T* p)
			: Tuple4<T>(p) {
		}

		//virtual ~Vector4_T () { }

		Vector4<T> transpose () const {
			Vector4<T> op(*this);
			return op;
		}

	};

	typedef Vector4_T<double> Vector4d_T;
	typedef Vector4_T<float> Vector4f_T;

}

#endif /* VMATH_VECTOR4_H_ */
