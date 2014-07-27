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

#ifndef PMMODEL_H_
#define PMMODEL_H_

#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "vecmath/Vector3.h"
#include "TPL.h"
#include "common.h"

class PMModel {
public:

	enum PMBlock {
		SGOBlock, PBlock, VBlock, PVBlock, NBlock, PNBlock, CBlock, PCBlock, PTCBlock,
		BLOCK11, BLOCK12, BLOCK13, BLOCK14, BLOCK15, BLOCK16, BLOCK17,
		TCBlock, BLOCK19, TMBlock, TBlock, MBlock, SGOVBlock, SGOTBlock, SGBlock, ABlock,
	};

	/**
	 * Header [BLOCK1]
	 */

	struct Header {
		enum { SIZE = 432 };

		u32 addrAnimation;
		std::string modelFile;
		std::string textureFile;
		std::string date;
		u32 u0xC4;
		u32 u0xC8;
		u32 u0xCC;
		f32 bboxV0x;
		f32 bboxV0y;
		f32 bboxV0z;
		f32 bboxV1x;
		f32 bboxV1y;
		f32 bboxV1z;
		std::vector<u32> numBlocks;
		std::vector<u32> blockOffset;

		Header () {
			numBlocks.resize(25);
			blockOffset.resize(25);
		}

		void read (const std::vector<u8>& buffer, int offset) {
			addrAnimation = getU32(buffer, offset + 0);
			modelFile = getString(buffer, offset + 4, 64);
			textureFile = getString(buffer, offset + 68, 64);
			date = getString(buffer, offset + 132, 64);
			u0xC4 = getU32(buffer, offset + 196);
			u0xC8 = getU32(buffer, offset + 200);
			u0xCC = getU32(buffer, offset + 204);
			bboxV0x = getF32(buffer, offset + 208);
			bboxV0y = getF32(buffer, offset + 212);
			bboxV0z = getF32(buffer, offset + 216);
			bboxV1x = getF32(buffer, offset + 220);
			bboxV1y = getF32(buffer, offset + 224);
			bboxV1z = getF32(buffer, offset + 228);

			int curOffset = offset + 232;
			for (int i = 0; i < 25; i++, curOffset += 4) {
				numBlocks[i] = getU32(buffer, curOffset);
			}
			for (int i = 0; i < 25; i++, curOffset += 4) {
				blockOffset[i] = getU32(buffer, curOffset);
			}

			std::cout << "Model File: " << modelFile << std::endl;
			std::cout << "Texture File: " << textureFile << std::endl;
			std::cout << "Date: " << date << std::endl;

			for (int i = 0; i < 25; i++) {
				std::cout << "Block " << i + 2 << ": " << numBlocks[i] << " at " << blockOffset[i] << std::endl;
			}
		}

		std::string toString () {
			std::stringstream str;

			str << std::left;
			str << std::setw(16) << "Model:" << modelFile << std::endl;
			str << std::setw(16) << "Texture:" << textureFile << std::endl;
			str << std::setw(16) << "Date:" << date << std::endl;
			str << std::setw(16) << "Unknowns:" << u0xC4 << std::endl;
			str << std::setw(16) << " " << u0xC8 << std::endl;
			str << std::setw(16) << " " << u0xCC << std::endl;
			str << std::setw(16) << "BBoxV0:" << vmath::Vector3f(bboxV0x, bboxV0y, bboxV0z) << std::endl;
			str << std::setw(16) << "BBoxV1:" << vmath::Vector3f(bboxV1x, bboxV1y, bboxV1z) << std::endl;

			return str.str();
		}
	};

	/**
	 * Scenegraph Object [Block 2]
	 */

	struct SGObject {
		enum { SIZE = 168 };

		std::string name;
		s32 vertexIndex;
		s32 vertexCount;
		u32 u0x48;
		u32 u0x4C;
		u32 u0x50;
		u32 u0x54;
		s32 texCoordIndex;
		s32 texCoordCount;
		s32 s0x60;
		s32 s0x64;
		s32 s0x68;
		s32 s0x6C;
		s32 s0x70;
		s32 s0x74;
		s32 s0x78;
		s32 s0x7C;
		s32 s0x80;
		s32 s0x84;
		s32 s0x88;
		s32 s0x8C;
		s32 s0x90;
		s32 s0x94;
		s32 meshIndex;
		s32 meshCount;
		u32 blending;
		u32 culling;

