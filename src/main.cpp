#include <SFML/Graphics.hpp>

#include "Tunnel.h"
#include "Renderer.h"

float yourZ = 0;
float rockZ = -10;
Tunnel tunnel;
Renderer renderer;

sf::RenderWindow window;
sf::Event event;
sf::Clock mainClock;
sf::Time prevTime;

void events() {
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			// SFML sent us a close event, so clean up
			window.close();
		}
	}
}

void logic() {
	sf::Time newTime = mainClock.getElapsedTime();
	sf::Time diff = newTime - prevTime;
	prevTime = newTime;
	float difff = diff.asMicroseconds() / 1000000.f;
	yourZ += difff * 5;
}

void render() {
	// Clear the window before we start drawing to it
	window.clear();

	renderer.render(window, tunnel, yourZ);

	// Notify the window that we're ready to render
	window.display();
}

// Program entry point
int main(int argc, char ** argv) {

    // Create the SFML window
	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game!");

    while (window.isOpen()) {
		events();
		logic();
		render();
    }

    return 0;
}
