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

#ifndef PMMODELGL_H_
#define PMMODELGL_H_

#include <iostream>
#include <algorithm>
#include <vector>
#include "renderer/Scenegraph.h"
#include "renderer/RenderGL.h"
#include "system/WindowController.h"
#include "vecmath/Vecmath.h"
#include "PMModel.h"
#include "TPL.h"
#include "common.h"

class PMModelGL : public PMModel {
public:

	TPL tpl;

	gfx::Scenegraph scenegraph;
	gfx::RenderGL renderer;

	std::string errorMessage;

public:

	/*
	 * Parses the PMModel Scene Graph and transformation data into a renderable
	 * Scenegraph object.
	 */

	void parseSceneNode (gfx::Scenegraph::Node* node, unsigned sgRecordIndex) {
		const Scenegraph& sgr = sgRecords[sgRecordIndex];

		vmath::Vector3f v1(sgObjectTransforms[sgr.sgObjectTransIndex + 0].transform,
				sgObjectTransforms[sgr.sgObjectTransIndex + 1].transform,
				sgObjectTransforms[sgr.sgObjectTransIndex + 2].transform);

		vmath::Vector3f v2(sgObjectTransforms[sgr.sgObjectTransIndex + 3].transform,
				sgObjectTransforms[sgr.sgObjectTransIndex + 4].transform,
				sgObjectTransforms[sgr.sgObjectTransIndex + 5].transform);

		vmath::Vector3f v3(sgObjectTransforms[sgr.sgObjectTransIndex + 6].transform,
				sgObjectTransforms[sgr.sgObjectTransIndex + 7].transform,
				sgObjectTransforms[sgr.sgObjectTransIndex + 8].transform);

		vmath::Vector3f v4(sgObjectTransforms[sgr.sgObjectTransIndex + 9].transform,
				sgObjectTransforms[sgr.sgObjectTransIndex + 10].transform,
				sgObjectTransforms[sgr.sgObjectTransIndex + 11].transform);

		vmath::Vector3f v5(sgObjectTransforms[sgr.sgObjectTransIndex + 12].transform,
				sgObjectTransforms[sgr.sgObjectTransIndex + 13].transform,
				sgObjectTransforms[sgr.sgObjectTransIndex + 14].transform);

		vmath::Vector3f v6(sgObjectTransforms[sgr.sgObjectTransIndex + 15].transform,
				sgObjectTransforms[sgr.sgObjectTransIndex + 16].transform,
				sgObjectTransforms[sgr.sgObjectTransIndex + 17].transform);

		vmath::Vector3f v7(sgObjectTransforms[sgr.sgObjectTransIndex + 18].transform,
				sgObjectTransforms[sgr.sgObjectTransIndex + 19].transform,
				sgObjectTransforms[sgr.sgObjectTransIndex + 20].transform);

		vmath::Vector3f v8(sgObjectTransforms[sgr.sgObjectTransIndex + 21].transform,
				sgObjectTransforms[sgr.sgObjectTransIndex + 22].transform,
				sgObjectTransforms[sgr.sgObjectTransIndex + 23].transform);

		node->addTransform(gfx::Scenegraph::Node::TRANSFORM_TRANSLATE, v1);
		node->addTransform(gfx::Scenegraph::Node::TRANSFORM_TRANSLATE, v7);
		node->addTransform(gfx::Scenegraph::Node::TRANSFORM_SCALE, v2);
		node->addTransform(gfx::Scenegraph::Node::TRANSFORM_TRANSLATE, 0.f - v8);
		node->addTransform(gfx::Scenegraph::Node::TRANSFORM_ROTATE_ZYX, v4);
		node->addTransform(gfx::Scenegraph::Node::TRANSFORM_TRANSLATE, v5);
		node->addTransform(gfx::Scenegraph::Node::TRANSFORM_ROTATE_ZYX, 2.f * v3);
		node->addTransform(gfx::Scenegraph::Node::TRANSFORM_TRANSLATE, 0.f - v6);

		parseGeometry(node, sgRecordIndex);

		if (sgr.childRecord != -1) {
			gfx::Scenegraph::Node* child = scenegraph.newChild(node);
			parseSceneNode(child, sgr.childRecord);
		}

		if (sgr.nextRecord != -1 && node->hasParent()) {
			gfx::Scenegraph::Node* sibling = scenegraph.newChild(node->getParent());
			parseSceneNode(sibling, sgr.nextRecord);
		}
	}

