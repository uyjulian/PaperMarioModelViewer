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

#ifndef COLOR_H_
#define COLOR_H_

#include "common.h"

const float CI_RED = 0.212;
const float CI_GREEN = 0.701;
const float CI_BLUE = 0.087;

class Color8 {
protected:

	u8 r;
	u8 g;
	u8 b;
	u8 a;
	u8 lum;

public:

	Color8 ()
		: r(0), g(0), b(0), a(0), lum(0) {
	}

	Color8 (u8 red, u8 green, u8 blue, u8 alpha)
		: r(red), g(green), b(blue), a(alpha), lum((u8)(r * CI_RED + g * CI_GREEN + b * CI_BLUE)) {
	}

	Color8 (u8 luminance)
		: r(luminance), g(luminance), b(luminance), a(255), lum(luminance) {
	}

	Color8 (u8 luminance, u8 alpha)
		: r(luminance), g(luminance), b(luminance), a(alpha), lum(luminance) {
	}

	Color8 (const Color8& c)
		: r(c.red()), g(c.green()), b(c.blue()), a(c.alpha()), lum(c.luminance()) {
	}

	void blend (const Color8& c0, const Color8& c1, u8 alpha) {
		r = ((255 - alpha) * c0.red() + alpha * c1.red()) >> 8;
		g = ((255 - alpha) * c0.green() + alpha * c1.green()) >> 8;
		b = ((255 - alpha) * c0.blue() + alpha * c1.blue()) >> 8;
		a = ((255 - alpha) * c0.alpha() + alpha * c1.alpha()) >> 8;
		lum = ((255 - alpha) * c0.luminance() + alpha * c1.luminance()) >> 8;
	}

	void set (u8 red, u8 green, u8 blue, u8 alpha) {
		r = red;
		g = green;
		b = blue;
		a = alpha;
		lum = (u8)(r * CI_RED + g * CI_GREEN + b * CI_BLUE);
	}

	u8 red () const { return r; }
	u8 green () const { return g; }
	u8 blue () const { return b; }
	u8 alpha () const { return a; }
	u8 luminance () const { return lum; }
};

#endif /* COLOR_H_ */
