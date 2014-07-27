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

#ifndef GFX_CAMERA_H_
#define GFX_CAMERA_H_

#include <OpenGL/gl.h>

#include "../vecmath/Vecmath.h"
#include "../vecmath/MatrixG3.h"
#include "../vecmath/MatrixG4.h"

namespace gfx {

	class Camera {
	protected:

		vmath::Vector3f eyePoint;
		vmath::Vector3f focusPoint;
		vmath::Vector3f focusAnchor;
		vmath::Vector3f up;

		vmath::Vector3f worldTranslate;
		vmath::Vector3f worldRotate;

		vmath::MatrixG4f cameraTransform;

	public:

		Camera ()
			: eyePoint(0.f, 0.f, 1.f),
			  focusPoint(0.f, 0.f, 0.f),
			  focusAnchor(0.f, 0.f, 0.f),
			  up(0.f, 1.f, 0.f),
			  worldTranslate(0.f, 0.f, 0.f),
			  worldRotate(0.f, 0.f, 0.f) {
		}

		const vmath::Vector3f& getEye () const {
			return eyePoint;
		}

		const vmath::Vector3f& getFocus () const {
			return focusPoint;
		}

		const vmath::Vector3f& getAnchor () const {
			return focusAnchor;
		}

		const vmath::Vector3f& getUp () const {
			return up;
		}

		const vmath::MatrixG4f& getCameraTransform () const {
			return cameraTransform;
		}

		void setEye (const vmath::Vector3f& v) {
			eyePoint.set(v);
		}

		void setEye (float x, float y, float z) {
			eyePoint.set(x, y, z);
		}

		void setFocus (const vmath::Vector3f& v) {
			focusPoint.set(v);
		}

		void setFocus (float x, float y, float z) {
			focusPoint.set(x, y, z);
		}

		void setAnchor (const vmath::Vector3f& v) {
			focusAnchor.set(v);
		}

		void setAnchor (float x, float y, float z) {
			focusAnchor.set(x, y, z);
		}

		void setUp (const vmath::Vector3f& v) {
			up.set(v);
		}

		void setUp (float x, float y, float z) {
			up.set(x, y, z);
		}

		/**
		 * Move the eye point by differential coordinates relative to itself
		 */

		void moveEye (const vmath::Vector3f& v) {
			eyePoint += v;
		}

		void moveEye (float x, float y, float z) {
			eyePoint.x += x;
			eyePoint.y += y;
			eyePoint.z += z;
		}

		/**
		 * Move the eye point in a straight line towards to focus point
		 */

		void moveEye (float dist) {
			vmath::Vector3f rayDir = focusPoint - eyePoint;
			rayDir.normalize();
			rayDir.scale(dist);

			eyePoint += rayDir;

			if (eyePoint == focusPoint) {
				eyePoint -= rayDir;
			}
		}

		/**
		 * Move the focus point by differential coordinates relative to itself
		 */

		void moveFocus (const vmath::Vector3f& v) {
			focusPoint += v;
		}

		void moveFocus (float x, float y, float z) {
			focusPoint.x += x;
			focusPoint.y += y;
			focusPoint.z += z;
		}

		/**
		 * Move the focus anchor point by differential coordinates relative to itself
		 */

		void moveAnchor (const vmath::Vector3f& v) {
			focusAnchor += v;
		}

		void moveAnchor (float x, float y, float z) {
			focusAnchor.x += x;
			focusAnchor.y += y;
			focusAnchor.z += z;
		}

		/**
		 * Move the eye and focus points together on the eye-focus line
		 */

		void moveEyeAndFocus (float dist) {
			vmath::Vector3f rayDir = focusPoint - eyePoint;
			rayDir.normalize();
			rayDir.scale(dist);

			eyePoint += rayDir;
			focusPoint += rayDir;
		}

		/**
		 * Rotate the eye point around the focus point by a differential azimuth and elevation
		 */

