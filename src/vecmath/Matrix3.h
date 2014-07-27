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

#ifndef VMATH_MATRIX3_H_
#define VMATH_MATRIX3_H_

#include <stdexcept>

#include "Vecmath.h"

namespace vmath {

	template <class T>
	class Matrix3 {
	public:

		class SingularException : public std::runtime_error {
		public:

			SingularException()
				: std::runtime_error("Singular Matrix Exception") {
			}
		};

		static const Matrix3<T> IDENTITY;

	protected:

		static const unsigned _nElements = 9;
		static const unsigned _nCols = 3;
		static const unsigned _nRows = 3;

		T _m[_nElements];

	public:

		Matrix3 () {
			for (unsigned int i = 0; i < _nElements; i++) {
				_m[i] = 0;
			}
		}

		Matrix3 (T a00, T a01, T a02, T a10, T a11, T a12, T a20, T a21, T a22) {
			_m[0] = a00;	_m[1] = a01;	_m[2] = a02;
			_m[3] = a10;	_m[4] = a11;	_m[5] = a12;
			_m[6] = a20;	_m[7] = a21;	_m[8] = a22;
		}

		Matrix3 (const T* p) {
			for (unsigned int i = 0; i < _nElements; i++) {
				_m[i] = p[i];
			}
		}

		Matrix3 (const Matrix3<T>& m) {
			for (unsigned int i = 0; i < _nElements; i++) {
				_m[i] = m._m[i];
			}
		}

		Matrix3<T>& operator= (const Matrix3<T>& m) {
			set(m);
			return *this;
		}

		friend Matrix3<T> operator+<> (const Matrix3<T>& m1, const Matrix3<T>& m2);
		friend Matrix3<T> operator+<> (const Matrix3<T>& m, T s);

		Matrix3<T>& operator+= (const Matrix3<T>& m) {
			add(m);
			return *this;
		}

		Matrix3<T>& operator+= (T s) {
			add(s);
			return *this;
		}

		friend Matrix3<T> operator-<> (const Matrix3<T>& m1, const Matrix3<T>& m2);
		friend Matrix3<T> operator-<> (const Matrix3<T>& m, T s);

		Matrix3<T>& operator-= (const Matrix3<T>& m) {
			sub(m);
			return *this;
		}

		Matrix3<T>& operator-= (T s) {
			sub(s);
			return *this;
		}

		friend Matrix3<T> operator*<> (const Matrix3<T>& m1, const Matrix3<T>& m2);
		friend Matrix3<T> operator*<> (const Matrix3<T>& m, T s);
		friend Matrix3<T> operator*<> (T s, const Matrix3<T>& m);
		friend Vector3<T> operator*<> (const Matrix3<T>& m, const Vector3<T>& v);
		friend Vector3_T<T> operator*<> (const Vector3_T<T>& vt, const Matrix3<T>& m);

		Matrix3<T>& operator*= (const Matrix3<T>& m) {
			mul(m);
			return *this;
		}

		Matrix3<T>& operator*= (T s) {
			mul(s);
			return *this;
		}

		friend Matrix3<T> operator/<> (const Matrix3<T>& m, T s);

		Matrix3<T>& operator/= (T s) {
			mul(1.0 / s);
			return *this;
		}

		Matrix3<T> operator== (const Matrix3<T>& m) const {
			return std::equal(_m.begin(), _m.end(), m);
		}

		Matrix3<T> operator!= (const Matrix3<T>& m) const {
			return !(*this == m);
		}

		T& operator() (unsigned row, unsigned col) {
			if (row >= _nRows || col >= _nCols) {
				throw std::out_of_range("Subscript index out of range");
			}

			return _m[_nRows * row + col];
		}

		T operator() (unsigned row, unsigned col) const {
			if (row >= _nRows || col > _nCols) {
				throw std::out_of_range("Subscript index out of range");
			}

			return _m[_nRows * row + col];
		}

		friend std::ostream& operator<< (std::ostream &os, const Matrix3<T>& m) {
			os << m.toString();
			return os;
		}

		void add (T s) {
			for (unsigned int i = 0; i < _nElements; i++) {
				_m[i] += s;
			}
		}

