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

#ifndef PMWORLD_H_
#define PMWORLD_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <stack>
#include <vector>
#include <map>
#include "common.h"

class PMWorld {
public:

	struct Header {
		enum { SIZE = 32 };

		u32 addrMasterIndex;
		u32 indexCount;
		u32 tableIndexCount;

		void read (const std::vector<u8>& buffer, u32 offset) {
			addrMasterIndex = getU32(buffer, offset + 4);
			indexCount = getU32(buffer, offset + 8);
			tableIndexCount = getU32(buffer, offset + 12);
		}

		std::string toString () const {
			std::stringstream str;

			str << std::showbase;
			str << std::setw(16) << std::left << "Master Index:";
			str << std::setw(8) << std::right << std::hex << addrMasterIndex << std::endl;
			str << std::setw(16) << std::left << "Index Count:";
			str << std::setw(8) << std::right << std::hex << indexCount << std::endl;
			str << std::setw(16) << std::left << "Table Count:";
			str << std::setw(8) << std::right << std::dec << tableIndexCount << std::endl;

			return str.str();
		}
	};

	struct InfoTable {

		std::string name;
		u32 addr;

		std::string verString;
		u32 sgRootNode;
		std::string str1;
		std::string str2;
		std::string dateString;

		void read (const std::vector<u8>& buffer, u32 offset) {
			addr = offset;

			u32 addrVerString = getU32(buffer, offset + 0);
			sgRootNode = getU32(buffer, offset + 4);
			u32 addrStr1 = getU32(buffer, offset + 8);
			u32 addrStr2 = getU32(buffer, offset + 12);
			u32 addrDateString = getU32(buffer, offset + 16);

			verString = getString(buffer, addrVerString, 64);
			str1 = getString(buffer, addrStr1, 64);
			str2 = getString(buffer, addrStr2, 64);
			dateString = getString(buffer, addrDateString, 64);
		}

		std::string toString () const {
			std::stringstream str;

			str << std::showbase;
			str << std::setw(16) << std::left << "Version:" << verString << std::endl;
			str << std::setw(16) << std::left << "String 1:" << str1 << std::endl;
			str << std::setw(16) << std::left << "String 2:" << str2 << std::endl;
			str << std::setw(16) << std::left << "Date:" << dateString << std::endl;
			str << std::setw(16) << std::left << "SG Root Node:";
			str << std::setw(8) << std::right << std::hex << sgRootNode << std::endl;

			return str.str();
		}
	};

	struct MaterialTable {

		struct MatNamePair {
			std::string name;
			u32 addr;
		};

		std::string name;
		u32 addr;

		u32 materialCount;
		std::vector<MatNamePair> materials;

		void read (const std::vector<u8>& buffer, u32 offset) {
			addr = offset;

			materialCount = getU32(buffer, offset + 0);

			materials.resize(materialCount);
			for (u32 i = 0; i < materialCount; i++) {
				u32 nameAddr = getU32(buffer, offset + 4 + (i * 8));

				materials[i].name = getString(buffer, nameAddr, 64);
				materials[i].addr = getU32(buffer, offset + 8 + (i * 8));
			}
		}

		std::string toString () const {
			std::stringstream str;

			for (u32 i = 0; i < materials.size(); i++) {
				str << std::setw(8) << std::dec << i << ": ";
				str << std::setw(8) << std::hex << materials[i].addr << ", ";
				str << std::left << materials[i].name << std::right << std::endl;
			}

			return str.str();
		}

	};

	struct TextureTable {

		std::string name;
		u32 addr;

		u32 textureCount;
		std::vector<std::string> textureNames;

		void read(const std::vector<u8>& buffer, u32 offset) {
			addr = offset;

			textureCount = getU32(buffer, offset + 0);

			textureNames.resize(textureCount);
			for (u32 i = 0; i < textureCount; i++) {
				u32 nameAddr = getU32(buffer, offset + 4 + (i * 4));
				textureNames[i] = getString(buffer, nameAddr, 64);
			}
		}

