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

#ifndef VMATH_VECTOR2_H_
#define VMATH_VECTOR2_H_

#include <cmath>
#include "Vecmath.h"

namespace vmath {

	template <class T>
	class Vector2 : public Tuple2<T> {
	public:

		using Tuple2<T>::x;
		using Tuple2<T>::y;

		static const Vector2<T> UNIT_X;
		static const Vector2<T> UNIT_Y;

	public:

		Vector2 ()
			: Tuple2<T>() {
		}

		Vector2 (T s)
			: Tuple2<T>(s) {
		}

		Vector2 (T fx, T fy)
			: Tuple2<T>(fx, fy) {
		}

		Vector2 (const Tuple2<T>& t2f)
			: Tuple2<T>(t2f) {
		}

		Vector2 (const T* p)
			: Tuple2<T>(p) {
		}

		//virtual ~Vector2 () { }

		T angle (const Vector2<T>& v2f) const {
			Vector2<T> a(*this);
			Vector2<T> b(v2f);
			a.normalize();
			b.normalize();
			return acos(a.dot(b));
		}

		T dot (const Vector2<T>& v2f) const {
			return x * v2f.x + y * v2f.y;
		}

		T length () const {
			return sqrt (lengthSquared());
		}

		T lengthSquared () const {
			return x * x + y * y;
		}

		void normalize () {
			T mag = length();
			if (mag != 0) {
				x /= mag;
				y /= mag;
			}
		}

		Vector2<T> normalized () const {
			Vector2<T> op(*this);
			op.normalize();
			return op;
		}

		Vector2_T<T> transpose () const {
			Vector2_T<T> op(*this);
			return op;
		}
	};

	template <typename T>
	const Vector2<T> Vector2<T>::UNIT_X(1, 0);

	template <typename T>
	const Vector2<T> Vector2<T>::UNIT_Y(0, 1);


	typedef Vector2<double> Vector2d;
	typedef Vector2<float> Vector2f;

	/**
	 * Transpose vector, useful for some matrix arithmetic.
	 */

	template <class T>
	class Vector2_T : public Tuple2<T> {
	public:

		Vector2_T ()
			: Tuple2<T>() {
		}

		Vector2_T (T s)
			: Tuple2<T>(s) {
		}

		Vector2_T (T fx, T fy)
			: Tuple2<T>(fx, fy) {
		}

		Vector2_T (const Tuple2<T>& t2f)
			: Tuple2<T>(t2f) {
		}

		Vector2_T (const Tuple2<T>* t2f)
			: Tuple2<T>(t2f) {
		}

		Vector2_T (const T* p)
			: Tuple2<T>(p) {
		}

		//virtual ~Vector2_T () { }

		Vector2<T> transpose () const {
			Vector2<T> op(*this);
			return op;
		}

	};

	typedef Vector2_T<double> Vector2d_T;
	typedef Vector2_T<float> Vector2f_T;
}

#endif /* VMATH_VECTOR2_H_ */