		void read (const std::vector<u8>& buffer, int offset) {
			name = getString(buffer, offset + 0, 64);
			vertexIndex = getS32(buffer, offset + 64);
			vertexCount = getS32(buffer, offset + 68);
			u0x48 = getU32(buffer, offset + 72); // normalIndex
			u0x4C = getU32(buffer, offset + 76); // normalCount
			u0x50 = getU32(buffer, offset + 80); // colorIndex
			u0x54 = getU32(buffer, offset + 84); // colorCount
			texCoordIndex = getS32(buffer, offset + 88);
			texCoordCount = getS32(buffer, offset + 92);
			s0x60 = getS32(buffer, offset + 96);
			s0x64 = getS32(buffer, offset + 100);
			s0x68 = getS32(buffer, offset + 104);
			s0x6C = getS32(buffer, offset + 108);
			s0x70 = getS32(buffer, offset + 112);
			s0x74 = getS32(buffer, offset + 116);
			s0x78 = getS32(buffer, offset + 120);
			s0x7C = getS32(buffer, offset + 124);
			s0x80 = getS32(buffer, offset + 128);
			s0x84 = getS32(buffer, offset + 132);
			s0x88 = getS32(buffer, offset + 136);
			s0x8C = getS32(buffer, offset + 140);
			s0x90 = getS32(buffer, offset + 144);
			s0x94 = getS32(buffer, offset + 148);
			meshIndex = getS32(buffer, offset + 152);
			meshCount = getS32(buffer, offset + 156);
			blending = getU32(buffer, offset + 160);
			culling = getU32(buffer, offset + 164);
		}

		static std::string headerString () {
			std::stringstream str;

			str << "      ID|                       Name|" << std::endl;
			str << "        |  VertIdx| VertCnt|     u48|     u4C|     u50|     u54| TxCdIdx| TxCdCnt|" << std::endl;
			str << "        |      u60|     u64|     u68|     u6C|     u70|     u74|     u78|     u7C|     u80|    u84|" << std::endl;
			str << "        |      u88|     u8C|     u90|     u94| meshIdx| meshCnt|   blend|    cull|";

			return str.str();
		}

		std::string toString (int id) {
			std::stringstream str;

			str << std::setw(8) << id << ": " << name << std::endl;
			str << std::setw(10) << " ";
			str << std::setw(8) << vertexIndex << ",";
			str << std::setw(8) << vertexCount << ",";
			str << std::setw(8) << u0x48 << ",";
			str << std::setw(8) << u0x4C << ",";
			str << std::setw(8) << u0x50 << ",";
			str << std::setw(8) << u0x54 << ",";
			str << std::setw(8) << texCoordIndex << ",";
			str << std::setw(8) << texCoordCount << "," << std::endl;
			str << std::setw(10) << " ";
			str << std::setw(8) << s0x60 << ",";
			str << std::setw(8) << s0x64 << ",";
			str << std::setw(8) << s0x68 << ",";
			str << std::setw(8) << s0x6C << ",";
			str << std::setw(8) << s0x70 << ",";
			str << std::setw(8) << s0x74 << ",";
			str << std::setw(8) << s0x78 << ",";
			str << std::setw(8) << s0x7C << ",";
			str << std::setw(8) << s0x80 << ",";
			str << std::setw(8) << s0x84 << "," << std::endl;
			str << std::setw(10) << " ";
			str << std::setw(8) << s0x88 << ",";
			str << std::setw(8) << s0x8C << ",";
			str << std::setw(8) << s0x90 << ",";
			str << std::setw(8) << s0x94 << ",";
			str << std::setw(8) << meshIndex << ",";
			str << std::setw(8) << meshCount << ",";
			str << std::setw(8) << blending << ",";
			str << std::setw(8) << culling << std::endl;

			return str.str();
		}
	};

	/**
	 * Polygons [Block 3]
	 */

	struct Polygon {
		enum { SIZE = 8 };

		u32 polyVertexIndex;
		u32 vertexCount;

		void read (const std::vector<u8>& buffer, int offset) {
			polyVertexIndex = getU32(buffer, offset + 0);
			vertexCount = getU32(buffer, offset + 4);
		}

		static std::string headerString () {
			std::stringstream str;

			str << "      ID|  pVertId| VertCnt|";

			return str.str();
		}

		std::string toString (int id) const {
			std::stringstream str;

			str << std::setw(8) << id << ": ";
			str << std::setw(8) << polyVertexIndex << ",";
			str << std::setw(8) << vertexCount;

			return str.str();
		}
	};

	/**
	 * Vertices [Block 4]
	 */

	struct Vertex {
		enum { SIZE = 12 };

		f32 x;
		f32 y;
		f32 z;

		void read (const std::vector<u8>& buffer, int offset) {
			x = getF32(buffer, offset + 0);
			y = getF32(buffer, offset + 4);
			z = getF32(buffer, offset + 8);
		}

		std::string toString () const {
			std::stringstream str;
			str.precision(3);

			str << "(";
			str << std::setw(10) << std::fixed << x << ",";
			str << std::setw(10) << std::fixed << y << ",";
			str << std::setw(10) << std::fixed << z;
			str << ")";

			return str.str();
		}
	};

	/**
	 * Polygon Vertex Map [Block 5]
	 */

	struct PolyVertex {
		enum { SIZE = 4 };

		u32 vertexIndex;

		void read (const std::vector<u8>& buffer, int offset) {
			vertexIndex = getU32(buffer, offset + 0);
		}

