#include <SFML/Graphics.hpp>

#include "Tunnel.h"
#include "Renderer.h"

Player player;
float rockZ = -10;
Tunnel tunnel;
Renderer renderer;

sf::RenderWindow window;
sf::Event event;
sf::Clock mainClock;
sf::Time prevTime;

void events() {
	while (window.pollEvent(event)) {
		switch(event.type) {
			case sf::Event::Closed: window.close(); break;
			default: break;
		}
	}
}

void logic() {
	sf::Time newTime = mainClock.getElapsedTime();
	sf::Time diff = newTime - prevTime;
	prevTime = newTime;
	float difff = diff.asMicroseconds() / 1000000.f;
	player.z += difff * 5;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		player.x -= difff;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		player.x += difff;
	}
}

void render() {
	// Clear the window before we start drawing to it
	window.clear();

	renderer.render(window, tunnel, player);

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
