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

#ifndef INPUT_H_JRAQ_
#define INPUT_H_JRAQ_

#include <SFML/Window.hpp>
#include <vector>

class Input {
protected:

	std::vector<bool> _mouseHoldState;
	std::vector<bool> _mouseClickState;
	std::vector<bool> _keyHoldState;
	std::vector<bool> _keyClickState;

	int _mouseWheelState;
	int _mouseX;
	int _mouseY;
	int _mouseDragX;
	int _mouseDragY;

public:

	Input ()
		: _mouseHoldState(5), _mouseClickState(5), _keyHoldState(384), _keyClickState(384) {
		initState();
	}

	virtual ~Input () { }

	void initState () {
		_mouseWheelState = 0;
		_mouseX = 0;
		_mouseY = 0;
		_mouseDragX = 0;
		_mouseDragY = 0;

		for (int i = 0; i < 384; i++) {
			_keyClickState[i] = false;
			_keyHoldState[i] = false;
		}
		for (int i = 0; i < 5; i++) {
			_mouseClickState[i] = false;
			_mouseHoldState[i] = false;
		}
	}

	void resetState () {
		for (int i = 0; i < 384; i++) {
			_keyClickState[i] = false;
		}
		for (int i = 0; i < 5; i++) {
			_mouseClickState[i] = false;
		}
	}

	void processEvent (sf::Event event) {
		switch (event.type) {
			case sf::Event::KeyPressed:
				keyPressedEvent(event); break;
			case sf::Event::KeyReleased:
				keyReleasedEvent(event); break;
			case sf::Event::MouseButtonPressed:
				mousePressedEvent(event); break;
			case sf::Event::MouseButtonReleased:
				mouseReleasedEvent(event); break;
			case sf::Event::MouseMoved:
				mouseMovedEvent(event); break;
			case sf::Event::MouseWheelMoved:
				mouseWheelMovedEvent(event); break;
			default: break;
		}
	}

	bool keyPressed (sf::Keyboard::Key key) const {
		return _keyClickState[key];
	}

	bool keyHeld (sf::Keyboard::Key key) const {
		return _keyHoldState[key];
	}

	bool mouseClicked (sf::Mouse::Button button) const {
		return _mouseClickState[button];
	}

	bool mouseHeld (sf::Mouse::Button button) const {
		return _mouseHoldState[button];
	}

	bool mouseWheelMoved () const {
		return (_mouseWheelState != 0);
	}

	int mouseWheelDistance () const {
		return _mouseWheelState;
	}

	int mouseX () const {
		return _mouseX;
	}

	int mouseY () const {
		return _mouseY;
	}

	bool mouseDragged () const;
	int mouseDraggedX () const;
	int mouseDraggedY () const;

protected:

	void keyPressedEvent (sf::Event event) {
		_keyHoldState[event.key.code] = true;
	}

	void keyReleasedEvent (sf::Event event) {
		_keyHoldState[event.key.code] = false;
		_keyClickState[event.key.code] = true;
	}

	void mousePressedEvent (sf::Event event) {
		_mouseHoldState[event.mouseButton.button] = true;
	}

	void mouseReleasedEvent (sf::Event event) {
		_mouseHoldState[event.mouseButton.button] = false;
		_mouseClickState[event.mouseButton.button] = true;
	}

	void mouseMovedEvent (sf::Event event) {
		_mouseX = event.mouseMove.x;
		_mouseY = event.mouseMove.y;
	}

	void mouseWheelMovedEvent (sf::Event event) {
		_mouseWheelState = event.mouseWheel.delta;
	}
};

#endif /* INPUT_H_JRAQ_*/
