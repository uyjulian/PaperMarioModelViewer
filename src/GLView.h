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

#ifndef GLVIEW_H_
#define GLVIEW_H_

#include <SFML/System.hpp>
#include <OpenGL/glu.h>
#include "AppState.h"
#include "PMModelGL.h"
//#include "PMWorldGL.h"
#include "system/Input.h"
#include "vecmath/MatrixG4.h"
#include "renderer/Scenegraph.h"

class GLView {
protected:

	std::string modelFile;
	PMModelGL pmm;

	unsigned _width;
	unsigned _height;

public:

	GLView () {
		_width = 800;
		_height = 600;
	}

	GLView (int width, int height) {
		_width = width;
		_height = height;
	}

	virtual ~GLView () { }

	void setModelFile (const std::string& str) {
		modelFile = str;
	}

	void init () {
		glClearDepth(1.f);
		glClearColor(.2f, .2f, .2f, 0.f);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);

		glDisable(GL_CULL_FACE);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);

		if (!pmm.LoadFile(modelFile)) {
			std::cout << "Error encountered: " << pmm.errorMessage << std::endl;
			exit(1);
		}

		pmm.Init();

		AppState::getState();
		resize(_width, _height);
	}

	void resize (int width, int height) {
		_width = width;
		_height = height;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(40.f, (float)width / (float)height, .1f, 2000.f);
		glViewport(0, 0, width, height);
		glMatrixMode(GL_MODELVIEW);
	}

	void display () {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Camera transformation

		AppState& state = AppState::getState();
		state.camera.viewTransform();

		// Draw Model

		pmm.Draw();

		// Draw state informational text

		WindowController& wc = WindowController::getController();

		std::stringstream str("");
		str << "Paper Mario Model Viewer | ";
		str << "Camera: ";
		str << cameraStateString();
		str << " | Model: ";
		str << pmm.GetInfoString();
		
		wc.window.setTitle(str.str());
	}

	void eventLoop () {
		AppState& state = AppState::getState();
		// Switch modelview transformation
		if (state.ic.input.keyPressed(sf::Keyboard::T)) {
			state.vars.transformState = AppState::Vars::TRANSLATE;
		}
		if (state.ic.input.keyPressed(sf::Keyboard::R)) {
			state.vars.transformState = AppState::Vars::ROTATE;
		}
		if (state.ic.input.keyPressed(sf::Keyboard::Z)) {
			state.vars.transformState = AppState::Vars::ZOOM;
		}

		// Switch cmarea model
		if (state.ic.input.keyPressed(sf::Keyboard::C)) {
			if (state.vars.cameraModel == AppState::Vars::MANIPULATE_WORLD) {
				state.vars.cameraModel = AppState::Vars::MANIPULATE_CAMERA;
			}
			else {
				state.vars.cameraModel = AppState::Vars::MANIPULATE_WORLD;
			}
		}

		float speedMult = 1.f;
		if (state.ic.input.keyHeld(sf::Keyboard::RShift)) {
			speedMult = 2.f;
		}

		if (state.vars.transformState == AppState::Vars::ZOOM) {
			if (state.ic.input.keyHeld(sf::Keyboard::Up)) {
				modelviewZoom(state, -.5f * speedMult);
			}
			if (state.ic.input.keyHeld(sf::Keyboard::Down)) {
				modelviewZoom(state, .5f * speedMult);
			}
		}
		if (state.vars.transformState == AppState::Vars::TRANSLATE) {
			if (state.ic.input.keyHeld(sf::Keyboard::Up)) {
				modelviewTranslate(state, 0.f, .5f * speedMult);
			}
			if (state.ic.input.keyHeld(sf::Keyboard::Down)) {
				modelviewTranslate(state, 0.f, -.5f * speedMult);
			}
			if (state.ic.input.keyHeld(sf::Keyboard::Left)) {
				modelviewTranslate(state, -.5f * speedMult, 0.f);
			}
			if (state.ic.input.keyHeld(sf::Keyboard::Right)) {
				modelviewTranslate(state, .5f * speedMult, 0.f);
			}
		}
		if (state.vars.transformState == AppState::Vars::ROTATE) {
			if (state.ic.input.keyHeld(sf::Keyboard::Up)) {
				modelviewRotate(state, -1.f * speedMult, 0.f);
			}
			if (state.ic.input.keyHeld(sf::Keyboard::Down)) {
				modelviewRotate(state, 1.f * speedMult, 0.f);
			}
			if (state.ic.input.keyHeld(sf::Keyboard::Left)) {
				modelviewRotate(state, 0.f, -1.f * speedMult);
			}
			if (state.ic.input.keyHeld(sf::Keyboard::Right)) {
				modelviewRotate(state, 0.f, 1.f * speedMult);
			}
		}

		if (state.ic.input.keyPressed(sf::Keyboard::Num1)) {
			state.vars.showSGObjectBBox = (state.vars.showSGObjectBBox) ? 0 : 1;
		}

		// Display Restrictions
		if (state.ic.input.keyPressed(sf::Keyboard::D)) {
			switch (state.vars.selectState) {
			case AppState::Vars::SELECT_SGNODE:
				state.vars.selectState = AppState::Vars::SELECT_MESH; break;
			case AppState::Vars::SELECT_MESH:
				state.vars.selectState = AppState::Vars::SELECT_POLY; break;
			case AppState::Vars::SELECT_POLY:
				state.vars.selectState = AppState::Vars::SELECT_SGNODE; break;
			}
		}

		if (state.ic.input.keyPressed(sf::Keyboard::Add)) {
			state.vars.selectIndex++;
		}
		if (state.ic.input.keyPressed(sf::Keyboard::Subtract)) {
			if (state.vars.selectIndex > -1) {
				state.vars.selectIndex--;
			}
		}
	}

protected:

	void modelviewTranslate (AppState& state, f32 x, f32 y) {
		switch (state.vars.cameraModel) {
		case AppState::Vars::MANIPULATE_WORLD:
			state.camera.translateWorld(x, y, 0.f);
			break;
		case AppState::Vars::MANIPULATE_CAMERA:
			break;
		}
	}

	void modelviewZoom (AppState& state, f32 z) {
		switch (state.vars.cameraModel) {
		case AppState::Vars::MANIPULATE_WORLD:
			state.camera.translateWorld(0.f, 0.f, z);
			break;
		case AppState::Vars::MANIPULATE_CAMERA:
			state.camera.moveEyeAndFocus(-z);
			break;
		}
	}

	void modelviewRotate (AppState& state, f32 x, f32 y) {
		switch (state.vars.cameraModel) {
		case AppState::Vars::MANIPULATE_WORLD:
			state.camera.rotateWorld(x, y, 0.f);
			break;
		case AppState::Vars::MANIPULATE_CAMERA:
			state.camera.moveFocusRelativeEye(y, -x);
			break;
		}
	}

	std::string cameraStateString () const {
		AppState& state = AppState::getState();
		std::stringstream str;

		switch (state.vars.cameraModel) {
		case AppState::Vars::MANIPULATE_WORLD:
			str << "WORLD | ";
			break;
		case AppState::Vars::MANIPULATE_CAMERA:
			str << "CAMERA | ";
			break;
		}

		switch (state.vars.transformState) {
		case AppState::Vars::TRANSLATE:
			str << "Translate";
			break;
		case AppState::Vars::ROTATE:
			str << "Rotate";
			break;
		case AppState::Vars::ZOOM:
			str << "Zoom";
			break;
		}

		return str.str();
	}

};

#endif /* GLVIEW_H_ */
