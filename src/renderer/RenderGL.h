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

#ifndef GFX_RENDERGL_H_
#define GFX_RENDERGL_H_

#include <OpenGL/gl.h>
#include <vector>
#include <algorithm>

#include "../vecmath/Vecmath.h"
#include "../vecmath/MatrixG4.h"
#include "Mesh.h"
#include "Texture.h"
#include "CameraController.h"
#include "../AppState.h"

namespace gfx {

	/**
	 * Sorting function for rendering transparent meshes back-to-font.
	 * Right now this is a hack and very inefficient.  Sorting by mesh
	 * is also not sufficient sometimes.
	 */

	bool blendSort (const Geometry* a, const Geometry* b) {
		CameraController& cc = CameraController::getController();

		vmath::Vector3f transEye = cc.camera->getEye();
		//cc.camera->getCameraTransform().transform(transEye);

		vmath::Vector3f transA = a->getAABB().center();
		vmath::Vector3f transB = b->getAABB().center();
		cc.camera->getCameraTransform().transform(transA);
		cc.camera->getCameraTransform().transform(transB);

		vmath::Vector3f dista = transA - transEye;
		vmath::Vector3f distb = transB - transEye;

		return dista.length() > distb.length();
	}

	/**
	 * Main rendering class.  This class owns most of its resources, so
	 * the client need not worry about cleaning up returned pointers.
	 */

	class RenderGL {
	protected:

		std::list<Mesh> meshList;
		std::vector<Texture> textures;

		std::vector<Geometry> geoList;

		Camera* _camera;

	public:

		Geometry* addGeometry (const Geometry& geo) {
			geoList.push_back(geo);
			return &geoList.back();
		}

		Mesh* addMesh (const Mesh& mesh) {
			meshList.push_back(mesh);
			return &meshList.back();
		}

		Texture* addTexture (const Texture& tex) {
			textures.push_back(tex);
			return &textures.back();
		}

		void camera (Camera* cam) {
			_camera = cam;
		}

		/**
		 * Draw all geometry added to the renderer.  First solid objects are
		 * drawn, then any transparent meshes are sorted and drawn.
		 */

		void drawGeometry () {
			AppState& state = AppState::getState();
			std::vector<const Geometry*> blended;

			for (unsigned i = 0; i < geoList.size(); i++) {
				const Geometry* citer = &geoList[i];
				if (state.vars.selectState == AppState::Vars::SELECT_SGNODE && state.vars.selectIndex != -1) {
					if (state.vars.selectIndex != (int)i) {
						continue;
					}
				}

				if (!citer->visible) {
					continue;
				}

				if (citer->blend) {
					blended.push_back(&*citer);
					continue;
				}

				drawGeometry(&*citer);
			}

			CameraController& cc = CameraController::getController();
			cc.camera = _camera;

			sort(blended.begin(), blended.end(), blendSort);

			std::vector<const Geometry*>::const_iterator cpiter;
			for (cpiter = blended.begin(); cpiter != blended.end(); cpiter++) {
				const Geometry* geoPtr = *cpiter;
				drawGeometry(geoPtr);
			}
		}

		void drawGeometry (const Geometry* geo) {
			const vmath::MatrixG4f& worldMat = geo->spacialNode->getWorldMatrix();

			glPushMatrix();
			glPushAttrib(GL_ENABLE_BIT);

			glMultMatrixf(worldMat.asArray());

			if (geo->hasTexture()) {
				geo->texture->enable();
				geo->texture->bind();
			}

			if (geo->alphaTest) {
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GEQUAL, geo->alphaThresh);
			}
			if (geo->blend) {
				glEnable(GL_BLEND);
				glBlendFunc(geo->blendSrc, geo->blendDst);
			}
			if (geo->cull) {
				glEnable(GL_CULL_FACE);
				glCullFace(geo->cullFunc);
			}

			drawMesh(geo->mesh(), GL_TRIANGLES);

			glPopAttrib();
			glPopMatrix();
		}

		void drawMesh (const Mesh* mesh, GLuint renderType) {
			drawMeshImmediate(mesh, renderType);
		}

		void drawMeshImmediate (const Mesh* mesh, GLuint renderType) {
			const std::vector<vertex3f>& vertexList = mesh->getVertexList();
			const std::vector<normal3f>& normalList = mesh->getNormalList();
			const std::vector<color4ub>& colorList = mesh->getColorList();
			const std::vector<texCoord2f>& texCoordList = mesh->getTexCoordList();
			const std::vector<unsigned int>& indexList = mesh->getIndexList();

			typedef std::vector<unsigned int>::const_iterator uintCIter;

			glBegin(renderType);

			for (uintCIter iter = indexList.begin(); iter < indexList.end(); iter++) {
				if (mesh->useColors()) {
					const color4ub& color = colorList[*iter];
					glColor4ub(color.r, color.g, color.b, color.a);
				}

				if (mesh->useNormals()) {
					const normal3f& normal = normalList[*iter];
					glNormal3f(normal.nx, normal.ny, normal.nz);
				}

				if (mesh->useTexCoords()) {
					const texCoord2f& texCoord = texCoordList[*iter];
					glTexCoord2f(texCoord.s, texCoord.t);
				}

				if (mesh->useVertices()) {
					const vertex3f& vertex = vertexList[*iter];
					glVertex3f(vertex.x, vertex.y, vertex.z);
				}
			}

			glEnd();
		}

		const std::vector<Geometry>& getGeometry () const {
			return geoList;
		}

		Texture* getTexture (unsigned id) {
			return &textures.at(id);
		}

		unsigned getTextureCount () const {
			return textures.size();
		}
	};



}

#endif /* GFX_RENDERGL_H_ */