		static std::string headerString () {
			std::stringstream str;

			str << "      ID|   VertID|->                Vertex Coordinates|";

			return str.str();
		}

		std::string toString (const std::vector<Vertex>& v, int id) const {
			std::stringstream str;

			str << std::setw(8) << id << ": ";
			str << std::setw(8) << vertexIndex << " -> ";
			str << v[vertexIndex].toString();

			return str.str();
		}
	};

	/**
	 * Normals [Block 6]
	 */

	struct Normal {
		enum { SIZE = 12 };

		f32 nx;
		f32 ny;
		f32 nz;

		void read (const std::vector<u8>& buffer, int offset) {
			nx = getF32(buffer, offset + 0);
			ny = getF32(buffer, offset + 4);
			nz = getF32(buffer, offset + 8);
		}

		std::string toString () const {
			std::stringstream str;
			str.precision(3);

			str << "(";
			str << std::setw(10) << std::fixed << nx << ",";
			str << std::setw(10) << std::fixed << ny << ",";
			str << std::setw(10) << std::fixed << nz;
			str << ")";

			return str.str();
		}
	};

	/**
	 * Polygon Normal Map [Block 7]
	 */

	struct PolyNormal {
		enum { SIZE = 4 };

		u32 normalIndex;

		void read (const std::vector<u8>& buffer, int offset) {
			normalIndex = getU32(buffer, offset + 0);
		}

		static std::string headerString () {
			std::stringstream str;

			str << "      ID|   NormID|->                    Vertex Normals|";

			return str.str();
		}

		std::string toString (const std::vector<Normal>& v, int id) const {
			std::stringstream str;

			str << std::setw(8) << id << ": ";
			str << std::setw(8) << normalIndex << " -> ";
			str << v[normalIndex].toString();

			return str.str();
		}
	};

	/**
	 * Color [Block 8]
	 */

	struct Color {
		enum { SIZE = 4 };

		u8 r;
		u8 g;
		u8 b;
		u8 a;

		void read (const std::vector<u8>& buffer, int offset) {
			r = buffer[offset + 0];
			g = buffer[offset + 1];
			b = buffer[offset + 2];
			a = buffer[offset + 3];
		}

		std::string toString () const {
			std::stringstream str;

			str << "(";
			str << std::setw(4) << (int) r << ",";
			str << std::setw(4) << (int) g << ",";
			str << std::setw(4) << (int) b << ",";
			str << std::setw(4) << (int) a;
			str << ")";

			return str.str();
		}
	};

	/**
	 * Polygon Color Map [Block 9]
	 */

	struct PolyColor {
		enum { SIZE = 4 };

		u32 colorIndex;

		void read (const std::vector<u8>& buffer, int offset) {
			colorIndex = getU32(buffer, offset + 0);
		}

		static std::string headerString () {
			std::stringstream str;

			str << "      ID|  ColorID|->         Vertex Colors|";

			return str.str();
		}

		std::string toString (const std::vector<Color>& v, int id) const {
			std::stringstream str;

			str << std::setw(8) << id << ": ";
			str << std::setw(8) << colorIndex << " -> ";
			str << v[colorIndex].toString();

			return str.str();
		}
	};

	/**
	 * Texture Coordinates [Block 18]
	 */

	struct TexCoord {
		enum { SIZE = 8 };

		f32 s;
		f32 t;

		void read (const std::vector<u8>& buffer, int offset) {
			s = getF32(buffer, offset + 0);
			t = getF32(buffer, offset + 4);
		}

		std::string toString () const {
			std::stringstream str;
			str.precision(3);

			str << "(";
			str << std::setw(10) << std::fixed << s << ",";
			str << std::setw(10) << std::fixed << t;
			str << ")";

			return str.str();
		}
	};

	/**
	 * Polygon Texture Coordinate Map [Block 10]
	 */

	struct PolyTexCoord {
		enum { SIZE = 4 };

		u32 texCoordIndex;

		void read (const std::vector<u8>& buffer, int offset) {
			texCoordIndex = getU32(buffer, offset + 0);
		}

		static std::string headerString () {
			std::stringstream str;

			str << "      ID|  TexCdID|->      Vertex Tex Coords|";

			return str.str();
		}

		std::string toString (const std::vector<TexCoord>& v, int id) const {
			std::stringstream str;

			str << std::setw(8) << id << ": ";
			str << std::setw(8) << texCoordIndex << " -> ";
			str << v[texCoordIndex].toString();

			return str.str();
		}
	};

	/**
	 * BLOCK11 - BLOCK17 ommitted; no data available
	 */

	/**
	 * Unknown Block [Block 19]
	 */

	struct Block19 {
		enum { SIZE = 24 };

		f32 f0x00;
		f32 f0x04;
		f32 f0x08;
		f32 f0x0C;
		f32 f0x10;
		f32 f0x14;

