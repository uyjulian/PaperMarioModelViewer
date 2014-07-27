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

#ifndef APPSTATE_H_
#define APPSTATE_H_

#include "system/InputController.h"
#include "renderer/Camera.h"

class AppState {
public:

	struct Vars {
		enum TransformState {
			TRANSLATE, ROTATE, ZOOM,
		};

		enum CameraModel {
			MANIPULATE_WORLD, MANIPULATE_CAMERA,
		};

		enum SelectState {
			SELECT_SGNODE, SELECT_MESH, SELECT_POLY,
		};

		bool showMeshBBox;
		bool showPolyFrame;
		bool showSGObjectBBox;
		TransformState transformState;
		CameraModel cameraModel;
		SelectState selectState;
		int selectIndex;

		Vars () {
			resetDefault();
		}

		void resetDefault () {
			showMeshBBox = false;
			showPolyFrame = false;
			showSGObjectBBox = false;
			transformState = ROTATE;
			cameraModel = MANIPULATE_WORLD;
			selectState = SELECT_SGNODE;
			selectIndex = -1;
		}
	};

	InputController& ic;
	gfx::Camera camera;
	Vars vars;

	unsigned frameCounter;

public:

	static AppState& getState () {
		static AppState as;
		return as;
	}

	void resetDefault () {
		frameCounter = 0;
		camera.setEye(0.f, 0.f, 30.f);
		camera.setFocus(0.f, 0.f, 0.f);
		vars.resetDefault();
	}

protected:

	AppState ()
		: ic(InputController::getController()) {
		resetDefault();
	}

	AppState (const AppState&)
		: ic(InputController::getController()) {
		resetDefault();
	}

	AppState& operator= (const AppState&) {
		return *this;
	}
};

#endif /* APPSTATE_H_ */