		std::string toString () const {
			std::stringstream str;

			for (u32 i = 0; i < textureCount; i++) {
				str << std::setw(8) << std::dec << i << ": ";
				str << std::left << textureNames[i] << std::right << std::endl;
			}

			return str.str();
		}
	};

	struct VCDTable {

		std::string name;
		u32 addr;

		u32 addrVertex;
		u32 u0x04;
		u32 u0x08;
		u32 addrColor;

		void read (const std::vector<u8>& buffer, u32 offset) {
			addr = offset;

			addrVertex = getU32(buffer, offset + 0);
			u0x04 = getU32(buffer, offset + 4);
			u0x08 = getU32(buffer, offset + 8);
			addrColor = getU32(buffer, offset + 12);
		}

		std::string toString() const {
			std::stringstream str;

			str << std::showbase;
			str << std::setw(16) << std::left << "Vertex Addr:";
			str << std::setw(8) << std::right << std::hex << addrVertex << std::endl;
			str << std::setw(16) << std::left << "u0x04:";
			str << std::setw(8) << std::right << std::hex << u0x04 << std::endl;
			str << std::setw(16) << std::left << "u0x08:";
			str << std::setw(8) << std::right << std::hex << u0x08 << std::endl;
			str << std::setw(16) << std::left << "Color Addr:";
			str << std::setw(8) << std::right << std::hex << addrColor << std::endl;

			return str.str();
		}
	};

	struct Material {

		u32 addr;

		std::string name;
		u8 r;
		u8 g;
		u8 b;
		u8 a;
		u8 p0;
		u8 p1;
		u8 p2;
		u8 p3;
		u32 addrTexture;

		s32 textureID;

		void read (const std::vector<u8>& buffer, u32 offset) {
			addr = offset;

			u32 nameAddr = getU32(buffer, offset + 0);
			r = buffer[offset + 4];
			g = buffer[offset + 5];
			b = buffer[offset + 6];
			a = buffer[offset + 7];
			p0 = buffer[offset + 8];
			p1 = buffer[offset + 9];
			p2 = buffer[offset + 10];
			p3 = buffer[offset + 11];
			addrTexture = getU32(buffer, offset + 12);

			name = getString(buffer, nameAddr, 64);

			s32 subTexAddr = getU32(buffer, addrTexture);
			textureID = (addrTexture > 0) ? (subTexAddr - 0x14) / 16 : -1;
		}

		static std::string headerString () {
			std::stringstream str;

			str << " Address|                        Name|" << std::endl;
			str << "        |    Color|   Params| AddrTex|";

			return str.str();
		}

		std::string toString () {
			std::stringstream str;

			u32 color32 = (r << 24) | (g << 16) | (b << 8) | a;
			u32 param32 = (p0 << 24) | (p1 << 16) | (p2 << 8) | p3;

			str << std::hex << std::uppercase;
			str << std::setw(8) << addr << ": ";
			str << std::left << name << std::right << std::endl;
			str << std::setw(10) << " ";
			str << std::setw(8) << std::setfill('0') << color32 << ", ";
			str << std::setw(8) << std::setfill('0') << param32 << ",";
			str << std::setw(8) << std::setfill(' ') << addrTexture << std::endl;

			return str.str();
		}
	};

	struct Vertex {

		f32 x;
		f32 y;
		f32 z;

		void read (const std::vector<u8>& buffer, u32 offset) {
			float scale = 0.01f; // Arbitrary scale value
			x = getS16(buffer, offset + 0) * scale;
			y = getS16(buffer, offset + 2) * scale;
			z = getS16(buffer, offset + 4) * scale;
		}
	};

	struct PolyVertex {
		u16 vertexID;
		u16 colorID;
		u16 texCoordID;

		void read (const std::vector<u8>& buffer, u32 offset, u32 mask) {
			vertexID = getU16(buffer, offset);
			texCoordID = getU16(buffer, offset + 2);
		}
	};