		void read (const std::vector<u8>& buffer, int offset) {
			f0x00 = getF32(buffer, offset + 0);
			f0x04 = getF32(buffer, offset + 4);
			f0x08 = getF32(buffer, offset + 8);
			f0x0C = getF32(buffer, offset + 12);
			f0x10 = getF32(buffer, offset + 16);
			f0x14 = getF32(buffer, offset + 20);
		}

		static std::string headerString () {
			std::stringstream str;

			str << "      ID|      u00|     u04|     u08|     u0C|     u10|     u14|";

			return str.str();
		}

		std::string toString (int id) {
			std::stringstream str;
			str.precision(3);

			str << std::fixed;
			str << std::setw(8) << id << ": ";
			str << std::setw(8) << f0x00 << ",";
			str << std::setw(8) << f0x04 << ",";
			str << std::setw(8) << f0x08 << ",";
			str << std::setw(8) << f0x0C << ",";
			str << std::setw(8) << f0x10 << ",";
			str << std::setw(8) << f0x14 << ",";

			return str.str();
		}
	};

	/**
	 * Texture Map [Block 20]
	 */

	struct TextureMap {
		enum { SIZE = 8 };

		u32 textureIndex;
		u32 u0x04;

		void read (const std::vector<u8>& buffer, int offset) {
			textureIndex = getU32(buffer, offset + 0);
			u0x04 = getU32(buffer, offset + 4);
		}

		static std::string headerString () {
			std::stringstream str;

			str << "      ID|    TexId| EnvMode|";

			return str.str();
		}

		std::string toString (int id) {
			std::stringstream str;

			str << std::setw(8) << id << ": ";
			str << std::setw(8) << textureIndex << ",";
			str << std::setw(8) << u0x04;

			return str.str();
		}
	};

	/**
	 * Textures [Block 21]
	 */

	struct Texture {
		enum { SIZE = 64 };

		u32 u0x00;
		u32 tplIndex;
		u32 u0x08;
		std::string name;

		void read (const std::vector<u8>& buffer, int offset) {
			u0x00 = getU32(buffer, offset + 0);
			tplIndex = getU32(buffer, offset + 4);
			u0x08 = getU32(buffer, offset + 8);
			name = getString(buffer, offset + 12, 52);
		}

		static std::string headerString () {
			std::stringstream str;

			str << "      ID|      u00|  TplIdx|     u08|                                                 Name|" << std::endl;
			str << "        |   Format|   WrapS|   WrapT| minFilt| magFilt| lodBias| edgeLod|  minLod|  maxLod|";

			return str.str();
		}

		std::string toString (const TPL& tpl, int id) {
			const TPL::TPLTexHeader& th = tpl._textures[tplIndex].texHeader;

			std::stringstream str;
			str.precision(3);

			str << std::setw(8) << id << ": ";
			str << std::setw(8) << u0x00 << ",";
			str << std::setw(8) << tplIndex << ",";
			str << std::setw(8) << u0x08 << ", ";
			str << std::left << name << std::right << std::endl;
			str << std::setw(10) << " ";
			str << std::setw(8) << th.format << ",";
			str << std::setw(8) << th.wrap_s << ",";
			str << std::setw(8) << th.wrap_t << ",";
			str << std::setw(8) << th.minFilter << ",";
			str << std::setw(8) << th.magFilter << ",";
			str << std::setw(8) << th.lodBias << ",";
			str << std::setw(8) << (int)th.edgeLod << ",";
			str << std::setw(8) << (int)th.minLod << ",";
			str << std::setw(8) << (int)th.maxLod << std::endl;

			return str.str();
		}
	};

	/**
	 * Mesh [Block 22]
	 */

	struct Mesh {
		enum { SIZE = 108 };

		u32 u0x00;
		u32 u0x04;
		u32 u0x08;
		u32 u0x0C;
		s32 texMapIndex;
		s32 s0x14;
		s32 s0x18;
		s32 s0x1C;
		s32 s0x20;
		s32 s0x24;
		s32 s0x28;
		s32 s0x2C;
		s32 s0x30;
		s32 s0x34;
		s32 polygonIndex;
		s32 polygonCount;
		s32 polyVertexIndex;
		s32 polyNormalIndex;
		s32 polyColorIndex;
		s32 polyTexCoordIndex;
		u32 u0x50;
		u32 u0x54;
		u32 u0x58;
		u32 u0x5C;
		u32 u0x60;
		u32 u0x64;
		u32 u0x68;

