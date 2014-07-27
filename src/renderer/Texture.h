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

#ifndef GFX_TEXTURE_H_
#define GFX_TEXTURE_H_

#include <OpenGL/gl.h>

#include "TextureData.h"

namespace gfx {

	class Texture {
	protected:

		TextureData _texData;

		GLuint _texName;
		GLuint _target;
		GLuint _env_mode;

		static GLuint _global_env_mode;

	public:

		Texture (GLuint target, const TextureData& texdata)
			: _texData(texdata), _texName(0), _target(target), _env_mode(GL_MODULATE) {

			glGenTextures(1, &_texName);
			glBindTexture(target, _texName);

			glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTexImage2D(target, _texData._mipmapLevel, _texData._internalFormat,
					_texData._width, _texData._height, _texData._border,
					_texData._pixelFormat, _texData._pixelType, &_texData._texels->_data[0]);
		}

		void bind () {
			glBindTexture(_target, _texName);

			if (_env_mode != _global_env_mode) {
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, _env_mode);
				_global_env_mode = _env_mode;
			}
		}

		void disable () {
			glDisable(_target);
		}

		void enable () {
			glEnable(_target);
		}

		GLuint getTarget () {
			return _target;
		}

		GLuint getTextureObject () {
			return _texName;
		}

		void setTexEnvMode (GLint value) {
			_env_mode = value;
		}

		void setTexParameterf (GLenum param, GLfloat value) {
			glTexParameterf(_target, param, value);
		}

		void setTexParameteri (GLenum param, GLint value) {
			glTexParameteri(_target, param, value);
		}

		void updateImage (const TextureData& texData) {
			_texData = texData;
		}

	};

	GLuint Texture::_global_env_mode = GL_MODULATE;

}

#endif /* GFX_TEXTURE_H_ */