	void parseGeometry (gfx::Scenegraph::Node* node, unsigned sgRecordIndex) {
		const Scenegraph& sgr = sgRecords[sgRecordIndex];

		// Skip node if it has no geometry attached
		if (sgr.sgObjectIndex == -1) {
			return;
		}

		const SGObject& object = sgObjects[sgr.sgObjectIndex];
		const SGObjectVis& visibility = sgObjectVisibility[sgr.sgObjectVisIndex];

		for (int meshId = 0; meshId < object.meshCount; meshId++) {
			const Mesh& mesh = meshes[object.meshIndex + meshId];
			gfx::Geometry geo;

			geo.mesh(parseMesh(object, meshId));
			geo.spacialNode = node;

			if (mesh.texMapIndex != -1) {
				unsigned textureId = textures[texMaps[mesh.texMapIndex].textureIndex].tplIndex;
				geo.texture = renderer.getTexture(textureId);
			}

			if (visibility.visibility == 0) {
				geo.visible = false;
			}

			parseGeometryBlending(object, geo);
			parseGeometryCulling(object, geo);

			node->addGeometry(renderer.addGeometry(geo));
		}
	}

	void parseGeometryBlending (const SGObject& object, gfx::Geometry& geo) {
		switch (object.blending) {
		case 0x00:
			geo.alphaTest = true;
			geo.alphaThresh = 0.995f;
			break;
		case 0x01:
			break;
		case 0x02:
			geo.alphaTest = true;
			geo.alphaThresh = 0.5f;
			break;
		case 0x03:
			geo.blend = true;
			geo.blendSrc = GL_SRC_ALPHA;
			geo.blendDst = GL_ONE_MINUS_SRC_ALPHA;
			break;
		default:
			std::cout << "(!!) Unknown blending mode: " << object.blending << std::endl;
		}
	}

	void parseGeometryCulling (const SGObject& object, gfx::Geometry& geo) {
		switch (object.culling) {
		case 0x01:
			geo.cull = true;
			geo.cullFunc = GL_BACK;
			break;
		case 0x03:
			geo.cull = false;
			break;
		default:
			std::cout << "(!!) Unknown culling mode: " << object.culling << std::endl;
		}
	}

	gfx::Mesh* parseMesh (const SGObject& object, int meshId) {
		const Mesh& mesh = meshes[object.meshIndex + meshId];

		gfx::TriMesh renderMesh;
		if (mesh.texMapIndex == -1) {
			renderMesh.useTexCoords(false);
		}

		for (int polyId = 0; polyId < mesh.polygonCount; polyId++) {
			const Polygon& poly = polygons[mesh.polygonIndex + polyId];
			std::vector<unsigned> polyIndex;

			// Draw each vertex suite in polygon
			for (unsigned int v = 0; v < poly.vertexCount; v++) {
				unsigned vertexId = object.vertexIndex + polyVertices[mesh.polyVertexIndex + poly.polyVertexIndex + v].vertexIndex;
				unsigned normalId = object.u0x48 + polyNormals[mesh.polyNormalIndex + poly.polyVertexIndex + v].normalIndex;
				unsigned colorId = object.u0x50 + polyColors[mesh.polyColorIndex + poly.polyVertexIndex + v].colorIndex;
				unsigned texCoordId = object.texCoordIndex + polyTexCoords[mesh.polyTexCoordIndex + poly.polyVertexIndex + v].texCoordIndex;

				gfx::vertexDef vdef;
				vdef.vertex = gfx::vertex3f(vertices[vertexId].x, vertices[vertexId].y, vertices[vertexId].z);
				vdef.normal = gfx::normal3f(normals[normalId].nx, normals[normalId].ny, normals[normalId].nz);
				vdef.color = gfx::color4ub(colors[colorId].r, colors[colorId].g, colors[colorId].b, colors[colorId].a);
				if (mesh.texMapIndex != -1) {
					vdef.texCoord = gfx::texCoord2f(texCoords[texCoordId].s, texCoords[texCoordId].t);
				}

				renderMesh.addVertex(vdef);
				polyIndex.push_back(renderMesh.getVertexCount() - 1);
			}

			renderMesh.addIndexPolygon(polyIndex);
		}

		return renderer.addMesh(renderMesh);
	}