		void read (const std::vector<u8>& buffer, int offset) {
			u0x00 = getU32(buffer, offset + 0);
			u0x04 = getU32(buffer, offset + 4);
			u0x08 = getU32(buffer, offset + 8);
			u0x0C = getU32(buffer, offset + 12);
			texMapIndex = getS32(buffer, offset + 16);
			s0x14 = getS32(buffer, offset + 20);
			s0x18 = getS32(buffer, offset + 24);
			s0x1C = getS32(buffer, offset + 28);
			s0x20 = getS32(buffer, offset + 32);
			s0x24 = getS32(buffer, offset + 36);
			s0x28 = getS32(buffer, offset + 40);
			s0x2C = getS32(buffer, offset + 44);
			s0x30 = getU32(buffer, offset + 48);
			s0x34 = getS32(buffer, offset + 52);
			polygonIndex = getS32(buffer, offset + 56);
			polygonCount = getS32(buffer, offset + 60);
			polyVertexIndex = getS32(buffer, offset + 64);
			polyNormalIndex = getS32(buffer, offset + 68);
			polyColorIndex = getS32(buffer, offset + 72);
			polyTexCoordIndex = getS32(buffer, offset + 76);
			u0x50 = getU32(buffer, offset + 80);
			u0x54 = getU32(buffer, offset + 84);
			u0x58 = getU32(buffer, offset + 88);
			u0x5C = getU32(buffer, offset + 92);
			u0x60 = getU32(buffer, offset + 96);
			u0x64 = getU32(buffer, offset + 100);
			u0x68 = getU32(buffer, offset + 104);
		}

		static std::string headerString () {
			std::stringstream str;

			str << "      ID|      u00|     u04|     u08|     u0C| txMapId|     u14|     u18|" << std::endl;
			str << "        |      u1C|     u20|     u24|     u28|     u2C|     u30|     u34| polyIdx| polyCnt|" << std::endl;
			str << "        |  pVerIdx| pNorIdx| pColIdx| pTxcIdx|     u50|     u54|     u58|     u5C|     u60|     u64|     u68|" << std::endl;

			return str.str();
		}

		std::string toString (int id) {
			std::stringstream str;
			str << std::uppercase;

			str << std::setw(8) << id << ": ";
			str << std::setw(8) << u0x00 << ",";
			str << std::setw(8) << u0x04 << ",";
			str << std::setw(8) << u0x08 << ",";
			str << std::setw(8) << u0x0C << ",";
			str << std::setw(8) << texMapIndex << ",";
			str << std::setw(8) << s0x14 << ",";
			str << std::setw(8) << s0x18 << "," << std::endl;
			str << std::setw(10) << " ";
			str << std::setw(8) << s0x1C << ",";
			str << std::setw(8) << s0x20 << ",";
			str << std::setw(8) << s0x24 << ",";
			str << std::setw(8) << s0x28 << ",";
			str << std::setw(8) << s0x2C << ",";
			str << std::setw(8) << std::hex << s0x30 << std::dec << ",";
			str << std::setw(8) << s0x34 << ",";
			str << std::setw(8) << polygonIndex << ",";
			str << std::setw(8) << polygonCount << "," << std::endl;
			str << std::setw(10) << " ";
			str << std::setw(8) << polyVertexIndex << ",";
			str << std::setw(8) << polyNormalIndex << ",";
			str << std::setw(8) << polyColorIndex << ",";
			str << std::setw(8) << polyTexCoordIndex << ",";
			str << std::setw(8) << u0x50 << ",";
			str << std::setw(8) << u0x54 << ",";
			str << std::setw(8) << u0x58 << ",";
			str << std::setw(8) << u0x5C << ",";
			str << std::setw(8) << u0x60 << ",";
			str << std::setw(8) << u0x64 << ",";
			str << std::setw(8) << u0x68 << "," << std::endl;

			return str.str();
		}
	};

	/**
	 * Scenegraph Object Visisbility [Block 23]
	 */

	struct SGObjectVis {
		enum { SIZE = 1 };

		s8 visibility;

		void read (const std::vector<u8>& buffer, int offset) {
			visibility = (s8)buffer[offset + 0];
		}

		static std::string headerString () {
			std::stringstream str;

			str << "      ID|  Visible|";

			return str.str();
		}

		std::string toString (int id) {
			std::stringstream str;

			str << std::setw(8) << id << ": ";
			str << std::setw(8) << (int)visibility;

			return str.str();
		}
	};

	/**
	 * Scenegraph Object Transformation [Block 24]
	 */

	struct SGObjectTrans {
		enum { SIZE = 4 };

		f32 transform;

		void read (const std::vector<u8>& buffer, int offset) {
			transform = getF32(buffer, offset + 0);
		}

		static std::string headerString () {
			std::stringstream str;

			str << "  Offset|   TransX|  TransY|  TransZ|  ScaleX|  ScaleY|  ScaleZ|   Rot1Z|   Rot1Y|   Rot1X|   Rot2Z|   Rot2Y|   Rot2Z|" << std::endl;
			str << "        |  R1PostX| R1PostY| R1PostZ|  R1PreX|  R1PreY|  R1PreZ|  SPostX|  SPostY|  SPostZ|   SPreX|   SPreY|   SPreZ|";

			return str.str();
		}

