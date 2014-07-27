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

#ifndef GFX_BOUND_H_
#define GFX_BOUND_H_

#include <vector>
#include <cfloat>

#include "../vecmath/Vecmath.h"
#include "Storage.h"

namespace gfx {

	class Bound {
	protected:

		vmath::Vector3f _center;
		vmath::Vector3f _extants;
		bool _singular;

	public:

		Bound ()
			: _center(0.f), _extants(0.f), _singular(true) {
		}

		virtual ~Bound () { }

		void addPoints (const std::vector<vmath::Vector3f>& verts) {
			if (verts.size() == 0) {
				return;
			}

			vmath::Vector3f min(FLT_MAX);
			vmath::Vector3f max(FLT_MIN);

			if (!_singular) {
				min = _center - _extants;
				max = _center + _extants;
			}

			for (unsigned i = 0; i < verts.size(); i++) {
				if (verts[i].x < min.x) min.x = verts[i].x;
				else if (verts[i].x > max.x) max.x = verts[i].x;

				if (verts[i].y < min.y) min.y = verts[i].y;
				else if (verts[i].y > max.y) max.y = verts[i].y;

				if (verts[i].z < min.z) min.z = verts[i].z;
				else if (verts[i].z > max.z) max.z = verts[i].z;
			}

			recalcBound(min, max);
		}

		void addPoints (const std::vector<vertex3f>& verts) {
			if (verts.size() == 0) {
				return;
			}

			vmath::Vector3f min(FLT_MAX);
			vmath::Vector3f max(FLT_MIN);

			if (!_singular) {
				min = _center - _extants;
				max = _center + _extants;
			}

			for (unsigned i = 0; i < verts.size(); i++) {
				if (verts[i].x < min.x) min.x = verts[i].x;
				else if (verts[i].x > max.x) max.x = verts[i].x;

				if (verts[i].y < min.y) min.y = verts[i].y;
				else if (verts[i].y > max.y) max.y = verts[i].y;

				if (verts[i].z < min.z) min.z = verts[i].z;
				else if (verts[i].z > max.z) max.z = verts[i].z;
			}

			recalcBound(min, max);
		}

		const vmath::Vector3f& center () const {
			return _center;
		}

		void center (const vmath::Vector3f& vec) {
			_center = vec;
		}

		const vmath::Vector3f& extants () const {
			return _extants;
		}

		void extants (const vmath::Vector3f& vec) {
			_extants = vec;
		}

		void reset () {
			_center.set(0.f);
			_extants.set(0.f);
			_singular = true;
		}

		virtual void transform (const vmath::Matrix4f& mat) = 0;

	protected:

		virtual void recalcBound (const vmath::Vector3f& min, const vmath::Vector3f& max) = 0;
	};

}

#endif /* GFX_BOUND_H_ */
