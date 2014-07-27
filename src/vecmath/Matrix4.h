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

#ifndef VMATH_MATRIX4_H_
#define VMATH_MATRIX4_H_

#include <stdexcept>

#include "Vecmath.h"

namespace vmath {

	template <class T>
	class Matrix4 {
	public:

		class SingularException : public std::runtime_error {
		public:

			SingularException()
				: std::runtime_error("Singular Matrix Exception") {
			}
		};

		static const Matrix4<T> IDENTITY;

	protected:

		static const unsigned _nElements = 16;
		static const unsigned _nCols = 4;
		static const unsigned _nRows = 4;

		T _m[_nElements];

	public:

		Matrix4 () {
			for (unsigned int i = 0; i < _nElements; i++) {
				_m[i] = 0;
			}
		}

		Matrix4 (T a00, T a01, T a02, T a03, T a10, T a11, T a12, T a13, T a20, T a21, T a22, T a23, T a30, T a31, T a32, T a33) {
			_m[0] = a00;	_m[1] = a01;	_m[2] = a02;	_m[3] = a03;
			_m[4] = a10;	_m[5] = a11;	_m[6] = a12;	_m[7] = a13;
			_m[8] = a20;	_m[9] = a21;	_m[10] = a22;	_m[11] = a23;
			_m[12] = a30;	_m[13] = a31;	_m[14] = a32;	_m[15] = a33;
		}

		Matrix4 (const T* p) {
			for (unsigned int i = 0; i < _nElements; i++) {
				_m[i] = p[i];
			}
		}

		Matrix4 (const Matrix4<T>& m) {
			for (unsigned int i = 0; i < _nElements; i++) {
				_m[i] = m._m[i];
			}
		}

		Matrix4<T>& operator= (const Matrix4<T>& m) {
			set(m);
			return *this;
		}

		friend Matrix4<T> operator+<> (const Matrix4<T>& m1, const Matrix4<T>& m2);
		friend Matrix4<T> operator+<> (const Matrix4<T>& m, T s);

		Matrix4<T>& operator+= (const Matrix4<T>& m) {
			add(m);
			return *this;
		}

		Matrix4<T>& operator+= (T s) {
			add(s);
			return *this;
		}

		friend Matrix4<T> operator-<> (const Matrix4<T>& m1, const Matrix4<T>& m2);
		friend Matrix4<T> operator-<> (const Matrix4<T>& m, T s);

		Matrix4<T>& operator-= (const Matrix4<T>& m) {
			sub(m);
			return *this;
		}

		Matrix4<T>& operator-= (T s) {
			sub(s);
			return *this;
		}

		friend Matrix4<T> operator*<> (const Matrix4<T>& m1, const Matrix4<T>& m2);
		friend Matrix4<T> operator*<> (const Matrix4<T>& m, T s);
		friend Matrix4<T> operator*<> (T s, const Matrix4<T>& m);
		friend Vector4<T> operator*<> (const Matrix4<T>& m, const Vector4<T>& v);
		friend Vector4_T<T> operator*<> (const Vector4_T<T>& vt, const Matrix4<T>& m);

		Matrix4<T>& operator*= (const Matrix4<T>& m) {
			mul(m);
			return *this;
		}

		Matrix4<T>& operator*= (T s) {
			mul(s);
			return *this;
		}

		friend Matrix4<T> operator/<> (const Matrix4<T>& m, T s);

		Matrix4<T>& operator/= (T s) {
			mul(1.0 / s);
			return *this;
		}

		Matrix4<T> operator== (const Matrix4<T>& m) const {
			return std::equal(_m.begin(), _m.end(), m);
		}

		Matrix4<T> operator!= (const Matrix4<T>& m) const {
			return !(*this == m);
		}

		T& operator() (unsigned row, unsigned col) {
			if (row >= _nRows || col >= _nRows) {
				throw std::out_of_range("Subscript index out of range");
			}

			return _m[_nRows * row + col];
		}

