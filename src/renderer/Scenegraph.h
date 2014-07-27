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

#ifndef GFX_SCENEGRAPH_H_
#define GFX_SCENEGRAPH_H_

#include <utility>
#include <list>
#include <vector>

#include "../vecmath/MatrixG4.h"
#include "BoundAABB.h"

namespace gfx {

	class Geometry;

	/**
	 * The Scene Graph represents the logical, and by extension, spacial relationship
	 * of objects in the scene.
	 *
	 * Each node caries local transformation data relative to the world position of
	 * its parent.  The world transformation is precomputed for each object to allow
	 * for independent draw order.
	 */

	class Scenegraph {
	public:

		class Node {
		public:

			enum TransformType {
				TRANSFORM_TRANSLATE,
				TRANSFORM_SCALE,
				TRANSFORM_ROTATE_XYZ,
				TRANSFORM_ROTATE_ZYX,
			};

			typedef std::pair<TransformType, vmath::Vector3f> Transform;

		protected:

			typedef std::list<Node*>::iterator NodeIter;

			friend class Scenegraph;

		protected:

			Node* parent;
			std::list<Node*> children;

			std::vector<Geometry*> geoNodes;

			vmath::MatrixG4f worldMatrix;
			bool dirtyMatrix;
			bool visible;

			std::vector<Transform> transformSet;

		public:

			Node ()
				: parent(NULL), children(0), geoNodes(0), worldMatrix(vmath::Matrix4f::IDENTITY),
				  dirtyMatrix(true), visible(true), transformSet(0) {
			}

			void addGeometry (Geometry* geo) {
				geoNodes.push_back(geo);
			}

			unsigned addTransform (TransformType type, const vmath::Vector3f& v) {
				transformSet.push_back(std::make_pair(type, v));
				dirtyMatrix = true;
				return transformSet.size() - 1;
			}

			const std::list<Node*>& getChildren () const {
				return children;
			}

			Node* getParent () const {
				return parent;
			}

			const vmath::Vector3f& getTransform (unsigned index) const {
				return transformSet.at(index).second;
			}

			TransformType getTransformType (unsigned index) const {
				return transformSet.at(index).first;
			}

			const vmath::MatrixG4f& getWorldMatrix () const {
				return worldMatrix;
			}

			bool hasChildren () const {
				return children.size() > 0;
			}

			bool hasParent () const {
				return parent != NULL;
			}

			void setTransform (unsigned index, const vmath::Vector3f& v) {
				transformSet.at(index).second = v;
				dirtyMatrix = true;
			}

			void setVisibility (bool vis) {
				visible = vis;
			}

			void updateTransform (unsigned index, const vmath::Vector3f& v) {
				transformSet.at(index).second += v;
				dirtyMatrix = true;
			}

		protected:

			void checkWorldMatrix () {
				if (parent == NULL && dirtyMatrix) {
					worldMatrix.setIdentity();
					updateWorldMatrix();
					return;
				}

				for (NodeIter iter = children.begin(); iter != children.end(); iter++) {
					Node* child = *iter;
					if (child->dirtyMatrix) {
						child->updateWorldMatrix(worldMatrix);
					}
					else {
						child->checkWorldMatrix();
					}
				}
			}

			void updateWorldMatrix (const vmath::MatrixG4f& parentWorld) {
				worldMatrix.set(parentWorld);
				updateWorldMatrix();
			}

			void updateWorldMatrix () {

				// Recalculate world matrix
				for (unsigned i = 0; i < transformSet.size(); i++) {
					const vmath::Vector3f& v = transformSet[i].second;

					switch (transformSet[i].first) {
					case TRANSFORM_TRANSLATE:
						worldMatrix.translate(v.x, v.y, v.z);
						break;
					case TRANSFORM_SCALE:
						worldMatrix.scale(v.x, v.y, v.z);
						break;
					case TRANSFORM_ROTATE_XYZ:
						worldMatrix.rotateX(v.x);
						worldMatrix.rotateY(v.y);
						worldMatrix.rotateZ(v.z);
						break;
					case TRANSFORM_ROTATE_ZYX:
						worldMatrix.rotateZ(v.z);
						worldMatrix.rotateY(v.y);
						worldMatrix.rotateX(v.x);
						break;
					}
				}

				for (NodeIter iter = children.begin(); iter != children.end(); iter++) {
					Node* child = *iter;
					child->updateWorldMatrix(worldMatrix);
				}

				worldMatrix.transpose();

				dirtyMatrix = false;
			}
		};

	protected:

		Node* rootNode;

		std::list<Node> nodeSet;

	public:

		Scenegraph () {
			nodeSet.push_back(Node());
			rootNode = &nodeSet.front();
		}

		virtual ~Scenegraph () { }

		Node* newChild (Node* parent) {
			nodeSet.push_back(Node());
			Node* node = &nodeSet.back();

			node->parent = parent;
			parent->children.push_back(node);

			return node;
		}

		Node* root () const {
			return rootNode;
		}

		void update () {
			rootNode->checkWorldMatrix();
		}

	};

}

#include "Geometry.h"

#endif /* GFX_SCENEGRAPH_H_ */
