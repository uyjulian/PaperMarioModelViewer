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

#ifndef VMATH_MATRIXG3_H_
#define VMATH_MATRIXG3_H_

#include <cmath>

#include "Vecmath.h"

namespace vmath {

	template <class T>
	class MatrixG3 : public Matrix3<T> {
	protected:

		using Matrix3<T>::_m;

	public:

		MatrixG3 ()
			: Matrix3<T>() {
		}

		MatrixG3 (T a00, T a01, T a02, T a10, T a11, T a12, T a20, T a21, T a22)
			: Matrix3<T>(a00, a01, a02, a10, a11, a12, a20, a21, a22) {
		}

		MatrixG3 (const T* p)
			: Matrix3<T>(p) {
		}

		MatrixG3 (const Matrix3<T>& m)
			: Matrix3<T>(m) {
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

			this->mul(M);
		}

		void rotateX (T a) {
			T rada = radians(a);
			T sina = std::sin(rada);
			T cosa = std::cos(rada);
			T tmp1, tmp2;

			tmp1 = _m[1] * cosa + _m[2] * sina;
			tmp2 = _m[1] * -sina + _m[2] * cosa;
			_m[1] = tmp1;	_m[2] = tmp2;

			tmp1 = _m[4] * cosa + _m[5] * sina;
			tmp2 = _m[4] * -sina + _m[5] * cosa;
			_m[4] = tmp1;	_m[5] = tmp2;

			tmp1 = _m[7] * cosa + _m[8] * sina;
			tmp2 = _m[7] * -sina + _m[8] * cosa;
			_m[7] = tmp1;	_m[8] = tmp2;
		}

		void rotateY (T a) {
			T rada = radians(a);
			T sina = std::sin(rada);
			T cosa = std::cos(rada);
			T tmp1, tmp2;

			tmp1 = _m[0] * cosa + _m[2] * -sina;
			tmp2 = _m[0] * sina + _m[2] * cosa;
			_m[0] = tmp1;	_m[2] = tmp2;

			tmp1 = _m[3] * cosa + _m[5] * -sina;
			tmp2 = _m[3] * sina + _m[5] * cosa;
			_m[3] = tmp1;	_m[5] = tmp2;

			tmp1 = _m[6] * cosa + _m[8] * -sina;
			tmp2 = _m[6] * sina + _m[8] * cosa;
			_m[6] = tmp1;	_m[8] = tmp2;
		}

		void rotateZ (T a) {
			T rada = radians(a);
			T sina = std::sin(rada);
			T cosa = std::cos(rada);
			T tmp1, tmp2;

			tmp1 = _m[0] * cosa + _m[1] * sina;
			tmp2 = _m[0] * -sina + _m[1] * cosa;
			_m[0] = tmp1;	_m[1] = tmp2;

			tmp1 = _m[3] * cosa + _m[4] * sina;
			tmp2 = _m[3] * -sina + _m[4] * cosa;
			_m[3] = tmp1;	_m[4] = tmp2;

			tmp1 = _m[6] * cosa + _m[7] * sina;
			tmp2 = _m[6] * -sina + _m[7] * cosa;
			_m[6] = tmp1;	_m[7] = tmp2;
		}

		void scale (T x, T y, T z) {
			_m[0] *= x;
			_m[3] *= x;
			_m[6] *= x;

			_m[1] *= y;
			_m[4] *= y;
			_m[7] *= y;

			_m[2] *= z;
			_m[5] *= z;
			_m[8] *= z;
		}

	protected:

		static T radians (T degrees) {
			//const double PI = 3.14159265;
			return (degrees * PI / 180.0);
		}
	};

	typedef MatrixG3<float> MatrixG3f;
	typedef MatrixG3<double> MatrixG3d;

}

#endif /* VMATH_MATRIXG3_H_ */