		static std::string toString (const std::vector<SGObjectTrans>& v, int id) {
			std::stringstream str;
			str.precision(3);

			str << std::fixed;
			str << std::setw(8) << id << ": ";
			str << std::setw(8) << v[id+0].transform << ",";
			str << std::setw(8) << v[id+1].transform << ",";
			str << std::setw(8) << v[id+2].transform << ",";
			str << std::setw(8) << v[id+3].transform << ",";
			str << std::setw(8) << v[id+4].transform << ",";
			str << std::setw(8) << v[id+5].transform << ",";
			str << std::setw(8) << v[id+6].transform << ",";
			str << std::setw(8) << v[id+7].transform << ",";
			str << std::setw(8) << v[id+8].transform << ",";
			str << std::setw(8) << v[id+9].transform << ",";
			str << std::setw(8) << v[id+10].transform << ",";
			str << std::setw(8) << v[id+11].transform << "," << std::endl;
			str << std::setw(10) << " ";
			str << std::setw(8) << v[id+12].transform << ",";
			str << std::setw(8) << v[id+13].transform << ",";
			str << std::setw(8) << v[id+14].transform << ",";
			str << std::setw(8) << v[id+15].transform << ",";
			str << std::setw(8) << v[id+16].transform << ",";
			str << std::setw(8) << v[id+17].transform << ",";
			str << std::setw(8) << v[id+18].transform << ",";
			str << std::setw(8) << v[id+19].transform << ",";
			str << std::setw(8) << v[id+20].transform << ",";
			str << std::setw(8) << v[id+21].transform << ",";
			str << std::setw(8) << v[id+22].transform << ",";
			str << std::setw(8) << v[id+23].transform << std::endl;

			return str.str();
		}
	};

	/**
	 * Scenegraph [Block 25]
	 */

	struct Scenegraph {
		enum { SIZE = 88 };

		std::string name;
		s32 nextRecord;
		s32 childRecord;
		s32 sgObjectIndex;
		s32 sgObjectVisIndex;
		s32 sgObjectTransIndex;
		s32 joint;

		void read (const std::vector<u8>& buffer, int offset) {
			name = getString(buffer, offset + 0, 64);
			nextRecord = getS32(buffer, offset + 64);
			childRecord = getS32(buffer, offset + 68);
			sgObjectIndex = getS32(buffer, offset + 72);
			sgObjectVisIndex = getS32(buffer, offset + 76);
			sgObjectTransIndex = getS32(buffer, offset + 80);
			joint = getS32(buffer, offset + 84);
		}

		static std::string headerString () {
			std::stringstream str;

			str << "      ID|                 Name|    Next|   Child|   ObjID|   VisID| TransID|   Joint|";

			return str.str();
		}

		std::string toString (int id) {
			std::stringstream str;

			str << std::setw(8) << id << ": ";
			str << std::setw(20) << std::left << name << std::right << ",";
			str << std::setw(8) << nextRecord << ",";
			str << std::setw(8) << childRecord << ",";
			str << std::setw(8) << sgObjectIndex << ",";
			str << std::setw(8) << sgObjectVisIndex << ",";
			str << std::setw(8) << sgObjectTransIndex << ",";
			str << std::setw(8) << joint;

			return str.str();
		}
	};

	/**
	 * Animation Index [Block 26]
	 */

	struct Animation {
		enum { SIZE = 64 };

		std::string name;
		u32 dataOffset;

		void read (const std::vector<u8>& buffer, u32 offset) {
			name = getString(buffer, offset, 60);
			dataOffset = getU32(buffer, offset + 60);
		}

		static std::string headerString () {
			std::stringstream str;

			str << "      ID|                         Name|    data|";

			return str.str();
		}

		std::string toString (u32 id) {
			std::stringstream str;

			str << std::setw(8) << id << ": ";
			str << std::setw(28) << std::left << name << std::right << ",";
			str << std::setw(8) << dataOffset;

			return str.str();
		}
	};

public:

	std::string filename;
	Header header;
	std::vector<SGObject> sgObjects;
	std::vector<Polygon> polygons;
	std::vector<Vertex> vertices;
	std::vector<PolyVertex> polyVertices;
	std::vector<Normal> normals;
	std::vector<PolyNormal> polyNormals;
	std::vector<Color> colors;
	std::vector<PolyColor> polyColors;
	std::vector<PolyTexCoord> polyTexCoords;
	std::vector<TexCoord> texCoords;
	std::vector<Block19> block19;
	std::vector<TextureMap> texMaps;
	std::vector<Texture> textures;
	std::vector<Mesh> meshes;
	std::vector<SGObjectVis> sgObjectVisibility;
	std::vector<SGObjectTrans> sgObjectTransforms;
	std::vector<Scenegraph> sgRecords;
	std::vector<Animation> animation;

public:

