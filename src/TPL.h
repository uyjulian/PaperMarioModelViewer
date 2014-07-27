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

#ifndef TPL_H_
#define TPL_H_

#include <string>
#include <vector>
#include <fstream>
#include "common.h"
#include "TexCodec.h"

class TPL {
public:

	struct TPLHeader {
		u32 magic;
		u32 nTextures;
		u32 szHeader;
	};

	struct TPLTexDef {
		u32 headerOffset;
		u32 paletteOffset;
	};

	struct TPLTexHeader {
		u16 height;
		u16 width;
		u32 format;
		u32 dataOffset;
		u32 wrap_s;
		u32 wrap_t;
		u32 minFilter;
		u32 magFilter;
		f32 lodBias;
		u8 edgeLod;
		u8 minLod;
		u8 maxLod;
		u8 unpacked;
	};

	struct TPLPalHeader {
		u16 nItems;
		u8 unpacked;
		u8 pad;
		u32 format;
		u32 dataOffset;
	};

	struct TPLTexture {
		TPLTexHeader texHeader;
		TPLPalHeader palHeader;
		Image tex;

		TPLTexture () { }
	};

public:

	std::string _filename;
	TPLHeader _header;
	std::vector<TPLTexture> _textures;

public:

	bool LoadFile (const std::string& filename) {
		_filename = filename;
		std::fstream filestr;

		filestr.open(filename.c_str(), std::fstream::in | std::fstream::binary);
		if (filestr.fail()) {
			return false;
		}

		// Get file size;
		filestr.seekg(0, std::fstream::end);
		int filesize = filestr.tellg();
		filestr.seekg(0, std::fstream::beg);

		// Read entire file
		std::vector<u8> buffer(filesize);
		filestr.read((char*)&buffer[0], filesize);

		filestr.close();

		// Read header
		//filestr.read((char*)&_header, sizeof(TPLHeader));
		_header.magic = getU32(buffer, 0);
		_header.nTextures = getU32(buffer, 4);
		_header.szHeader = getU32(buffer, 8);

		if (_header.magic != 0x0020AF30) {
			return false;
		}

		_textures.resize(_header.nTextures);

		// Get list of defined textures
		std::vector<TPLTexDef> defs(_header.nTextures);
		for (unsigned int i = 0; i < _header.nTextures; i++) {
			int dataIndex = _header.szHeader + (i * 8);
			defs[i].headerOffset = getU32(buffer, dataIndex);
			defs[i].paletteOffset = getU32(buffer, dataIndex + 4);
		}

		// Get textures
		for (unsigned int i = 0; i < _header.nTextures; i++) {
			_textures[i].texHeader.height = getU16(buffer, defs[i].headerOffset + 0);
			_textures[i].texHeader.width = getU16(buffer, defs[i].headerOffset + 2);
			_textures[i].texHeader.format = getU32(buffer, defs[i].headerOffset + 4);
			_textures[i].texHeader.dataOffset = getU32(buffer, defs[i].headerOffset + 8);
			_textures[i].texHeader.wrap_s = getU32(buffer, defs[i].headerOffset + 12);
			_textures[i].texHeader.wrap_t = getU32(buffer, defs[i].headerOffset + 16);
			_textures[i].texHeader.minFilter = getU32(buffer, defs[i].headerOffset + 20);
			_textures[i].texHeader.magFilter = getU32(buffer, defs[i].headerOffset + 24);
			_textures[i].texHeader.lodBias = getF32(buffer, defs[i].headerOffset + 28);
			_textures[i].texHeader.edgeLod = buffer[defs[i].headerOffset + 32];
			_textures[i].texHeader.minLod = buffer[defs[i].headerOffset + 33];
			_textures[i].texHeader.maxLod = buffer[defs[i].headerOffset + 34];
			_textures[i].texHeader.unpacked = buffer[defs[i].headerOffset + 35];

			if (defs[i].paletteOffset != 0) {
				_textures[i].palHeader.nItems = getU16(buffer, defs[i].paletteOffset + 0);
				_textures[i].palHeader.unpacked = buffer[defs[i].paletteOffset + 4];
				_textures[i].palHeader.pad = buffer[defs[i].paletteOffset + 5];
				_textures[i].palHeader.format = getU32(buffer, defs[i].paletteOffset + 6);
				_textures[i].palHeader.dataOffset = getU32(buffer, defs[i].paletteOffset + 10);
			}

			TexCodec codec;
			codec.texWidth = _textures[i].texHeader.width;
			codec.texHeight = _textures[i].texHeader.height;
			codec.dataOffset = _textures[i].palHeader.dataOffset;
			codec.cacheLineSize = 32;
			codec.cacheLinesPerTile = 1;

			switch (_textures[i].texHeader.format) {
				case 0:	codec.type = TexCodec::I4; break;
				case 1: codec.type = TexCodec::I8; break;
				case 2: codec.type = TexCodec::IA4; break;
				case 3: codec.type = TexCodec::IA8; break;
				case 4: codec.type = TexCodec::RGB565; break;
				case 5: codec.type = TexCodec::RGB5A3; break;
				case 6: codec.type = TexCodec::RGBA8;
					codec.cacheLinesPerTile = 2; break;
				case 14: codec.type = TexCodec::CMPR; break;
				default: std::cout << "Unsupported TPL format: " << codec.type << std::endl;
					return false;
			}

			if (_textures[i].texHeader.format == 14) {
				_textures[i].tex = codec.DecodeCMPR(buffer, _textures[i].texHeader.dataOffset);
			}
			else {
				_textures[i].tex = codec.DecodeTiled(buffer, _textures[i].texHeader.dataOffset);
			}

		}

		return true;
	}
};

#endif /* TPL_H_ */