	struct Polygon {

		u32 addr;

		u16 u0x00;
		u8 vertexCount;
		std::vector<PolyVertex> vertices;

		void read (const std::vector<u8>& buffer, u32 offset, u32 mask) {
			addr = offset;

			u0x00 = getU16(buffer, offset + 0);
			vertexCount = buffer[offset + 2];

			u16 bitcount = 0;
			for (int i = 0; i < 8; i++) {
				if (mask & (1 << i)) {
					bitcount++;
				}
			}

			vertices.resize(vertexCount);
			for (int i = 0; i < vertexCount; i++) {
				vertices[i].read(buffer, offset + 3 + (i * 2 * bitcount), mask);
			}
		}
	};

	struct MeshData {
		u32 addr;

		u32 addrData;
		u32 length;
		std::vector<u8> data;

		void read(const std::vector<u8>& buffer, u32 offset) {
			addr = offset;

			addrData = getU32(buffer, offset + 0);
			length = getU32(buffer, offset + 4);

			// Raw data .. for now
			data.resize(length);
			for (unsigned int i = 0; i < length; i++) {
				data[i] = buffer[addrData + i];
			}
		}

		std::string toString () const {
			std::stringstream str;

			str << std::uppercase;
			str << std::setw(17) << std::hex << addrData << ": ";
			for (unsigned int i = 0; i < data.size(); i += 32) {
				if (i > 0) {
					str << std::setw(19) << std::setfill(' ') << " ";
				}
				str << std::setfill('0');
				for (unsigned int j = 0; j < 32; j++) {
					if (i + j >= data.size()) break;
					str << std::setw(2) << std::hex << (int)data[i + j] << " ";
					if (j % 8 == 7) {
						str << "  ";
					}
				}
				str << std::endl;
			}

			return str.str();
		}
	};

	struct Mesh {

		u32 id;
		u32 addr;

		u32 u0x00;
		u32 polyCount;
		u32 elementMask;
		u32 u0x0C;

		std::vector<MeshData> data;
		std::vector<Polygon> polygons;

		void read (const std::vector<u8>& buffer, u32 offset) {
			id = 0;
			addr = offset;

			u0x00 = getU32(buffer, offset + 0);
			polyCount = getU32(buffer, offset + 4);
			elementMask = getU32(buffer, offset + 8);
			u0x0C = getU32(buffer, offset + 12);

			data.resize(polyCount);
			polygons.resize(polyCount);

			u32 polyOffset = offset + 16;
			for (u32 i = 0; i < polyCount; i++) {
				u32 polyAddr = getU32(buffer, polyOffset);
				//u32 polyDataLen = getU32(buffer, polyOffset + 4);

				polygons[i].read(buffer, polyAddr, elementMask);
				polyOffset += 8;
			}
		}

		static std::string headerString () {
			std::stringstream str;

			str << "      ID|  (Addr)|      u00|     u04|     u08|     u0C|";

			return str.str();
		}

		std::string toString() const {
			std::stringstream str;

			str << std::uppercase;
			str << std::setw(8) << id << " (" << std::setw(6) << std::hex << addr << "): ";
			str << std::setw(8) << std::hex << u0x00 << ",";
			str << std::setw(8) << std::hex << polyCount << ",";
			str << std::setw(8) << std::hex << elementMask << ",";
			str << std::setw(8) << std::hex << u0x0C << std::endl;

			for (unsigned int i = 0; i < data.size(); i++) {
				str << data[i].toString() << std::endl;
			}
			str << std::endl;

			return str.str();
		}
	};

	struct Scenegraph {
		enum { SIZE = 104 };

		u32 id;
		u32 addr;

