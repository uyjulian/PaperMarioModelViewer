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

#ifndef VMATH_TUPLE3_H_
#define VMATH_TUPLE3_H_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "Vecmath.h"

namespace vmath {

	template <class T>
	class Tuple3 {
	protected:

		static const unsigned _nElements = 3;

		typedef T Tuple3<T>::* const TRef[3];
		static const TRef _t;

	public:

		T x;
		T y;
		T z;

	public:

		Tuple3 ()
			: x(0), y(0), z(0) {
		}

		Tuple3 (T s)
			: x(s), y(s), z(s) {
		}

		Tuple3 (T fx, T fy, T fz)
			: x(fx), y(fy), z(fz) {
		}

		Tuple3 (const Tuple3<T>& t)
			: x(t.x), y(t.y), z(t.z) {
		}

		Tuple3 (const Tuple3<T>* t)
			: x(t->x), y(t->y), z(t->z) {
		}

		Tuple3 (const T* p)
			: x(p[0]), y(p[1]), z(p[2]) {
		}

		//virtual ~Tuple3 () { }

		Tuple3<T>& operator= (const Tuple3<T>& t) {
			x = t.x;
			y = t.y;
			z = t.z;
			return *this;
		}

		friend Tuple3<T> operator+<> (const Tuple3<T>& t1, const Tuple3<T>& t2);
		friend Tuple3<T> operator+<> (const Tuple3<T>& t, T s);
		friend Tuple3<T> operator+<> (T s, const Tuple3<T>& t);

		void operator+= (const Tuple3<T>& t) {
			add(t);
		}

		void operator+= (T s) {
			add(s);
		}

		friend Tuple3<T> operator-<> (const Tuple3<T>& t1, const Tuple3<T>& t2);
		friend Tuple3<T> operator-<> (const Tuple3<T>& t, T s);
		friend Tuple3<T> operator-<> (T s, const Tuple3<T>& t);

		friend Tuple3<T> operator-<> (const Tuple3<T>& t);

		void operator-= (const Tuple3<T>& t) {
			sub(t);
		}

		void operator-= (T s) {
			sub(s);
		}

		friend Tuple3<T> operator*<> (const Tuple3<T>& t, T s);
		friend Tuple3<T> operator*<> (T s, const Tuple3<T>& t);

		void operator*= (T s) {
			scale(s);
		}

		friend Tuple3<T> operator/<> (const Tuple3<T>& t, T s);

		void operator/= (T s) {
			scale(1.0 / s);
		}

		bool operator== (const Tuple3<T>& t) const {
			return (x == t.x && y == t.y && z == t.z);
		}

		bool operator== (T s) const {
			return ((x * x + y * y + z * z) == s);
		}

		bool operator!= (const Tuple3<T>& t) const {
			return !(*this == t);
		}

		bool operator!= (T s) const {
			return !(*this == s);
		}

		T& operator[] (unsigned index) {
			return this->*_t[index];
		}

		const T& operator[] (unsigned index) const {
			return this->*_t[index];
		}

		T& operator() (unsigned index) {
			if (index >= _nElements) {
				throw std::out_of_range("Subscript index out of range");
			}

			return this->*_t[index];
		}

		const T& operator() (unsigned index) const {
			if (index >= _nElements) {
				throw std::out_of_range("Subscript index out of range");
			}

			return this->*_t[index];
		}

		friend std::ostream& operator<< (std::ostream &os, const Tuple3<T>& t) {
			os << t.toString();
			return os;
		}

		void absolute () {
			if (x < 0) x = 0 - x;
			if (y < 0) y = 0 - y;
			if (z < 0) z = 0 - z;
		}

		void add (T s) {
			x += s;
			y += s;
			z += s;
		}

		void add (const Tuple3<T>& t) {
			x += t.x;
			y += t.y;
			z += t.z;
		}

		void clamp (T min, T max) {
			clampMin(min);
			clampMax(max);
		}

		void clampMax (T max) {
			if (x > max) x = max;
			if (y > max) y = max;
			if (z > max) z = max;
		}

		void clampMin (T min) {
			if (x < min) x = min;
			if (y < min) y = min;
			if (z < min) z = min;
		}

		bool equals (const Tuple3<T>& t) const {
			return (x == t.x && y == t.y && z == t.z);
		}

		bool epsilonEquals (const Tuple3<T>& t, T epsilon) const {
			return (x * x + y * y + z * z) <= (epsilon * epsilon);
		}

		void get (T* p) const {
			p[0] = x;
			p[1] = y;
			p[2] = z;
		}

		T getElement (unsigned index) const {
			if (index >= _nElements) {
				throw std::out_of_range("Subscript index out of range");
			}

			return this->*_t[index];
		}

		void interpolate (const Tuple3<T>& t, float alpha) {
			x = (1.f - alpha) * x + alpha * t.x;
			y = (1.f - alpha) * y + alpha * t.y;
			z = (1.f - alpha) * z + alpha * t.z;
		}

		void negate () {
			scale(-1.f);
		}

		void scale (T s) {
			x *= s;
			y *= s;
			z *= s;
		}

		void scaleAdd (T s, T a) {
			x = x * s + a;
			y = y * s + a;
			z = z * s + a;
		}

		void scaleAdd (T s, const Tuple3<T>& t) {
			x = x * s + t.x;
			y = y * s + t.y;
			z = z * s + t.z;
		}

		void set (T s) {
			x = s;
			y = s;
			z = s;
		}

		void set (T fx, T fy, T fz) {
			x = fx;
			y = fy;
			z = fz;
		}

		void set (const Tuple3<T>& t) {
			x = t.x;
			y = t.y;
			z = t.z;
		}

		void set (const T* p) {
			x = p[0];
			y = p[1];
			z = p[2];
		}

		void setElement (unsigned index, T s) {
			if (index >= _nElements) {
				throw std::out_of_range("Subscript index out of range");
			}

			this->*_t[index] = s;
		}

		void sub (T s) {
			x -= s;
			y -= s;
			z -= s;
		}

		void sub (const Tuple3<T>& t) {
			x -= t.x;
			y -= t.y;
			z -= t.z;
		}

		std::string toString () const {
			std::stringstream str;
			str.precision(3);

			str << "(";
			str << std::setw(8) << std::fixed << x << ",";
			str << std::setw(8) << std::fixed << y << ",";
			str << std::setw(8) << std::fixed << z;
			str << ")";

			return str.str();
		}
	};

	template <typename T>
	const typename Tuple3<T>::TRef Tuple3<T>::_t = {
		&Tuple3<T>::x,
		&Tuple3<T>::y,
		&Tuple3<T>::z
	};

	typedef Tuple3<unsigned char> Tuple3b;
	typedef Tuple3<double> Tuple3d;
	typedef Tuple3<float> Tuple3f;
	typedef Tuple3<int> Tuple3i;

}

#endif /* VMATH_TUPLE3_H_ */
