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

#ifndef GFX_TEXTUREDATA_H_
#define GFX_TEXTUREDATA_H_

#include <OpenGL/gl.h>

#include "../Image.h"

namespace gfx {

	class Texture;

	class TextureData {
	protected:

		friend class Texture;

		int _internalFormat;
		int _width;
		int _height;
		int _border;
		int _pixelFormat;
		int _pixelType;
		int _mipmapLevel;

		const Image* _texels;

	public:

		TextureData (int internalFormat, int width, int height, int border, int pixelFormat, int pixelType, int mipmap, const Image& texels)
			: _internalFormat(internalFormat), _width(width), _height(height), _border(border),
			  _pixelFormat(pixelFormat), _pixelType(pixelType), _mipmapLevel(mipmap), _texels(&texels) {
		}

		TextureData (int internalFormat, int pixelFormat, int mipmap, const Image& texels)
			: _internalFormat(internalFormat), _width(texels.getWidth()), _height(texels.getHeight()),
			  _border(0), _pixelFormat(pixelFormat), _pixelType(GL_UNSIGNED_BYTE), _mipmapLevel(mipmap),
			  _texels(&texels) {
		}

	};

}

#endif /* GFX_TEXTUREDATA_H_ */