		void moveEyeRelativeFocus (float azimuth, float elevation) {
			vmath::Vector3f rot = eyePoint - focusPoint;

			vmath::MatrixG3f mat = vmath::Matrix3f::IDENTITY;
			mat.rotateY(azimuth);

			vmath::Vector3f axis = orthoAxisY(focusPoint, eyePoint);
			mat.rotate(elevation, axis.x, axis.y, axis.z);

			mat.transform(rot);
			rot += focusPoint;
			eyePoint = rot;
		}

		/**
		 * Rotate the focus point around the eye point by a differential azimuth and elevation
		 */

		void moveFocusRelativeEye (float azimuth, float elevation) {
			vmath::Vector3f rot = focusPoint - eyePoint;

			vmath::MatrixG3f mat = vmath::Matrix3f::IDENTITY;
			mat.rotateY(azimuth);

			vmath::Vector3f axis = orthoAxisY(eyePoint, focusPoint);
			mat.rotate(elevation, axis.x, axis.y, axis.z);

			mat.transform(rot);
			rot += eyePoint;
			focusPoint = rot;
		}

		/**
		 * Rotate the focus point around the eye point by an azimuth and elevation relative
		 * to a set anchor point
		 */

		void moveFocusRelativeAnchor (float azimuth, float elevation) {
			vmath::Vector3f rot = focusAnchor - eyePoint;

			vmath::MatrixG3f mat = vmath::Matrix3f::IDENTITY;
			vmath::Vector3f axis = orthoAxisY(eyePoint, focusAnchor);
			mat.rotate(elevation, axis.x, axis.y, axis.z);

			mat.rotateY(azimuth);

			mat.transform(rot);
			rot += eyePoint;
			focusPoint = rot;
		}

		void rotateWorld (const vmath::Vector3f& v) {
			worldRotate += v;
		}

		void rotateWorld (float x, float y, float z) {
			worldRotate.x += x;
			worldRotate.y += y;
			worldRotate.z += z;
		}

		void translateWorld (const vmath::Vector3f& v) {
			worldTranslate += v;
		}

		void translateWorld (float x, float y, float z) {
			worldTranslate.x += x;
			worldTranslate.y += y;
			worldTranslate.z += z;
		}

		void viewTransform () {

			cameraTransform = viewLookAt();

			if (worldTranslate != 0) {
				cameraTransform.translate(worldTranslate.x, worldTranslate.y, worldTranslate.z);
			}

			if (worldRotate.x) cameraTransform.rotateX(worldRotate.x);
			if (worldRotate.y) cameraTransform.rotateY(worldRotate.y);
			if (worldRotate.z) cameraTransform.rotateZ(worldRotate.z);

			vmath::Matrix4f cameraTranspose = cameraTransform;
			cameraTranspose.transpose();

			glMultMatrixf(cameraTranspose.asArray());
		}

	protected:

		vmath::Matrix4f viewLookAt () const {
			vmath::Vector3f f = focusPoint - eyePoint;
			f.normalize();

			vmath::Vector3f UP = up;
			UP.normalize();

			vmath::Vector3f s = f.cross(UP);
			vmath::Vector3f u = s.cross(f);

			vmath::MatrixG4f ctrans(s.x, s.y, s.z, 0,
								   u.x, u.y, u.z, 0,
								   -f.x, -f.y, -f.z, 0,
								   0, 0, 0, 1);

			ctrans.translate(-eyePoint.x, -eyePoint.y, -eyePoint.z);

			return ctrans;
		}

		static vmath::Vector3f orthoAxisY (const vmath::Vector3f& p0, const vmath::Vector3f& p1) {
			vmath::Vector3f v0 = p1 - p0;
			vmath::Vector3f v1 = p1;
			v1.y += 5.f;
			v1 -= p0;

			vmath::Vector3f v2 = v0.cross(v1);
			v2.normalize();
			return v2;
		}

	};

}

#endif /* GFX_CAMERA_H_ */
