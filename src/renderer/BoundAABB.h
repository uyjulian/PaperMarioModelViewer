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

#ifndef GFX_BOUNDAABB_H_
#define GFX_BOUNDAABB_H_

#include "Bound.h"

namespace gfx {

	class BoundAABB : public Bound {
	public:

		BoundAABB ()
			: Bound() {
		}

		virtual ~BoundAABB () { }

		void transform (const vmath::Matrix4f& mat) {
			vmath::Vector3f min = _center - _extants;
			vmath::Vector3f max = _center + _extants;

			vmath::Vector3f newMin(mat.getElement(12), mat.getElement(13), mat.getElement(14));
			vmath::Vector3f newMax(newMin);

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					float av = mat(i, j) * min(j);
					float bv = mat(i, j) * max(j);

					if (av < bv) {
						newMin(i) += av;
						newMax(i) += bv;
					}
					else {
						newMin(i) += bv;
						newMax(i) += av;
					}
				}
			}

			recalcBound(newMin, newMax);
		}

	protected:

		void recalcBound (const vmath::Vector3f& min, const vmath::Vector3f& max) {
			_center = (min + max) / 2.f;
			_extants = max - _center;
			_singular = false;
		}

	};

}


#endif /* GFX_BOUNDAABB_H_ */