	bool LoadFile (const std::string& file) {
		filename = file;

		// Attempt to open file
		std::ifstream filestr;
		filestr.open(filename.c_str(), std::fstream::in | std::fstream::binary);
		if (filestr.fail() || !filestr.is_open()) {
			return false;
		}

		// Get size of file
		filestr.seekg(0, std::fstream::end);
		int filesize = filestr.tellg();
		filestr.seekg(0, std::fstream::beg);

		// Read file into buffer
		std::vector<u8> buffer(filesize);
		filestr.read((char*)&buffer[0], filesize);
		filestr.close();

		std::cout << "File Size: " << filesize << " bytes" << std::endl;

		// Parse file into data structures
		header.read(buffer, 0);

		sgObjects.resize(header.numBlocks[SGOBlock]);
		for (u32 i = 0; i < header.numBlocks[SGOBlock]; i++) {
			sgObjects[i].read(buffer, header.blockOffset[SGOBlock] + i * SGObject::SIZE);
		}

		polygons.resize(header.numBlocks[PBlock]);
		for (u32 i = 0; i < header.numBlocks[PBlock]; i++) {
			polygons[i].read(buffer, header.blockOffset[PBlock] + i * Polygon::SIZE);
		}

		vertices.resize(header.numBlocks[VBlock]);
		for (u32 i = 0; i < header.numBlocks[VBlock]; i++) {
			vertices[i].read(buffer, header.blockOffset[VBlock] + i * Vertex::SIZE);
		}

		polyVertices.resize(header.numBlocks[PVBlock]);
		for (u32 i = 0; i < header.numBlocks[PVBlock]; i++) {
			polyVertices[i].read(buffer, header.blockOffset[PVBlock] + i * PolyVertex::SIZE);
		}

		normals.resize(header.numBlocks[NBlock]);
		for (u32 i = 0; i < header.numBlocks[NBlock]; i++) {
			normals[i].read(buffer, header.blockOffset[NBlock] + i * Normal::SIZE);
		}

		polyNormals.resize(header.numBlocks[PNBlock]);
		for (u32 i = 0; i < header.numBlocks[PNBlock]; i++) {
			polyNormals[i].read(buffer, header.blockOffset[PNBlock] + i * PolyNormal::SIZE);
		}

		colors.resize(header.numBlocks[CBlock]);
		for (u32 i = 0; i < header.numBlocks[CBlock]; i++) {
			colors[i].read(buffer, header.blockOffset[CBlock] + i * Color::SIZE);
		}

		polyColors.resize(header.numBlocks[PCBlock]);
		for (u32 i = 0; i < header.numBlocks[PCBlock]; i++) {
			polyColors[i].read(buffer, header.blockOffset[PCBlock] + i * PolyColor::SIZE);
		}

		polyTexCoords.resize(header.numBlocks[PTCBlock]);
		for (u32 i = 0; i < header.numBlocks[PTCBlock]; i++) {
			polyTexCoords[i].read(buffer, header.blockOffset[PTCBlock] + i * PolyTexCoord::SIZE);
		}

		texCoords.resize(header.numBlocks[TCBlock]);
		for (u32 i = 0; i < header.numBlocks[TCBlock]; i++) {
			texCoords[i].read(buffer, header.blockOffset[TCBlock] + i * TexCoord::SIZE);
		}

		block19.resize(header.numBlocks[BLOCK19]);
		for (u32 i = 0; i < header.numBlocks[BLOCK19]; i++) {
			block19[i].read(buffer, header.blockOffset[BLOCK19] + i * Block19::SIZE);
		}

		texMaps.resize(header.numBlocks[TMBlock]);
		for (u32 i = 0; i < header.numBlocks[TMBlock]; i++) {
			texMaps[i].read(buffer, header.blockOffset[TMBlock] + i * TextureMap::SIZE);
		}

		textures.resize(header.numBlocks[TBlock]);
		for (u32 i = 0; i < header.numBlocks[TBlock]; i++) {
			textures[i].read(buffer, header.blockOffset[TBlock] + i * Texture::SIZE);
		}

		meshes.resize(header.numBlocks[MBlock]);
		for (u32 i = 0; i < header.numBlocks[MBlock]; i++) {
			meshes[i].read(buffer, header.blockOffset[MBlock] + i * Mesh::SIZE);
		}

		sgObjectVisibility.resize(header.numBlocks[SGOVBlock]);
		for (u32 i = 0; i < header.numBlocks[SGOVBlock]; i++) {
			sgObjectVisibility[i].read(buffer, header.blockOffset[SGOVBlock] + i * SGObjectVis::SIZE);
		}

		sgObjectTransforms.resize(header.numBlocks[SGOTBlock]);
		for (u32 i = 0; i < header.numBlocks[SGOTBlock]; i++) {
			sgObjectTransforms[i].read(buffer, header.blockOffset[SGOTBlock] + i * SGObjectTrans::SIZE);
		}

		sgRecords.resize(header.numBlocks[SGBlock]);
		for (u32 i = 0; i < header.numBlocks[SGBlock]; i++) {
			sgRecords[i].read(buffer, header.blockOffset[SGBlock] + i * Scenegraph::SIZE);
		}

		animation.resize(header.numBlocks[ABlock]);
		for (u32 i = 0; i < header.numBlocks[ABlock]; i++) {
			animation[i].read(buffer, header.blockOffset[ABlock] + i * Animation::SIZE);
		}

		return true;
	}

