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

#ifndef VMATH_TUPLE2_H_
#define VMATH_TUPLE2_H_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "Vecmath.h"

namespace vmath {

	template <class T>
	class Tuple2 {

	protected:

		static const unsigned _nElements = 2;

		typedef T Tuple2<T>::* const TRef[2];
		static const TRef _t;

	public:

		T x;
		T y;

	public:

		Tuple2 ()
			: x(0), y(0) {
		}

		Tuple2 (T s)
			: x(s), y(s) {
		}

		Tuple2 (T fx, T fy)
			: x(fx), y(fy) {
		}

		Tuple2 (const Tuple2<T>& t)
			: x(t.x), y(t.y) {
		}

		Tuple2 (const Tuple2<T>* t)
			: x(t->x), y(t->y) {
		}

		Tuple2 (const T* p)
			: x(p[0]), y(p[1]) {
		}

		//virtual ~Tuple2 () { }

		Tuple2<T>& operator= (const Tuple2<T>& t) {
			x = t.x;
			y = t.y;
			return *this;
		}

		friend Tuple2<T> operator+<> (const Tuple2<T>& t1, const Tuple2<T>& t2);
		friend Tuple2<T> operator+<> (const Tuple2<T>& t, T s);
		friend Tuple2<T> operator+<> (T s, const Tuple2<T>& t);

		void operator+= (const Tuple2<T>& t) {
			add(t);
		}

		void operator+= (T s) {
			add(s);
		}

		friend Tuple2<T> operator-<> (const Tuple2<T>& t1, const Tuple2<T>& t2);
		friend Tuple2<T> operator-<> (const Tuple2<T>& t, T s);
		friend Tuple2<T> operator-<> (T s, const Tuple2<T>& t);

		friend Tuple2<T> operator-<> (const Tuple2<T>& t);

		void operator-= (const Tuple2<T>& t) {
			sub(t);
		}

		void operator-= (T s) {
			sub(s);
		}

		friend Tuple2<T> operator*<> (const Tuple2<T>& t, T s);
		friend Tuple2<T> operator*<> (T s, const Tuple2<T>& t);

		void operator*= (T s) {
			scale(s);
		}

		friend Tuple2<T> operator/<> (const Tuple2<T>& t, T s);

		void operator/= (T s) {
			scale(1.0 / s);
		}

		bool operator== (const Tuple2<T>& t) const {
			return (x == t.x && y == t.y);
		}

		bool operator== (T s) const {
			return ((x * x + y * y) == s);
		}

		bool operator!= (const Tuple2<T>& t) const {
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

		friend std::ostream& operator<< (std::ostream &os, const Tuple2<T>& t) {
			os << t.toString();
			return os;
		}

		void absolute () {
			if (x < 0) x = 0 - x;
			if (y < 0) y = 0 - y;
		}

		void add (T s) {
			x += s;
			y += s;
		}

		void add (const Tuple2<T>& t) {
			x += t.x;
			y += t.y;
		}

		void clamp (T min, T max) {
			clampMin(min);
			clampMax(max);
		}

		void clampMax (T max) {
			if (x > max) x = max;
			if (y > max) y = max;
		}

		void clampMin (T min) {
			if (x < min) x = min;
			if (y < min) y = min;
		}

		bool equals (const Tuple2<T>& t) const {
			return (x == t.x && y == t.y);
		}

		bool epsilonEquals (const Tuple2<T>& t, T epsilon) const {
			return (x * x + y * y) <= (epsilon * epsilon);
		}

		void get (T* p) const {
			p[0] = x;
			p[1] = y;
		}

		T getElement (unsigned index) const {
			if (index >= _nElements) {
				throw std::out_of_range("Subscript index out of range");
			}

			return this->*_t[index];
		}

		void interpolate (const Tuple2<T>& t, float alpha) {
			x = (1.f - alpha) * x + alpha * t.x;
			y = (1.f - alpha) * y + alpha * t.y;
		}

		void negate () {
			scale(-1.f);
		}

		void scale (T s) {
			x *= s;
			y *= s;
		}

		void scaleAdd (T s, T a) {
			x = x * s + a;
			y = y * s + a;
		}

		void scaleAdd (T s, const Tuple2<T>& t) {
			x = x * s + t.x;
			y = y * s + t.y;
		}

		void set (T s) {
			x = s;
			y = s;
		}

		void set (T fx, T fy) {
			x = fx;
			y = fy;
		}

		void set (const Tuple2<T>& t) {
			x = t.x;
			y = t.y;
		}

		void set (const T* p) {
			x = p[0];
			y = p[1];
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
		}

		void sub (const Tuple2<T>& t) {
			x -= t.x;
			y -= t.y;
		}

		std::string toString () const {
			std::stringstream str;
			str.precision(3);

			str << "(";
			str << std::setw(8) << std::fixed << x << ",";
			str << std::setw(8) << std::fixed << y;
			str << ")";

			return str.str();
		}
	};

	template <typename T>
	const typename Tuple2<T>::TRef Tuple2<T>::_t = {
		&Tuple2<T>::x,
		&Tuple2<T>::y
	};

	typedef Tuple2<unsigned char> Tuple2b;
	typedef Tuple2<double> Tuple2d;
	typedef Tuple2<float> Tuple2f;
	typedef Tuple2<int> Tuple2i;

}

#endif /* VMATH_TUPLE2_H_ */
