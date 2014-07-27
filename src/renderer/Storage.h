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

#ifndef GFX_STORAGE_H_
#define GFX_STORAGE_H_

namespace gfx {

	struct vertex3f {
		float x;
		float y;
		float z;

		vertex3f () { }

		vertex3f (float a, float b, float c)
			: x(a), y(b), z(c) {
		}
	};

	struct normal3f {
		float nx;
		float ny;
		float nz;

		normal3f () { }

		normal3f (float a, float b, float c)
			: nx(a), ny(b), nz(c) {
		}
	};

	struct color4ub {
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

		color4ub () { }

		color4ub (unsigned char cr, unsigned char cg, unsigned char cb, unsigned char ca)
			: r(cr), g(cg), b(cb), a(ca) {
		}
	};

	struct texCoord2f {
		float s;
		float t;

		texCoord2f ()
			: s(0), t(0) {
		}

		texCoord2f (float ts, float tt)
			: s(ts), t(tt) {
		}
	};

	struct vertexDef {
		vertex3f vertex;
		normal3f normal;
		color4ub color;
		texCoord2f texCoord;
	};

}

#endif /* GFX_STORAGE_H_ */