		T operator() (unsigned row, unsigned col) const {
			if (row >= _nRows || col >= _nCols) {
				throw std::out_of_range("Subscript index out of range");
			}

			return _m[_nRows * row + col];
		}

		friend std::ostream& operator<< (std::ostream &os, const Matrix4<T>& m) {
			os << m.toString();
			return os;
		}

		void add (T s) {
			for (unsigned int i = 0; i < _nElements; i++) {
				_m[i] += s;
			}
		}

		void add (const Matrix4<T>& m) {
			for (unsigned int i = 0; i < _nElements; i++) {
				_m[i] += m._m[i];
			}
		}

		const T* asArray () const {
			return _m;
		}

		T determinant () const {
			T sub0 = _m[0] * det3x3(_m[5], _m[6], _m[7], _m[9], _m[10], _m[11], _m[13], _m[14], _m[15]);
			T sub1 = _m[1] * det3x3(_m[4], _m[6], _m[7], _m[8], _m[10], _m[11], _m[12], _m[14], _m[15]);
			T sub2 = _m[2] * det3x3(_m[4], _m[5], _m[7], _m[8], _m[9], _m[11], _m[12], _m[13], _m[15]);
			T sub3 = _m[3] * det3x3(_m[4], _m[5], _m[6], _m[8], _m[9], _m[10], _m[12], _m[13], _m[14]);

			return sub0 - sub1 + sub2 - sub3;
		}

		void getColumn (unsigned col, Vector4<T>& v) const {
			if (col >= _nCols) {
				throw std::out_of_range("Column out of range");
			}

			v.x = _m[_nRows * 0 + col];
			v.y = _m[_nRows * 1 + col];
			v.z = _m[_nRows * 2 + col];
			v.w = _m[_nRows * 3 + col];
		}

