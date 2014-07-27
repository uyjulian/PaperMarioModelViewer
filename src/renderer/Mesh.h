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

#ifndef GFX_MESH_H_
#define GFX_MESH_H_

#include <vector>
#include <stdexcept>

#include "../vecmath/Vecmath.h"
#include "Storage.h"

namespace gfx {

	class Mesh {
	protected:

		std::vector<vertex3f> vertexList;
		std::vector<normal3f> normalList;
		std::vector<color4ub> colorList;
		std::vector<texCoord2f> texCoordList;
		unsigned int vertexCount;

		std::vector<unsigned int> indexList;
		unsigned int indexCount;

		unsigned int vtxBitmask;

	public:

		enum {
			VTX_VERTEX = 1,
			VTX_NORMAL = 2,
			VTX_COLOR = 4,
			VTX_TEXCOORD = 8,
		};

		Mesh ()
			: vertexList(0), normalList(0), colorList(0), texCoordList(0), vertexCount(0),
			  indexList(0), indexCount(0), vtxBitmask(0xFFFF) {
		}

		Mesh (unsigned int bitmask)
			: vertexList(0), normalList(0), colorList(0), texCoordList(0), vertexCount(0),
			  indexList(0), indexCount(0), vtxBitmask(bitmask) {
		}

		void addIndex (unsigned int idx) {
			if (idx >= vertexCount) {
				throw std::out_of_range("Vertex ID out of range.");
			}

			indexList.push_back(idx);
			indexCount++;
		}

		void addVertex (const vertexDef& vdef) {
			if (vtxBitmask & VTX_VERTEX) {
				vertexList.push_back(vdef.vertex);
			}
			if (vtxBitmask & VTX_NORMAL) {
				normalList.push_back(vdef.normal);
			}
			if (vtxBitmask & VTX_COLOR) {
				colorList.push_back(vdef.color);
			}
			if (vtxBitmask & VTX_TEXCOORD) {
				texCoordList.push_back(vdef.texCoord);
			}

			vertexCount++;
		}

		void clear () {
			vertexList.clear();
			normalList.clear();
			colorList.clear();
			texCoordList.clear();
			indexList.clear();
		}

		const std::vector<color4ub>& getColorList () const {
			return colorList;
		}

		unsigned int getIndexCount () const {
			return indexCount;
		}

		const std::vector<unsigned int>& getIndexList () const {
			return indexList;
		}

		const std::vector<normal3f>& getNormalList () const {
			return normalList;
		}

		const std::vector<texCoord2f>& getTexCoordList () const {
			return texCoordList;
		}

		unsigned int getVertexCount () const {
			return vertexCount;
		}

		const std::vector<vertex3f>& getVertexList () const {
			return vertexList;
		}

		bool useColors () const {
			return (vtxBitmask & VTX_COLOR);
		}

		void useColors (bool state) {
			if (state != (vtxBitmask & VTX_COLOR)) {
				clear();
			}
			setState(VTX_COLOR, state);
		}

	    bool useNormals () const {
	    	return (vtxBitmask & VTX_NORMAL);
	    }

		void useNormals (bool state) {
			if (state != (vtxBitmask & VTX_NORMAL)) {
				clear();
			}
			setState(VTX_NORMAL, state);
		}

		bool useTexCoords () const {
			return (vtxBitmask & VTX_TEXCOORD);
		}

		void useTexCoords (bool state) {
			if (state != (vtxBitmask & VTX_TEXCOORD)) {
				clear();
			}
			setState(VTX_TEXCOORD, state);
		}

		bool useVertices () const {
			return (vtxBitmask & VTX_VERTEX);
		}

		void useVertices (bool state) {
			if (state != (vtxBitmask & VTX_VERTEX)) {
				clear();
			}
			setState(VTX_VERTEX, state);
		}

	protected:

		void disableState (unsigned int mask) {
			vtxBitmask &= (0xFFFFFFFF ^ mask);
		}

		void enableState (unsigned int mask) {
			vtxBitmask |= mask;
		}

		void setState (unsigned int mask, bool state) {
			if (state) {
				enableState(mask);
			}
			else {
				disableState(mask);
			}
		}

	};


	class TriMesh : public Mesh {
	public:

		TriMesh ()
			: Mesh() {
		}

		TriMesh (unsigned int bitmask)
			: Mesh(bitmask) {
		}

		void addIndexPolygon (const std::vector<unsigned int>& points) {
			if (points.size() <= 3) {
				for (unsigned int i = 0; i < points.size(); i++) {
					addIndex(points[i]);
				}
				return;
			}

			// If we're dealing with quads or higher, split them

			unsigned int p0 = points[0];
			for (unsigned int i = 2; i < points.size(); i++) {
				addIndex(p0);
				addIndex(points[i - 1]);
				addIndex(points[i]);
			}
		}

		void addIndexTriangle (unsigned int i0, unsigned int i1, unsigned int i2) {
			addIndex(i0);
			addIndex(i1);
			addIndex(i2);
		}

	};

}

#endif /* GFX_MESH_H_ */