		void add (const Matrix3<T>& m) {
			for (unsigned int i = 0; i < _nElements; i++) {
				_m[i] += m._m[i];
			}
		}

		const T* asArray () const {
			return _m;
		}

		T determinant () const {
			T sub0 = _m[0] * det2x2(_m[4], _m[5], _m[7], _m[8]);
			T sub1 = _m[1] * det2x2(_m[3], _m[5], _m[6], _m[8]);
			T sub2 = _m[2] * det2x2(_m[3], _m[4], _m[6], _m[7]);

			return sub0 - sub1 + sub2;
		}

		void getColumn (unsigned col, Vector3<T>& v) const {
			if (col >= _nCols) {
				throw std::out_of_range("Column out of range");
			}

			v.x = _m[_nRows * 0 + col];
			v.y = _m[_nRows * 1 + col];
			v.z = _m[_nRows * 2 + col];
		}

		Vector3<T> getColumn (unsigned col) const {
			Vector3<T> v;
			getColumn(col, v);
			return v;
		}

		T getElement (unsigned index) const {
			if (index >= _nElements) {
				throw std::out_of_range("Element index out of range");
			}

			return _m[index];
		}

		T getElement (unsigned row, unsigned col) const {
			if (row >= _nRows || col >= _nCols) {
				throw std::out_of_range("Element index out of range");
			}

			return _m[_nRows * row + col];
		}

		void getRow (unsigned row, Vector3<T>& v) const {
			if (row >= _nRows) {
				throw std::out_of_range("Row out of range");
			}

			v.x = _m[_nRows * row + 0];
			v.y = _m[_nRows * row + 1];
			v.z = _m[_nRows * row + 2];
		}

		Vector3<T> getRow (unsigned row) const {
			Vector3<T> v;
			getRow(row, v);
			return v;
		}

		void invert () {
			T d00 = det2x2(_m[4], _m[5], _m[7], _m[8]);
			T d01 = det2x2(_m[3], _m[5], _m[6], _m[8]);
			T d02 = det2x2(_m[3], _m[4], _m[6], _m[7]);

			T det = d00 - d01 + d02;
			if (det == 0) {
				throw SingularException();
			}

			T d10 = det2x2(_m[1], _m[2], _m[7], _m[8]);
			T d11 = det2x2(_m[0], _m[2], _m[6], _m[8]);
			T d12 = det2x2(_m[0], _m[1], _m[6], _m[7]);

			T d20 = det2x2(_m[1], _m[2], _m[4], _m[5]);
			T d21 = det2x2(_m[0], _m[2], _m[3], _m[5]);
			T d22 = det2x2(_m[0], _m[1], _m[3], _m[4]);

			setRow(0, d00, -d01, d02);
			setRow(1, -d10, d11, -d12);
			setRow(2, d20, -d21, d22);

			mul(1.0 / det);
			transpose();
		}

		void mul (T s) {
			for (unsigned int i = 0; i < _nElements; i++) {
				_m[i] *= s;
			}
		}

		void mul (const Matrix3<T>& m) {
			T n00 = _m[0] * m._m[0] + _m[1] * m._m[3] + _m[2] * m._m[6];
			T n01 = _m[0] * m._m[1] + _m[1] * m._m[4] + _m[2] * m._m[7];
			T n02 = _m[0] * m._m[2] + _m[1] * m._m[5] + _m[2] * m._m[8];
			T n10 = _m[3] * m._m[0] + _m[4] * m._m[3] + _m[5] * m._m[6];
			T n11 = _m[3] * m._m[1] + _m[4] * m._m[4] + _m[5] * m._m[7];
			T n12 = _m[3] * m._m[2] + _m[4] * m._m[5] + _m[5] * m._m[8];
			T n20 = _m[6] * m._m[0] + _m[7] * m._m[3] + _m[8] * m._m[6];
			T n21 = _m[6] * m._m[1] + _m[7] * m._m[4] + _m[8] * m._m[7];
			T n22 = _m[6] * m._m[2] + _m[7] * m._m[5] + _m[8] * m._m[8];

			setRow(0, n00, n01, n02);
			setRow(1, n10, n11, n12);
			setRow(2, n20, n21, n22);
		}

