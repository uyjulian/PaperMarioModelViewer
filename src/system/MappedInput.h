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

#ifndef MAPPEDINPUT_H_
#define MAPPEDINPUT_H_

#include "Input.h"

namespace Keymap {
	enum Code {
		Left,
		Right,
		Up,
		Down,
		Action1,
		Action2,
		Action3,
		Action4,
		Start,
	};
}

namespace Mousemap {
	enum Button {
		Left,
		Middle,
		Right,
		XButton1,
		XButton2,
	};
}

class MappedInput: public Input {
protected:

	std::vector<sf::Key::Code> _keyMap;
	std::vector<sf::Mouse::Button> _mouseMap;

public:

	MappedInput ()
		: _keyMap(9), _mouseMap(5), Input() {
		loadDefaultMap();
	}

	virtual ~MappedInput () { }

	void loadDefaultMap () {
		_keyMap[Keymap::Left] = sf::Key::Left;
		_keyMap[Keymap::Right] = sf::Key::Right;
		_keyMap[Keymap::Up] = sf::Key::Up;
		_keyMap[Keymap::Down] = sf::Key::Down;
		_keyMap[Keymap::Action1] = sf::Key::RShift;
		_keyMap[Keymap::Action2] = sf::Key::RControl;
		_keyMap[Keymap::Action3] = sf::Key::A;
		_keyMap[Keymap::Action4] = sf::Key::LShift;
		_keyMap[Keymap::Start] = sf::Key::Return;

		_mouseMap[Mousemap::Left] = sf::Mouse::Left;
		_mouseMap[Mousemap::Middle] = sf::Mouse::Middle;
		_mouseMap[Mousemap::Right] = sf::Mouse::Right;
		_mouseMap[Mousemap::XButton1] = sf::Mouse::XButton1;
		_mouseMap[Mousemap::XButton2] = sf::Mouse::XButton2;
	}

	void setMapping (Keymap::Code code, sf::Key::Code sfcode) {
		_keyMap[code] = sfcode;
	}

	void setMapping (Mousemap::Button button, sf::Mouse::Button sfbutton) {
		_mouseMap[button] = sfbutton;
	}

	bool keyPressed (Keymap::Code key) const {
		return Input::keyHeld(_keyMap[key]);
	}

	bool keyHeld (Keymap::Code key) const {
		return Input::keyPressed(_keyMap[key]);
	}

	bool mouseClicked (Mousemap::Button button) const {
		return Input::mouseClicked(_mouseMap[button]);
	}

	bool mouseHeld (Mousemap::Button button) const {
		return Input::mouseHeld(_mouseMap[button]);
	}
};

#endif /* MAPPEDINPUT_H_ */