		std::string str1;
		std::string str2;
		u32 addrParent;
		u32 addrChild;
		u32 addrNext;
		u32 addrPrev;
		f32 u0x18;
		f32 u0x1C;
		f32 u0x20;
		f32 u0x24;
		f32 u0x28;
		f32 u0x2C;
		f32 u0x30;
		f32 u0x34;
		f32 u0x38;
		f32 u0x3C;
		f32 u0x40;
		f32 u0x44;
		f32 u0x48;
		f32 u0x4C;
		f32 u0x50;
		u32 u0x54;
		u32 u0x58;
		u32 u0x5C;
		u32 u0x60;
		u32 u0x64;

		std::vector<Mesh> meshes;
		u32 parentID;
		u32 childID;
		u32 nextID;
		u32 prevID;

		void read (const std::vector<u8>& buffer, int offset) {
			id = 0;
			addr = offset;

			u32 addrStr1 = getU32(buffer, offset + 0);
			u32 addrStr2 = getU32(buffer, offset + 4);
			addrParent = getU32(buffer, offset + 8);
			addrChild = getU32(buffer, offset + 12);
			addrNext = getU32(buffer, offset + 16);
			addrPrev = getU32(buffer, offset + 20);
			u0x18 = getF32(buffer, offset + 24);
			u0x1C = getF32(buffer, offset + 28);
			u0x20 = getF32(buffer, offset + 32);
			u0x24 = getF32(buffer, offset + 36);
			u0x28 = getF32(buffer, offset + 40);
			u0x2C = getF32(buffer, offset + 44);
			u0x30 = getF32(buffer, offset + 48);
			u0x34 = getF32(buffer, offset + 52);
			u0x38 = getF32(buffer, offset + 56);
			u0x3C = getF32(buffer, offset + 60);
			u0x40 = getF32(buffer, offset + 64);
			u0x44 = getF32(buffer, offset + 68);
			u0x48 = getF32(buffer, offset + 72);
			u0x4C = getF32(buffer, offset + 76);
			u0x50 = getF32(buffer, offset + 80);
			u0x54 = getU32(buffer, offset + 84);
			u0x58 = getU32(buffer, offset + 88);
			u0x5C = getU32(buffer, offset + 92);
			u0x60 = getU32(buffer, offset + 96);
			u0x64 = getU32(buffer, offset + 100);

			str1 = getString(buffer, addrStr1, 64);
			str2 = getString(buffer, addrStr2, 64);

			// Read mesh(es)
			meshes.resize(u0x5C);
			if (u0x5C != 0) {
				meshes[0].read(buffer, u0x64);

				// Mesh data
				for (unsigned int j = 0; j < meshes[0].polyCount; j++) {
					meshes[0].data[j].read(buffer, meshes[0].addr + 16 + (j * 8));
				}
			}
		}

		static void read (const std::vector<u8>& buffer, int offset, std::vector<Scenegraph>& sgrv, s32 parentID, s32 prevID) {
			Scenegraph sgNode;
			sgNode.read(buffer, offset);
			sgNode.id = sgrv.size();

			if (parentID != -1) {
				sgNode.parentID = parentID;
				sgrv[parentID].childID = sgNode.id;
			}

			if (prevID != -1) {
				sgNode.prevID = prevID;
				sgrv[prevID].nextID = sgNode.id;
			}

			sgrv.push_back(sgNode);

			if (sgNode.addrChild != 0) {
				Scenegraph::read(buffer, sgNode.addrChild, sgrv, sgNode.id, -1);
			}

			if (sgNode.addrNext != 0) {
				Scenegraph::read(buffer, sgNode.addrNext, sgrv, -1, sgNode.id);
			}
		}

		static std::string headerString () {
			std::stringstream str;

			str << "      ID|                          String1|                         String2|" << std::endl;
			str << "  (Addr)|  adrParent|  adrChild|   adrNext|   adrPrev|       u18|       u1C|       u20|" << std::endl;
			str << "        |        u24|       u28|       u2C|       u30|       u34|       u38|       u3C|       u40|       u44|" << std::endl;
			str << "        |        u48|       u4C|       u50|       u54|       u58|       u5C|       u60|       u64|" << std::endl;

			return str.str();
		}

