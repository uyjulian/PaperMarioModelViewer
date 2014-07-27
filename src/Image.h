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

#ifndef IMAGE_H_
#define IMAGE_H_

#include <vector>
#include <iostream>
#include "Color.h"

class Image {
public:

	enum ImageType {
		LUM4, LUM8,
		LUM4_A4, LUM8_A8,
		RGB8, RGBA8,
	};

	std::vector<u8> _data;

protected:

	ImageType _type;
	int _width;
	int _height;

public:

	Image ()
		: _type(RGBA8), _width(0), _height(0) {
	}

	Image (int width, int height, ImageType imageType) {
		setup(width, height, imageType);
	}

	~Image () {

	}

	int getHeight () const {
		return _height;
	}

	int getWidth () const {
		return _width;
	}

	void setup (int width, int height, ImageType imageType) {
		_width = width;
		_height = height;
		_type = imageType;

		int bpp = typeBPP(_type);
		_data.resize(((_width * _height) >> 1) * ((bpp << 1) / 8));
	}

	void setPixel (int x, int y, const Color8& c) {
		switch (_type) {
			case LUM4: setPixel_LUM4(x, y, c); break;
			case LUM8: setPixel_LUM8(x, y, c); break;
			case LUM4_A4: setPixel_LUM4_A4(x, y, c); break;
			case LUM8_A8: setPixel_LUM8_A8(x, y, c); break;
			case RGB8: setPixel_RGB8(x, y, c); break;
			case RGBA8: setPixel_RGBA8(x, y, c); break;
		}
	}

	void setPixelBlock (int x, int y, int w, int h, const std::vector<Color8>& data, int offset, int scansize) {
		unsigned int vecsz = offset + (h * scansize);
		if (data.capacity() < vecsz) {
			return;
		}

		// Clip dimentions
		if (x + w > _width) {
			w = _width - x;
		}
		if (y + h > _height) {
			h = _height - y;
		}

		// Set data
		for (int j = y; j < y + h; j++) {
			for (int i = x; i < x + w; i++) {
				int srcIndex = offset + ((j - y) * scansize) + (i - x);
				setPixel(i, j, data[srcIndex]);
			}
		}
	}

protected:

	int typeBPP (ImageType type) const {
		switch (type) {
			case LUM4: return 4;
			case LUM8: return 8;
			case LUM4_A4: return 8;
			case LUM8_A8: return 16;
			case RGB8: return 24;
			case RGBA8: return 32;
			default: return 0;
		}
	}

	void setPixel_LUM4 (int x, int y, const Color8& c) {
		int pixelsIn = (y * _width) + x;
		int addr = pixelsIn >> 1;
		if (pixelsIn % 2 == 0) {
			_data[addr] = (_data[addr] & 0x0F) | (c.luminance() & 0xF0);
		}
		else {
			_data[addr] = (_data[addr] & 0xF0) | (c.luminance() >> 4);
		}
	}

	void setPixel_LUM8 (int x, int y, const Color8& c) {
		int addr = (y * _width) + x;
		_data[addr] = c.luminance();
	}

	void setPixel_LUM4_A4 (int x, int y, const Color8& c) {
		int addr = (y * _width) + x;
		_data[addr] = (c.luminance() & 0xF0) | (c.alpha() >> 4);
	}

	void setPixel_LUM8_A8 (int x, int y, const Color8& c) {
		int addr = 2 * ((y * _width) + x);
		_data[addr] = c.luminance();
		_data[addr+1] = c.alpha();
	}

	void setPixel_RGB8 (int x, int y, const Color8& c) {
		int addr = 3 * ((y * _width) + x);
		_data[addr] = c.red();
		_data[addr+1] = c.green();
		_data[addr+2] = c.blue();
	}

	void setPixel_RGBA8 (int x, int y, const Color8& c) {
		int addr = 4 * ((y * _width) + x);
		_data[addr] = (c.red());
		_data[addr+1] = (c.green());
		_data[addr+2] = (c.blue());
		_data[addr+3] = c.alpha();
	}
};

#endif /* IMAGE_H_ */