	void parseTextures () {
		for (unsigned i = 0; i < textures.size(); i++) {
			gfx::TextureData texData(GL_RGBA, tpl._textures[i].texHeader.width,
					tpl._textures[i].texHeader.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
					0, tpl._textures[i].tex);
			gfx::Texture tex(GL_TEXTURE_2D, texData);

			renderer.addTexture(tex);
		}

		for (unsigned i = 0; i < texMaps.size(); i++) {
			//gfx::Texture* texPtr = renderer.getTexture(texMaps[i].textureIndex);

			switch (texMaps[i].u0x04) {
			case 0x00:
				//tex.setTexEnvMode(GL_REPLACE);
				//tex.setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				//tex.setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				break;
			case 0x03:
				//tex.setTexEnvMode(GL_MODULATE);
				//tex.setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				//tex.setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				break;
			default:
				std::cout << "(!!) Unknown tex env mode: " << texMaps[i].u0x04 << std::endl;
			}
		}
	}

	void Init () {
		AppState& state = AppState::getState();
		renderer.camera(&state.camera);

		parseTextures();

		parseSceneNode (scenegraph.root(), sgRecords.size() - 1);
		scenegraph.update();
	}

	void Draw () {
		renderer.drawGeometry();
	}

	std::string GetInfoString () {
		AppState& state = AppState::getState();

		std::stringstream str("Select: ");

		switch (state.vars.selectState) {
		case AppState::Vars::SELECT_SGNODE:
			str << "Node";
			break;
		case AppState::Vars::SELECT_MESH:
			str << "Mesh";
			break;
		case AppState::Vars::SELECT_POLY:
			str << "Poly";
			break;
		}

		if (state.vars.selectIndex == -1) {
			str << " = ALL";
		}
		else {
			str << " = " << state.vars.selectIndex;
		}

		return str.str();
	}

	bool LoadFile (const std::string& file) {
		if (!PMModel::LoadFile(file)) {
			errorMessage.append("could not open model file '" + filename + "';");
			return false;
		}

		std::string tplPath = pathname(file) + "/" + header.textureFile + "-";

		if (!tpl.LoadFile(tplPath)) {
			errorMessage.append("could not open texture file '" + tplPath + "';");
			return false;
		}

		std::string infoPath = filename + ".info.txt";

		// Write info file
		if (!PMModel::WriteInfoFile(infoPath, tpl)) {
			errorMessage.append("could not open info file '" + infoPath + "';");
			return false;
		}

		return true;
	}

	void drawWireBox (const vmath::Vector3f& vmin, const vmath::Vector3f& vmax) const {
		glBegin(GL_LINE_LOOP);
			glVertex3f(vmin.x, vmin.y, vmin.z);
			glVertex3f(vmin.x, vmin.y, vmax.z);
			glVertex3f(vmax.x, vmin.y, vmax.z);
			glVertex3f(vmax.x, vmin.y, vmin.z);
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex3f(vmin.x, vmax.y, vmin.z);
			glVertex3f(vmin.x, vmax.y, vmax.z);
			glVertex3f(vmax.x, vmax.y, vmax.z);
			glVertex3f(vmax.x, vmax.y, vmin.z);
		glEnd();
		glBegin(GL_LINES);
			glVertex3f(vmin.x, vmin.y, vmin.z);
			glVertex3f(vmin.x, vmax.y, vmin.z);
			glVertex3f(vmin.x, vmin.y, vmax.z);
			glVertex3f(vmin.x, vmax.y, vmax.z);
			glVertex3f(vmax.x, vmin.y, vmax.z);
			glVertex3f(vmax.x, vmax.y, vmax.z);
			glVertex3f(vmax.x, vmin.y, vmin.z);
			glVertex3f(vmax.x, vmax.y, vmin.z);
		glEnd();
	}
};

#endif /* PMMODELGL_H_ */