		std::string toString () const {
			std::stringstream str;
			str.precision(3);

			str << std::uppercase;
			str << std::setw(8) << id << ": ";
			str << std::left << std::setw(32) << str1 << ", ";
			str << std::left << std::setw(31) << str2 << std::right << "," << std::endl;
			str << "(" << std::setw(6) << std::hex << addr << ")  ";
			str << std::setw(10) << std::hex << addrParent << ",";
			str << std::setw(10) << std::hex << addrChild << ",";
			str << std::setw(10) << std::hex << addrNext << ",";
			str << std::setw(10) << std::hex << addrPrev << ",";
			str << std::setw(10) << std::fixed << u0x18 << ",";
			str << std::setw(10) << std::fixed << u0x1C << ",";
			str << std::setw(10) << std::fixed << u0x20 << "," << std::endl;
			str << std::setw(10) << " ";
			str << std::setw(10) << std::fixed << u0x24 << ",";
			str << std::setw(10) << std::fixed << u0x28 << ",";
			str << std::setw(10) << std::fixed << u0x2C << ",";
			str << std::setw(10) << std::fixed << u0x30 << ",";
			str << std::setw(10) << std::fixed << u0x34 << ",";
			str << std::setw(10) << std::fixed << u0x38 << ",";
			str << std::setw(10) << std::fixed << u0x3C << ",";
			str << std::setw(10) << std::fixed << u0x40 << ",";
			str << std::setw(10) << std::fixed << u0x44 << "," << std::endl;
			str << std::setw(10) << " ";
			str << std::setw(10) << std::fixed << u0x48 << ",";
			str << std::setw(10) << std::fixed << u0x4C << ",";
			str << std::setw(10) << std::fixed << u0x50 << ",";
			str << std::setw(10) << std::hex << u0x54 << ",";
			str << std::setw(10) << std::hex << u0x58 << ",";
			str << std::setw(10) << std::hex << u0x5C << ",";
			str << std::setw(10) << std::hex << u0x60 << ",";
			str << std::setw(10) << std::hex << u0x64 << "," << std::endl;

			return str.str();
		}
	};

public:

	std::string filename;
	Header header;
	InfoTable infoTable;
	MaterialTable matTable;
	TextureTable texTable;
	VCDTable vcdTable;

	std::vector<Scenegraph> sgRecords;
	std::vector<Mesh> meshes;
	std::vector<Vertex> vertices;
	std::map<u32, Material> materials;

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
		int filesize = (int)filestr.tellg() - 32;
		filestr.seekg(0, std::fstream::beg);

		// Read first 32 bytes
		std::vector<u8> descriptor(32);
		filestr.read((char*)&descriptor[0], 32);

		// Read file into buffer
		std::vector<u8> buffer(filesize);
		filestr.read((char*)&buffer[0], filesize);
		filestr.close();

		std::cout << "File Size: " << filesize << " bytes" << std::endl;

		// Read Header
		header.read(descriptor, 0);

		// Read Tables
		u32 tableBase = header.addrMasterIndex + (4 * header.indexCount);
		u32 tableStringBase = tableBase + (header.tableIndexCount * 8);

		u32 infoAddr = getU32(buffer, tableBase + 24);
		u32 infoStringOffset = getU32(buffer, tableBase + 28);
		infoTable.read(buffer, infoAddr);
		infoTable.name = getString(buffer, tableStringBase + infoStringOffset, 64);

		u32 matAddr = getU32(buffer, tableBase + 40);
		u32 matStringOffset = getU32(buffer, tableBase + 44);
		matTable.read(buffer, matAddr);
		matTable.name = getString(buffer, tableStringBase + matStringOffset, 64);

