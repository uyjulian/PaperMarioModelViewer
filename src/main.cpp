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

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdexcept>
#include "system/InputController.h"
#include "system/WindowController.h"
#include "GLView.h"

int main (int argc, char** argv)
{
	if (argc <= 1) {
		std::cout << "No model file specified" << std::endl;
		return 1;
	}
	
	AppState& state = AppState::getState();
	WindowController& wc = WindowController::getController();
	InputController& ic = InputController::getController();

	int appInitWidth = 800;
	int appInitHeight = 600;

	sf::FloatRect windowRect(0.f, 0.f, 800.f, 600.f);
	sf::View windowView(windowRect);

	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 2;
	settings.majorVersion = 2;
	settings.minorVersion = 1;

	wc.window.create(sf::VideoMode(appInitWidth, appInitHeight, 32), "", sf::Style::Resize | sf::Style::Close, settings);
	wc.window.setView(windowView);
	wc.window.setVerticalSyncEnabled(true);
	wc.window.setFramerateLimit(60);
	wc.window.setTitle("Paper Mario Model Viewer");
	
	//wc.window.preserveOpenGLStates(true);

	GLView view = GLView(appInitWidth, appInitHeight);

	std::cout << "File: " << argv[1] << std::endl;

	try {
		view.setModelFile(std::string(argv[1]));
		view.init();

		std::string programDir = pathname(std::string(argv[0]));

	}
	catch (std::exception e) {
		std::cout << std::endl;
		std::cout << "Uncaught Initialization Exception:" << std::endl;
		std::cout << e.what() << std::endl;
		std::cout << "Press any key to continue..." << std::flush;

		std::cin.ignore( std::numeric_limits <std::streamsize> ::max(), '\n' );
		exit(1);
	}

	try {
		while (wc.window.isOpen()) {
			wc.window.setActive();
			ic.input.resetState();

			wc.window.clear(sf::Color(96, 96, 96));

			sf::Event event;
			while (wc.window.pollEvent(event))
			{
				ic.input.processEvent(event);

				if (event.type == sf::Event::Closed) {
					wc.window.close();
				}

				if (event.type == sf::Event::Resized) {
					view.resize(event.size.width, event.size.height);
				}
			}
			
			view.eventLoop();
			view.display();
			

			wc.window.display();


			state.frameCounter++;
		}
	}
	catch (std::exception e) {
		std::cout << std::endl;
		std::cout << "Uncaught Runtime Exception:" << std::endl;
		std::cout << e.what() << std::endl << std::endl;
		std::cout << "Press any key to continue..." << std::flush;

		std::cin.ignore( std::numeric_limits <std::streamsize> ::max(), '\n' );
		exit(1);
	}

    return 0;
}