		Vector3<T> mul (const Vector3<T>& v) {
			Vector3<T> out(v);
			transform(out);
			return out;
		}

		void negate () {
			mul(-1.f);
		}

		void set (T s) {
			_m[0] = s;	_m[1] = 0;	_m[2] = 0;
			_m[3] = 0;	_m[4] = s;	_m[5] = 0;
			_m[6] = 0;	_m[7] = 0;	_m[8] = s;
		}

		void set (T a00, T a01, T a02, T a10, T a11, T a12, T a20, T a21, T a22) {
			_m[0] = a00;	_m[1] = a01;	_m[2] = a02;
			_m[3] = a10;	_m[4] = a11;	_m[5] = a12;
			_m[6] = a20;	_m[7] = a21;	_m[8] = a22;
		}

		void set (const T* p) {
			for (unsigned int i = 0; i < _nElements; i++) {
				_m[i] += p[i];
			}
		}

		void set (const Matrix3<T>& m) {
			for (unsigned int i = 0; i < _nElements; i++) {
				_m[i] = m._m[i];
			}
		}

		void setColumn (unsigned col, T x, T y, T z) {
			if (col >= _nCols) {
				throw std::out_of_range("Column out of range");
			}

			_m[_nRows * 0 + col] = x;
			_m[_nRows * 1 + col] = y;
			_m[_nRows * 2 + col] = z;
		}

		void setColumn (unsigned col, const Vector3<T>& v) {
			setColumn(col, v.x, v.y, v.z);
		}

		void setElement (unsigned index, T s) {
			if (index >= _nElements) {
				throw std::out_of_range("Element index out of range");
			}

			_m[index] = s;
		}

		void setElement (unsigned row, unsigned col, T s) {
			if (row >= _nRows || col >= _nCols) {
				throw std::out_of_range("Element index out of range");
			}

			_m[_nRows * row + col] = s;
		}

		void setIdentity () {
			set(1.f);
		}

		void setRow (unsigned row, T x, T y, T z) {
			if (row >= _nRows) {
				throw std::out_of_range("Row out of range");
			}

			_m[_nRows * row + 0] = x;
			_m[_nRows * row + 1] = y;
			_m[_nRows * row + 2] = z;
		}

		void setRow (unsigned row, const Vector3<T>& v) {
			setRow(row, v.x, v.y, v.z);
		}

		void setZero () {
			set(0.f);
		}

		void sub (T s) {
			add(-s);
		}

		void sub(const Matrix3<T>& m) {
			for (unsigned int i = 0; i < _nElements; i++) {
				_m[i] -= m._m[i];
			}
		}

		std::string toString () const {
			std::stringstream str;
			str.precision(3);

			str << "(";
			for (unsigned i = 0; i < _nElements - 1; i++) {
				str << std::setw(8) << std::fixed << _m[i] << ",";
			}
			str << std::setw(8) << std::fixed << _m[_nElements - 1];
			str << ")";

			return str.str();
		}

		void transform (Vector3<T>& t) const {
			T tx = _m[0] * t.x + _m[1] * t.y + _m[2] * t.z;
			T ty = _m[3] * t.x + _m[4] * t.y + _m[5] * t.z;
			T tz = _m[6] * t.x + _m[7] * t.y + _m[8] * t.z;

			t.set(tx, ty, tz);
		}

		void transpose () {
			swap(_m[1], _m[3]);
			swap(_m[2], _m[6]);
			swap(_m[5], _m[7]);
		}

	protected:

		T det2x2 (T a, T b, T c, T d) const {
			return (a * d) - (b * c);
		}


		void swap (T& s0, T& s1) {
			T s2 = s0;
			s0 = s1;
			s1 = s2;
		}
	};

	template <typename T>
	const Matrix3<T> Matrix3<T>::IDENTITY(1, 0, 0, 0, 1, 0, 0, 0, 1);

	typedef Matrix3<double> Matrix3d;
	typedef Matrix3<float> Matrix3f;

}

#endif /* VMATH_MATRIX3_H_ */
