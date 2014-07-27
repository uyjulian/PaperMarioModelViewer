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

#ifndef PMWORLDGL_H_
#define PMWORLDGL_H_

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include "vecmath/Vecmath.h"
#include "renderer/RenderGL.h"
#include "PMWorld.h"
#include "TPL.h"
#include "common.h"

class PMWorldGL : public PMWorld {
public:

	TPL tpl;
	std::vector<GLuint> glTextures;

	std::string errorMessage;

public:

	void Init() {

	}

	void Draw () {
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		drawSceneGraph(sgRecords[0]);

		/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		for (u32 i = 0; i < meshes.size(); i++) {
			const Mesh& mesh = meshes[i];

			for (u32 j = 0; j < mesh.polyCount; j++) {
				const Polygon& polygon = mesh.polygons[j];
				glBegin(GL_TRIANGLE_STRIP);

				for (u32 k = 0; k < polygon.vertexCount; k++) {
					const PolyVertex& pvertex = polygon.vertices[k];
					const Vertex& vertex = vertices[pvertex.vertexID];

					glVertex3f(vertex.x, vertex.y, vertex.z);
				}
				glEnd();
			}
		}*/
		/*glBegin(GL_POINTS);
		for (unsigned int i = 0; i < vertices.size(); i++) {
			glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
		}
		glEnd();*/
	}

	void drawSceneGraph (const Scenegraph& sgr) {
		glPushMatrix();



		glTranslatef(sgr.u0x30, sgr.u0x34, sgr.u0x38);

		glScalef(sgr.u0x18, sgr.u0x1C, sgr.u0x20);


		glTranslatef(sgr.u0x48, sgr.u0x4C, sgr.u0x50);
		//glRotatef(sgr.u0x2C, 0.f, 0.f, 1.f);
		//glRotatef(sgr.u0x28, 0.f, 1.f, 0.f);
		//glRotatef(sgr.u0x24, 1.f, 0.f, 0.f);
		glTranslatef(sgr.u0x3C, sgr.u0x40, sgr.u0x44);


		if (sgr.u0x5C != 0) {
			const Material& material = materials[sgr.u0x60];

			drawMesh(sgr.meshes[0], material);
		}

		glPopMatrix();

		if (sgr.addrChild != 0) {
			drawSceneGraph(sgRecords[sgr.childID]);
		}

		//glPopMatrix();

		if (sgr.addrNext != 0) {
			drawSceneGraph(sgRecords[sgr.nextID]);
		}
	}

	void drawMesh (const Mesh& mesh, const Material& material) {

		if (material.textureID >= 0) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, glTextures[material.textureID]);
		}

		for (u32 j = 0; j < mesh.polyCount; j++) {
			const Polygon& polygon = mesh.polygons[j];
			glBegin(GL_TRIANGLE_STRIP);

			for (u32 k = 0; k < polygon.vertexCount; k++) {
				const PolyVertex& pvertex = polygon.vertices[k];
				const Vertex& vertex = vertices[pvertex.vertexID];

				glVertex3f(vertex.x, vertex.y, vertex.z);
				glTexCoord2f(vertex.x, vertex.y);
			}
			glEnd();
		}

		glDisable(GL_TEXTURE_2D);
	}

	bool LoadFile (const std::string& file) {
		if (!PMWorld::LoadFile(file)) {
			errorMessage.append("could not open world file '" + filename + "';");
			return false;
		}

		std::string tplPath = pathname(file) + "/t";

		if (!tpl.LoadFile(tplPath)) {
			errorMessage.append("could not open texture file '" + tplPath + "';");
			return false;
		}

		std::string infoPath = filename + ".info.txt";

		// Write info file
		if (!PMWorld::WriteInfoFile(infoPath)) {
			errorMessage.append("could not open info file '" + infoPath + "';");
			return false;
		}

		LoadTextures();
		return true;
	}

	void LoadTextures () {
		glTextures.resize(tpl._header.nTextures);
		glGenTextures(tpl._header.nTextures, &glTextures[0]);

		//for (unsigned int i = 0; i < tpl._header.nTextures; i++) {
		for (unsigned int i = 0; i < texTable.textureCount; i++) {
			int ti = i;
			glBindTexture(GL_TEXTURE_2D, glTextures[ti]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			//switch (texMaps[i].u0x04) {
			//	case 0x00:
			//		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			//		break;
			//	case 0x03:
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			//		break;
			//	default:
			//		std::cout << "(!!) Unknown tex env mode: " << texMaps[i].u0x04 << std::endl;
			//}

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
					tpl._textures[ti].texHeader.width, tpl._textures[ti].texHeader.height,
					0, GL_RGBA, GL_UNSIGNED_BYTE, &tpl._textures[ti].tex._data[0]);
		}
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

#endif /* PMWORLDGL_H_ */
