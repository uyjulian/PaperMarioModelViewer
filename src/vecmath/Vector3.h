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

#ifndef VMATH_VECTOR3_H_
#define VMATH_VECTOR3_H_

#include <cmath>
#include "Vecmath.h"

namespace vmath {

	template <class T>
	class Vector3 : public Tuple3<T> {
	public:

		using Tuple3<T>::x;
		using Tuple3<T>::y;
		using Tuple3<T>::z;

		static const Vector3<T> UNIT_X;
		static const Vector3<T> UNIT_Y;
		static const Vector3<T> UNIT_Z;

	public:

		Vector3 ()
			: Tuple3<T>() {
		}

		Vector3 (T s)
			: Tuple3<T>(s) {
		}

		Vector3 (T fx, T fy, T fz)
			: Tuple3<T>(fx, fy, fz) {
		}

		Vector3 (const Tuple3<T>& t3f)
			: Tuple3<T>(t3f) {
		}

		Vector3 (const Tuple3<T>* t3f)
			: Tuple3<T>(t3f) {
		}

		Vector3 (const T* p)
			: Tuple3<T>(p) {
		}

		//virtual ~Vector3 () { }

		T angle (const Vector3<T>& v3f) const {
			Vector3<T> a(*this);
			Vector3<T> b(v3f);
			a.normalize();
			b.normalize();
			return acos(a.dot(b));
		}

		Vector3<T> cross (const Vector3<T>& v3f) const {
			T i = y * v3f.z - z * v3f.y;
			T j = z * v3f.x - x * v3f.z;
			T k = x * v3f.y - y * v3f.x;
			return Vector3<T>(i, j, k);
		}

		T dot (const Vector3<T>& v3f) const {
			return x * v3f.x + y * v3f.y + z * v3f.z;
		}

		T length () const {
			return sqrt (lengthSquared());
		}

		T lengthSquared () const {
			return x * x + y * y + z * z;
		}

		void normalize () {
			T mag = length();
			if (mag != 0) {
				x /= mag;
				y /= mag;
				z /= mag;
			}
		}

		Vector3<T> normalized () const {
			Vector3<T> op(*this);
			op.normalize();
			return op;
		}

		Vector3_T<T> transpose () const {
			Vector3_T<T> op(*this);
			return op;
		}
	};

	template <typename T>
	const Vector3<T> Vector3<T>::UNIT_X(1, 0, 0);

	template <typename T>
	const Vector3<T> Vector3<T>::UNIT_Y(0, 1, 0);

	template <typename T>
	const Vector3<T> Vector3<T>::UNIT_Z(0, 0, 1);


	typedef Vector3<double> Vector3d;
	typedef Vector3<float> Vector3f;

	/**
	 * Transpose vector, useful for some matrix arithmetic.
	 */

	template <class T>
	class Vector3_T : public Tuple3<T> {
	public:

		Vector3_T ()
			: Tuple3<T>() {
		}

		Vector3_T (T s)
			: Tuple3<T>(s) {
		}

		Vector3_T (T fx, T fy, T fz)
			: Tuple3<T>(fx, fy, fz) {
		}

		Vector3_T (const Tuple3<T>& t3f)
			: Tuple3<T>(t3f) {
		}

		Vector3_T (const T* p)
			: Tuple3<T>(p) {
		}

		//virtual ~Vector3_T () { }

		Vector3<T> transpose () const {
			Vector3<T> op(*this);
			return op;
		}

	};

	typedef Vector3_T<double> Vector3d_T;
	typedef Vector3_T<float> Vector3f_T;
}

#endif /* VMATH_VECTOR3_H_ */