		Vector4<T> getColumn (unsigned col) const {
			Vector4<T> v;
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

		void getRow (unsigned row, Vector4<T>& v) const {
			if (row >= _nRows) {
				throw std::out_of_range("Row out of range");
			}

			v.x = _m[_nRows * row + 0];
			v.y = _m[_nRows * row + 1];
			v.z = _m[_nRows * row + 2];
			v.w = _m[_nRows * row + 3];
		}

		Vector4<T> getRow (unsigned row) const {
			Vector4<T> v;
			getRow(row, v);
			return v;
		}

		void invert () {
			T d00 = det3x3(_m[5], _m[6], _m[7], _m[9], _m[10], _m[11], _m[13], _m[14], _m[15]);
			T d01 = det3x3(_m[4], _m[6], _m[7], _m[8], _m[10], _m[11], _m[12], _m[14], _m[15]);
			T d02 = det3x3(_m[4], _m[5], _m[7], _m[8], _m[9], _m[11], _m[12], _m[13], _m[15]);
			T d03 = det3x3(_m[4], _m[5], _m[6], _m[8], _m[9], _m[10], _m[12], _m[13], _m[14]);

			T det = d00 - d01 + d02 - d03;
			if (det == 0) {
				throw SingularException();
			}

			T d10 = det3x3(_m[1], _m[2], _m[3], _m[9], _m[10], _m[11], _m[13], _m[14], _m[15]);
			T d11 = det3x3(_m[0], _m[2], _m[3], _m[8], _m[10], _m[11], _m[12], _m[14], _m[15]);
			T d12 = det3x3(_m[0], _m[1], _m[3], _m[8], _m[9], _m[11], _m[12], _m[13], _m[15]);
			T d13 = det3x3(_m[0], _m[1], _m[2], _m[8], _m[9], _m[10], _m[12], _m[13], _m[14]);

			T d20 = det3x3(_m[1], _m[2], _m[3], _m[5], _m[6], _m[7], _m[13], _m[14], _m[15]);
			T d21 = det3x3(_m[0], _m[2], _m[3], _m[4], _m[6], _m[7], _m[12], _m[14], _m[15]);
			T d22 = det3x3(_m[0], _m[1], _m[3], _m[4], _m[5], _m[7], _m[12], _m[13], _m[15]);
			T d23 = det3x3(_m[0], _m[1], _m[2], _m[4], _m[5], _m[6], _m[12], _m[13], _m[14]);

			T d30 = det3x3(_m[1], _m[2], _m[3], _m[5], _m[6], _m[7], _m[9], _m[10], _m[11]);
			T d31 = det3x3(_m[0], _m[2], _m[3], _m[4], _m[6], _m[7], _m[8], _m[10], _m[11]);
			T d32 = det3x3(_m[0], _m[1], _m[3], _m[4], _m[5], _m[7], _m[8], _m[9], _m[11]);
			T d33 = det3x3(_m[0], _m[1], _m[2], _m[4], _m[5], _m[6], _m[8], _m[9], _m[10]);

			setRow(0, d00, -d01, d02, -d03);
			setRow(1, -d10, d11, -d12, d13);
			setRow(2, d20, -d21, d22, -d23);
			setRow(3, -d30, d31, -d32, d33);

			mul(1.0 / det);
			transpose();
		}

		void mul (T s) {
			for (unsigned int i = 0; i < _nElements; i++) {
				_m[i] *= s;
			}
		}

		void mul (const Matrix4<T>& m) {
			T n00 = _m[0] * m._m[0] + _m[1] * m._m[4] + _m[2] * m._m[8] + _m[3] * m._m[12];
			T n01 = _m[0] * m._m[1] + _m[1] * m._m[5] + _m[2] * m._m[9] + _m[3] * m._m[13];
			T n02 = _m[0] * m._m[2] + _m[1] * m._m[6] + _m[2] * m._m[10] + _m[3] * m._m[14];
			T n03 = _m[0] * m._m[3] + _m[1] * m._m[7] + _m[2] * m._m[11] + _m[3] * m._m[15];
			T n10 = _m[4] * m._m[0] + _m[5] * m._m[4] + _m[6] * m._m[8] + _m[7] * m._m[12];
			T n11 = _m[4] * m._m[1] + _m[5] * m._m[5] + _m[6] * m._m[9] + _m[7] * m._m[13];
			T n12 = _m[4] * m._m[2] + _m[5] * m._m[6] + _m[6] * m._m[10] + _m[7] * m._m[14];
			T n13 = _m[4] * m._m[3] + _m[5] * m._m[7] + _m[6] * m._m[11] + _m[7] * m._m[15];
			T n20 = _m[8] * m._m[0] + _m[9] * m._m[4] + _m[10] * m._m[8] + _m[11] * m._m[12];
			T n21 = _m[8] * m._m[1] + _m[9] * m._m[5] + _m[10] * m._m[9] + _m[11] * m._m[13];
			T n22 = _m[8] * m._m[2] + _m[9] * m._m[6] + _m[10] * m._m[10] + _m[11] * m._m[14];
			T n23 = _m[8] * m._m[3] + _m[9] * m._m[7] + _m[10] * m._m[11] + _m[11] * m._m[15];
			T n30 = _m[12] * m._m[0] + _m[13] * m._m[4] + _m[14] * m._m[8] + _m[15] * m._m[12];
			T n31 = _m[12] * m._m[1] + _m[13] * m._m[5] + _m[14] * m._m[9] + _m[15] * m._m[13];
			T n32 = _m[12] * m._m[2] + _m[13] * m._m[6] + _m[14] * m._m[10] + _m[15] * m._m[14];
			T n33 = _m[12] * m._m[3] + _m[13] * m._m[7] + _m[14] * m._m[11] + _m[15] * m._m[15];

			setRow(0, n00, n01, n02, n03);
			setRow(1, n10, n11, n12, n13);
			setRow(2, n20, n21, n22, n23);
			setRow(3, n30, n31, n32, n33);
		}

		Vector4<T> mul (const Vector4<T>& v) {
			Vector4<T> out(v);
			transform(out);
			return out;
		}

		void negate () {
			mul(-1.f);
		}

		void set (T s) {
			_m[0] = s;		_m[1] = 0;		_m[2] = 0;		_m[3] = 0;
			_m[4] = 0;		_m[5] = s;		_m[6] = 0;		_m[7] = 0;
			_m[8] = 0;		_m[9] = 0;		_m[10] = s;		_m[11] = 0;
			_m[12] = 0;		_m[13] = 0;		_m[14] = 0;		_m[15] = s;
		}

		void set (T a00, T a01, T a02, T a03, T a10, T a11, T a12, T a13, T a20, T a21, T a22, T a23, T a30, T a31, T a32, T a33) {
			_m[0] = a00;	_m[1] = a01;	_m[2] = a02;	_m[3] = a03;
			_m[4] = a10;	_m[5] = a11;	_m[6] = a12;	_m[7] = a13;
			_m[8] = a20;	_m[9] = a21;	_m[10] = a22;	_m[11] = a23;
			_m[12] = a30;	_m[13] = a31;	_m[14] = a32;	_m[15] = a33;
		}

		void set (const T* p) {
			for (unsigned int i = 0; i < _nElements; i++) {
				_m[i] += p[i];
			}
		}

		void set (const Matrix4<T>& m) {
			for (unsigned int i = 0; i < _nElements; i++) {
				_m[i] = m._m[i];
			}
		}

		void setColumn (unsigned col, T x, T y, T z, T w) {
			if (col >= _nCols) {
				throw std::out_of_range("Column out of range");
			}

			_m[_nRows * 0 + col] = x;
			_m[_nRows * 1 + col] = y;
			_m[_nRows * 2 + col] = z;
			_m[_nRows * 3 + col] = w;
		}

		void setColumn (unsigned col, const Vector4<T>& v) {
			setColumn(col, v.x, v.y, v.z, v.w);
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

		void setRow (unsigned row, T x, T y, T z, T w) {
			if (row >= _nRows) {
				throw std::out_of_range("Row out of range");
			}

			_m[_nRows * row + 0] = x;
			_m[_nRows * row + 1] = y;
			_m[_nRows * row + 2] = z;
			_m[_nRows * row + 3] = w;
		}

		void setRow (unsigned row, const Vector4<T>& v) {
			setRow(row, v.x, v.y, v.z, v.w);
		}

		void setZero () {
			set(0.f);
		}

		void sub (T s) {
			add(-s);
		}

		void sub(const Matrix4<T>& m) {
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

		void transform (Tuple4<T>& t) const {
			T tx = _m[0] * t.x + _m[1] * t.y + _m[2] * t.z + _m[3] * t.w;
			T ty = _m[4] * t.x + _m[5] * t.y + _m[6] * t.z + _m[7] * t.w;
			T tz = _m[8] * t.x + _m[9] * t.y + _m[10] * t.z + _m[11] * t.w;
			T tw = _m[12] * t.x + _m[13] * t.y + _m[14] * t.z + _m[15] * t.w;

			t.set(tx, ty, tz, tw);
		}

		void transpose () {
			swap(_m[1], _m[4]);
			swap(_m[2], _m[8]);
			swap(_m[3], _m[12]);
			swap(_m[6], _m[9]);
			swap(_m[7], _m[13]);
			swap(_m[11], _m[14]);
		}

	protected:

		T det2x2 (T a, T b, T c, T d) const {
			return (a * d) - (b * c);
		}

		T det3x3 (T a, T b, T c, T d, T e, T f, T g, T h, T i) const {
			T sub0 = a * det2x2(e, f, h, i);
			T sub1 = b * det2x2(d, f, g, i);
			T sub2 = c * det2x2(d, e, g, h);

			return sub0 - sub1 + sub2;
		}

		void swap (T& s0, T& s1) {
			T s2 = s0;
			s0 = s1;
			s1 = s2;
		}
	};

	template <typename T>
	const Matrix4<T> Matrix4<T>::IDENTITY(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

	typedef Matrix4<double> Matrix4d;
	typedef Matrix4<float> Matrix4f;

}

#endif /* VMATH_MATRIX4_H_ */
