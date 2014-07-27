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

#ifndef GFX_GEOMETRY_H_
#define GFX_GEOMETRY_H_

#include "Scenegraph.h"
#include "Mesh.h"
#include "Texture.h"

namespace gfx {

	/**
	 * Geometry reflects an instance of geometric data; more than one Geometry node
	 * may point to the same data set, but each object reflects a separate spacial
	 * instance.
	 */

	class Geometry {
	public:

		Texture* texture;

		Scenegraph::Node* spacialNode;

		bool visible;
		bool alphaTest;
		bool blend;
		bool cull;

		float alphaThresh;
		unsigned blendSrc;
		unsigned blendDst;
		unsigned cullFunc;

	protected:

		Mesh* _mesh;
		BoundAABB _aabb;

	public:

		Geometry ()
			: texture(NULL), spacialNode(NULL), visible(true), alphaTest(false),
			  blend(false), cull(false), _mesh(NULL) {
		}

		Geometry (Mesh* meshPtr)
			: texture(NULL), spacialNode(NULL), visible(true), alphaTest(false),
			  blend(false), cull(false), _mesh(meshPtr) {
		}

		Geometry (Mesh* meshPtr, Texture* texPtr)
			: texture(texPtr), spacialNode(NULL), visible(true), alphaTest(false),
			  blend(false), cull(false), _mesh(meshPtr) {
		}

		const BoundAABB& getAABB () const {
			return _aabb;
		}

		bool hasMesh () const {
			return _mesh != NULL;
		}

		bool hasSpacialNode () const {
			return spacialNode != NULL;
		}

		bool hasTexture () const {
			return texture != NULL;
		}

		Mesh* mesh () {
			return _mesh;
		}

		const Mesh* mesh () const {
			return _mesh;
		}

		void mesh (Mesh* meshPtr) {
			_mesh = meshPtr;

			_aabb.reset();
			_aabb.addPoints(_mesh->getVertexList());
		}

	};

}

#endif /* GFX_GEOMETRY_H_ */