	bool WriteInfoFile (const std::string& outfile, const TPL& tpl) {

		// Attempt to open file for output
		std::ofstream filestr;
		filestr.open(outfile.c_str(), std::fstream::out | std::fstream::trunc);
		if (filestr.fail() || !filestr.is_open()) {
			return false;
		}

		// Write out data
		filestr << std::left << "Block 1: Header" << std::endl;
		filestr << std::setw(120) << std::setfill('-') << '-' << std::endl;
		filestr << std::setfill(' ') << std::endl;
		filestr << header.toString() << std::endl << std::endl;

		filestr << blockHeader("Block 25: Scene Graph", Scenegraph::headerString());
		for (unsigned int i = 0; i < sgRecords.size(); i++) {
			filestr << sgRecords[i].toString(i) << std::endl;
		}
		filestr << std::endl << std::endl;

		filestr << blockHeader("Block 23: Scene Graph Object Visibility", SGObjectVis::headerString());
		for (unsigned int i = 0; i < sgObjectVisibility.size(); i++) {
			filestr << sgObjectVisibility[i].toString(i) << std::endl;
		}
		filestr << std::endl << std::endl;

		filestr << blockHeader("Block 24: Scene Graph Object Transformation", SGObjectTrans::headerString());
		for (unsigned int i = 0; i < sgObjectTransforms.size(); i += 24) {
			filestr << SGObjectTrans::toString(sgObjectTransforms, i) << std::endl;
		}
		filestr << std::endl << std::endl;

		filestr << blockHeader("Block 2: Scene Graph Object Geometry", SGObject::headerString());
		for (unsigned int i = 0; i < sgObjects.size(); i++) {
			filestr << sgObjects[i].toString(i) << std::endl;
		}
		filestr << std::endl << std::endl;

		filestr << blockHeader("Block 22: Mesh", Mesh::headerString());
		for (unsigned int i = 0; i < meshes.size(); i++) {
			filestr << meshes[i].toString(i) << std::endl;
		}
		filestr << std::endl << std::endl;

		filestr << blockHeader("Block 19: Unknown (Maybe Texture Related)", Block19::headerString());
		for (unsigned int i = 0; i < block19.size(); i++) {
			filestr << block19[i].toString(i) << std::endl;
		}
		filestr << std::endl << std::endl;

		filestr << blockHeader("Block 20: Texture Map", TextureMap::headerString());
		for (unsigned int i = 0; i < texMaps.size(); i++) {
			filestr << texMaps[i].toString(i) << std::endl;
		}
		filestr << std::endl << std::endl;

		filestr << blockHeader("Block 21: Textures", Texture::headerString());
		for (unsigned int i = 0; i < textures.size(); i++) {
			filestr << textures[i].toString(tpl, i) << std::endl;
		}
		filestr << std::endl << std::endl;

		filestr << blockHeader("Block 3 - 10, 18: Polygons & Attributes", Polygon::headerString());
		for (unsigned int i = 0; i < polygons.size(); i++) {
			filestr << polygons[i].toString(i) << std::endl;
		}
		filestr << std::endl << std::endl;

		filestr << blockHeader("Block 4, 5: Vertex Coordinates", PolyVertex::headerString());
		for (unsigned int i = 0; i < polyVertices.size(); i++) {
			filestr << polyVertices[i].toString(vertices, i) << std::endl;
		}
		filestr << std::endl << std::endl;

		filestr << blockHeader("Block 6, 7: Vertex Normals", PolyVertex::headerString());
		for (unsigned int i = 0; i < polyNormals.size(); i++) {
			filestr << polyNormals[i].toString(normals, i) << std::endl;
		}
		filestr << std::endl << std::endl;

		filestr << blockHeader("Block 8, 9: Vertex Colors", PolyColor::headerString());
		for (unsigned int i = 0; i < polyColors.size(); i++) {
			filestr << polyColors[i].toString(colors, i) << std::endl;
		}
		filestr << std::endl << std::endl;

		filestr << blockHeader("Block 10, 18: Vertex Texture Coordinates", PolyTexCoord::headerString());
		for (unsigned int i = 0; i < polyTexCoords.size(); i++) {
			filestr << polyTexCoords[i].toString(texCoords, i) << std::endl;
		}
		filestr << std::endl << std::endl;

		filestr << blockHeader("Block 26: Animation Index", Animation::headerString());
		for (unsigned int i = 0; i < animation.size(); i++) {
			filestr << animation[i].toString(i) << std::endl;
		}
		filestr << std::endl << std::endl;

		filestr.flush();
		filestr.close();

		return true;
	}

	std::string blockHeader (const std::string& title, const std::string& legend) {
		std::stringstream str;

		str << std::left << title << std::endl;
		str << std::setw(120) << std::setfill('-') << '-' << std::endl;
		str << std::right << std::setfill(' ') << legend << std::endl;
		str << std::setw(120) << std::setfill('-') << '-' << std::endl;
		str << std::setfill(' ') << std::endl;

		return str.str();
	}
};

#endif /* PMMODEL_H_ */