		u32 texAddr = getU32(buffer, tableBase + 48);
		u32 texStringOffset = getU32(buffer, tableBase + 52);
		texTable.read(buffer, texAddr);
		texTable.name = getString(buffer, tableStringBase + texStringOffset, 64);

		u32 vcdAddr = getU32(buffer, tableBase + 56);
		u32 vcdStringOffset = getU32(buffer, tableBase + 60);
		vcdTable.read(buffer, vcdAddr);
		vcdTable.name = getString(buffer, tableStringBase + vcdStringOffset, 64);

		// Read Material Data
		for (u32 i = 0; i < matTable.materialCount; i++) {
			Material mat;
			mat.read(buffer, matTable.materials[i].addr);
			materials[matTable.materials[i].addr] = mat;
		}

		// Read VCD Data
		u32 vertexCount = getU32(buffer, vcdTable.addrVertex);
		vertices.resize(vertexCount);
		for (unsigned int i = 0; i < vertexCount; i++) {
			vertices[i].read(buffer, vcdTable.addrVertex + 4 + (i * 6));
		}

		// Read scenegraph
		Scenegraph::read(buffer, infoTable.sgRootNode, sgRecords, -1, -1);

		// Read meshes
		for (unsigned int i = 0; i < sgRecords.size(); i++) {
			if (sgRecords[i].u0x64 != 0) {
				Mesh mesh;
				mesh.read(buffer, sgRecords[i].u0x64);
				mesh.id = meshes.size();

				// Mesh data
				for (unsigned int j = 0; j < mesh.polyCount; j++) {
					mesh.data[j].read(buffer, mesh.addr + 16 + (j * 8));
				}

				meshes.push_back(mesh);
			}
		}

		return true;
	}

	bool WriteInfoFile (const std::string& outfile) {

		// Attempt to open file for output
		std::ofstream filestr;
		filestr.open(outfile.c_str(), std::fstream::out | std::fstream::trunc);
		if (filestr.fail() || !filestr.is_open()) {
			return false;
		}

		// Write out data
		filestr << std::left << "Header" << std::endl;
		filestr << std::setw(120) << std::setfill('-') << '-' << std::endl;
		filestr << std::setfill(' ') << std::endl;
		filestr << header.toString() << std::endl << std::endl;

		filestr << std::left << "Information Table" << std::endl;
		filestr << std::setw(120) << std::setfill('-') << '-' << std::endl;
		filestr << std::setfill(' ') << std::endl;
		filestr << infoTable.toString() << std::endl << std::endl;

		filestr << std::left << "Material Name Table" << std::endl;
		filestr << std::setw(120) << std::setfill('-') << '-' << std::endl;
		filestr << std::setfill(' ') << std::endl;
		filestr << matTable.toString() << std::endl << std::endl;

		filestr << std::left << "Texture Name Table" << std::endl;
		filestr << std::setw(120) << std::setfill('-') << '-' << std::endl;
		filestr << std::setfill(' ') << std::endl;
		filestr << texTable.toString() << std::endl << std::endl;

		filestr << std::left << "VCD Table" << std::endl;
		filestr << std::setw(120) << std::setfill('-') << '-' << std::endl;
		filestr << std::setfill(' ') << std::endl;
		filestr << vcdTable.toString() << std::endl << std::endl;

		filestr << blockHeader("Materials", Material::headerString());
		std::map<u32, Material>::iterator matIter;
		for (matIter = materials.begin(); matIter != materials.end(); matIter++) {
			filestr << matIter->second.toString() << std::endl;
		}

		filestr << blockHeader("Scene Graph", Scenegraph::headerString());
		for (unsigned int i = 0; i < sgRecords.size(); i++) {
			filestr << sgRecords[i].toString() << std::endl;
		}
		filestr << std::endl << std::endl;

		filestr << blockHeader("Meshes", Mesh::headerString());
		for (unsigned int i = 0; i < meshes.size(); i++) {
			filestr << meshes[i].toString() << std::endl;
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

#endif /* PMWORLD_H_ */
