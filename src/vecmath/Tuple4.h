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

#ifndef VMATH_TUPLE4_H_
#define VMATH_TUPLE4_H_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "Vecmath.h"

namespace vmath {

	template <class T>
	class Tuple4 {
	protected:

		static const unsigned _nElements = 4;

		typedef T Tuple4<T>::* const TRef[4];
		static const TRef _t;

	public:

		T x;
		T y;
		T z;
		T w;

	public:

		Tuple4 ()
			: x(0), y(0), z(0), w(0) {
		}

		Tuple4 (T s)
			: x(s), y(s), z(s), w(s) {
		}

		Tuple4 (T fx, T fy, T fz, T fw)
			: x(fx), y(fy), z(fz), w(fw) {
		}

		Tuple4 (const Tuple3<T>& t, T fw)
			: x(t.x), y(t.y), z(t.z), w(fw) {
		}

		Tuple4 (const Tuple4<T>& t)
			: x(t.x), y(t.y), z(t.z), w(t.w) {
		}

		Tuple4 (const Tuple4<T>* t)
			: x(t->x), y(t->y), z(t->z), w(t->w) {
		}

		Tuple4 (const T* p)
			: x(p[0]), y(p[1]), z(p[2]), w(p[3]) {
		}

		//virtual ~Tuple4 () { }

		Tuple4<T>& operator= (const Tuple4<T>& t) {
			x = t.x;
			y = t.y;
			z = t.z;
			w = t.w;
			return *this;
		}

		friend Tuple4<T> operator+<> (const Tuple4<T>& t1, const Tuple4<T>& t2);
		friend Tuple4<T> operator+<> (const Tuple4<T>& t, T s);
		friend Tuple4<T> operator+<> (T s, const Tuple4<T>& t);

		void operator+= (const Tuple4<T>& t) {
			add(t);
		}

		void operator+= (T s) {
			add(s);
		}

		friend Tuple4<T> operator-<> (const Tuple4<T>& t1, const Tuple4<T>& t2);
		friend Tuple4<T> operator-<> (const Tuple4<T>& t, T s);
		friend Tuple4<T> operator-<> (T s, const Tuple4<T>& t);

		friend Tuple4<T> operator-<> (const Tuple4<T>& t);

		void operator-= (const Tuple4<T>& t) {
			sub(t);
		}

		void operator-= (T s) {
			sub(s);
		}

		friend Tuple4<T> operator*<> (const Tuple4<T>& t, T s);
		friend Tuple4<T> operator*<> (T s, const Tuple4<T>& t);

		void operator*= (T s) {
			scale(s);
		}

		friend Tuple4<T> operator/<> (const Tuple4<T>& t, T s);

		void operator/= (T s) {
			scale(1.0 / s);
		}

		bool operator== (const Tuple4<T>& t) const {
			return (x == t.x && y == t.y && z == t.z && w = t.w);
		}

		bool operator== (T s) const {
			return ((x * x + y * y + z * z + w * w) == s);
		}

		bool operator!= (const Tuple4<T>& t) const {
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

		friend std::ostream& operator<< (std::ostream &os, const Tuple4<T>& t) {
			os << t.toString();
			return os;
		}

		void absolute () {
			if (x < 0) x = 0 - x;
			if (y < 0) y = 0 - y;
			if (z < 0) z = 0 - z;
			if (w < 0) w = 0 - w;
		}

		void add (T s) {
			x += s;
			y += s;
			z += s;
			w += s;
		}

		void add (const Tuple4<T>& t) {
			x += t.x;
			y += t.y;
			z += t.z;
			w += t.w;
		}

		void clamp (T min, T max) {
			clampMin(min);
			clampMax(max);
		}

		void clampMax (T max) {
			if (x > max) x = max;
			if (y > max) y = max;
			if (z > max) z = max;
			if (w > max) w = max;
		}

		void clampMin (T min) {
			if (x < min) x = min;
			if (y < min) y = min;
			if (z < min) z = min;
			if (w < min) w = min;
		}

		bool equals (const Tuple4<T>& t) const {
			return (x == t.x && y == t.y && z == t.z && w = t.w);
		}

		bool epsilonEquals (const Tuple4<T>& t, T epsilon) const {
			return (x * x + y * y + z * z + w * w) <= (epsilon * epsilon);
		}

		void get (T* p) const {
			p[0] = x;
			p[1] = y;
			p[2] = z;
			p[3] = w;
		}

		T getElement (unsigned index) const {
			if (index >= _nElements) {
				throw std::out_of_range("Subscript index out of range");
			}

			return this->*_t[index];
		}

		void interpolate (const Tuple4<T>& t, float alpha) {
			x = (1.f - alpha) * x + alpha * t.x;
			y = (1.f - alpha) * y + alpha * t.y;
			z = (1.f - alpha) * z + alpha * t.z;
			w = (1.f - alpha) * w + alpha * t.w;
		}

		void negate () {
			scale(-1.f);
		}

		void scale (T s) {
			x *= s;
			y *= s;
			z *= s;
			w *= s;
		}

		void scaleAdd (T s, T a) {
			x = x * s + a;
			y = y * s + a;
			z = z * s + a;
			w = w * s + a;
		}

		void scaleAdd (T s, const Tuple4<T>& t) {
			x = x * s + t.x;
			y = y * s + t.y;
			z = z * s + t.z;
			w = w * s + t.w;
		}

		void set (T s) {
			x = s;
			y = s;
			z = s;
			w = s;
		}

		void set (T fx, T fy, T fz, T fw) {
			x = fx;
			y = fy;
			z = fz;
			w = fw;
		}

		void set (const Tuple4<T>& t) {
			x = t.x;
			y = t.y;
			z = t.z;
			w = t.w;
		}

		void set (const T* p) {
			x = p[0];
			y = p[1];
			z = p[2];
			w = p[3];
		}

		void setElement (unsigned index, T s){
			if (index >= _nElements) {
				throw std::out_of_range("Subscript index out of range");
			}

			this->*_t[index] = s;
		}

		void sub (T s) {
			x -= s;
			y -= s;
			z -= s;
			w -= s;
		}

		void sub (const Tuple4<T>& t) {
			x -= t.x;
			y -= t.y;
			z -= t.z;
			w -= t.w;
		}

		std::string toString () const {
			std::stringstream str;
			str.precision(3);

			str << "(";
			str << std::setw(8) << std::fixed << x << ",";
			str << std::setw(8) << std::fixed << y << ",";
			str << std::setw(8) << std::fixed << z << ",";
			str << std::setw(8) << std::fixed << w;
			str << ")";

			return str.str();
		}
	};

	template <typename T>
	const typename Tuple4<T>::TRef Tuple4<T>::_t = {
		&Tuple4<T>::x,
		&Tuple4<T>::y,
		&Tuple4<T>::z,
		&Tuple4<T>::w
	};

	typedef Tuple4<unsigned char> Tuple4b;
	typedef Tuple4<double> Tuple4d;
	typedef Tuple4<float> Tuple4f;
	typedef Tuple4<int> Tuple4i;

}

#endif /* VMATH_TUPLE4_H_ */
