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

#ifndef VMATH_MATRIXG4_H_
#define VMATH_MATRIXG4_H_

#include <cmath>

#include "Vecmath.h"

namespace vmath {

	template <class T>
	class MatrixG4 : public Matrix4<T> {
	protected:

		using Matrix4<T>::_m;

	public:

		MatrixG4 ()
			: Matrix4<T>() {
		}

		MatrixG4 (T a00, T a01, T a02, T a03, T a10, T a11, T a12, T a13, T a20, T a21, T a22, T a23, T a30, T a31, T a32, T a33)
			: Matrix4<T>(a00, a01, a02, a03, a10, a11, a12, a13, a20, a21, a22, a23, a30, a31, a32, a33) {
		}

		MatrixG4 (const T* p)
			: Matrix4<T>(p) {
		}

		MatrixG4 (const Matrix4<T>& m)
			: Matrix4<T>(m) {
		}

		void rotate (T a, T x, T y, T z) {
			T rada = radians(a);
			T sina = std::sin(rada);
			T cosa = std::cos(rada);

			Vector3<T> u(x, y, z);
			u.normalize();

			Matrix3<T> uuT = u * u.transpose();

			Matrix3<T> S(0, -u.z, u.y, u.z, 0, -u.x, -u.y, u.x, 0);

			Matrix3<T> M = uuT + cosa * (Matrix3<T>::IDENTITY - uuT) + sina * S;

			Vector3<T> r0 = M.getRow(0);
			Vector3<T> r1 = M.getRow(1);
			Vector3<T> r2 = M.getRow(2);

			T tmp1, tmp2, tmp3;

			tmp1 = _m[0] * r0.x + _m[1] * r1.x + _m[2] * r2.x;
			tmp2 = _m[0] * r0.y + _m[1] * r1.y + _m[2] * r2.y;
			tmp3 = _m[0] * r0.z + _m[1] * r1.z + _m[2] * r2.z;
			_m[0] = tmp1;	_m[1] = tmp2;	_m[2] = tmp3;

			tmp1 = _m[4] * r0.x + _m[5] * r1.x + _m[6] * r2.x;
			tmp2 = _m[4] * r0.y + _m[5] * r1.y + _m[6] * r2.y;
			tmp3 = _m[4] * r0.z + _m[5] * r1.z + _m[6] * r2.z;
			_m[4] = tmp1;	_m[5] = tmp2;	_m[6] = tmp3;

			tmp1 = _m[8] * r0.x + _m[9] * r1.x + _m[10] * r2.x;
			tmp2 = _m[8] * r0.y + _m[9] * r1.y + _m[10] * r2.y;
			tmp3 = _m[8] * r0.z + _m[9] * r1.z + _m[10] * r2.z;
			_m[8] = tmp1;	_m[9] = tmp2;	_m[10] = tmp3;

			tmp1 = _m[12] * r0.x + _m[13] * r1.x + _m[14] * r2.x;
			tmp2 = _m[12] * r0.y + _m[13] * r1.y + _m[14] * r2.y;
			tmp3 = _m[12] * r0.z + _m[13] * r1.z + _m[14] * r2.z;
			_m[12] = tmp1;	_m[13] = tmp2;	_m[14] = tmp3;
		}

		void rotateX (T a) {
			T rada = radians(a);
			T sina = std::sin(rada);
			T cosa = std::cos(rada);
			T tmp1, tmp2;

			tmp1 = _m[1] * cosa + _m[2] * sina;
			tmp2 = _m[1] * -sina + _m[2] * cosa;
			_m[1] = tmp1;	_m[2] = tmp2;

			tmp1 = _m[5] * cosa + _m[6] * sina;
			tmp2 = _m[5] * -sina + _m[6] * cosa;
			_m[5] = tmp1;	_m[6] = tmp2;

			tmp1 = _m[9] * cosa + _m[10] * sina;
			tmp2 = _m[9] * -sina + _m[10] * cosa;
			_m[9] = tmp1;	_m[10] = tmp2;

			tmp1 = _m[13] * cosa + _m[14] * sina;
			tmp2 = _m[13] * -sina + _m[14] * cosa;
			_m[13] = tmp1;	_m[14] = tmp2;
		}

		void rotateY (T a) {
			T rada = radians(a);
			T sina = std::sin(rada);
			T cosa = std::cos(rada);
			T tmp1, tmp2;

			tmp1 = _m[0] * cosa + _m[2] * -sina;
			tmp2 = _m[0] * sina + _m[2] * cosa;
			_m[0] = tmp1;	_m[2] = tmp2;

			tmp1 = _m[4] * cosa + _m[6] * -sina;
			tmp2 = _m[4] * sina + _m[6] * cosa;
			_m[4] = tmp1;	_m[6] = tmp2;

			tmp1 = _m[8] * cosa + _m[10] * -sina;
			tmp2 = _m[8] * sina + _m[10] * cosa;
			_m[8] = tmp1;	_m[10] = tmp2;

			tmp1 = _m[12] * cosa + _m[14] * -sina;
			tmp2 = _m[12] * sina + _m[14] * cosa;
			_m[12] = tmp1;	_m[14] = tmp2;
		}

		void rotateZ (T a) {
			T rada = radians(a);
			T sina = std::sin(rada);
			T cosa = std::cos(rada);
			T tmp1, tmp2;

			tmp1 = _m[0] * cosa + _m[1] * sina;
			tmp2 = _m[0] * -sina + _m[1] * cosa;
			_m[0] = tmp1;	_m[1] = tmp2;

			tmp1 = _m[4] * cosa + _m[5] * sina;
			tmp2 = _m[4] * -sina + _m[5] * cosa;
			_m[4] = tmp1;	_m[5] = tmp2;

			tmp1 = _m[8] * cosa + _m[9] * sina;
			tmp2 = _m[8] * -sina + _m[9] * cosa;
			_m[8] = tmp1;	_m[9] = tmp2;

			tmp1 = _m[12] * cosa + _m[13] * sina;
			tmp2 = _m[12] * -sina + _m[13] * cosa;
			_m[12] = tmp1;	_m[13] = tmp2;
		}

		void scale (T x, T y, T z) {
			_m[0] *= x;
			_m[4] *= x;
			_m[8] *= x;
			_m[12] *= x;

			_m[1] *= y;
			_m[5] *= y;
			_m[9] *= y;
			_m[13] *= y;

			_m[2] *= z;
			_m[6] *= z;
			_m[10] *= z;
			_m[14] *= z;
		}

		void transform (Tuple3<T>& t) const {
			T tx = _m[0] * t.x + _m[1] * t.y + _m[2] * t.z + _m[3];
			T ty = _m[4] * t.x + _m[5] * t.y + _m[6] * t.z + _m[7];
			T tz = _m[8] * t.x + _m[9] * t.y + _m[10] * t.z + _m[11];

			t.set(tx, ty, tz);
		}

		void translate (T x, T y, T z) {
			_m[3] += _m[0] * x + _m[1] * y + _m[2] * z;
			_m[7] += _m[4] * x + _m[5] * y + _m[6] * z;
			_m[11] += _m[8] * x + _m[9] * y + _m[10] * z;
			_m[15] += _m[12] * x + _m[13] * y + _m[14] * z;
		}

	protected:

		static T radians (T degrees) {
			//const double PI = 3.14159265;
			return (degrees * PI / 180.0);
		}
	};

	typedef MatrixG4<float> MatrixG4f;
	typedef MatrixG4<double> MatrixG4d;

}

#endif /* VMATH_MATRIXG4_H_ */
