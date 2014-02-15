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

	int z1 = std::floor(player.z + 1.1);
	int z2 = std::floor(player.z + 0.9);
	int x1 = std::floor(player.x + 2.1);
	int x2 = std::floor(player.x + 1.9);
	if (!player.fallen && player.y < 0.01 && (tunnel.get(z1, x1) == 1 && tunnel.get(z2, x1) == 1 && tunnel.get(z1, x2) == 1 && tunnel.get(z2, x2) == 1)) {
		player.fallen = true;
	} else if (player.fallen) {
		player.zVel -= difff * 10;
		if (player.zVel < 0) player.zVel = 0;
	} else {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			player.xVel -= difff * 4;
			if (player.xVel < -1) player.xVel = -1;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			player.xVel += difff * 4;
			if (player.xVel > 1) player.xVel = 1;
		} else {
			if (player.xVel > 0) {
				if (player.xVel < difff * 3) {
					player.xVel = 0;
				} else {
					player.xVel -= difff * 3;
				}
			} else if (player.xVel < 0) {
				if (player.xVel > -difff * 3) {
					player.xVel = 0;
				} else {
					player.xVel += difff * 3;
				}
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			player.zVel += difff * 4;
			if (player.zVel > 7) player.zVel = 7;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			player.zVel -= difff * 4;
			if (player.zVel < 3) player.zVel = 3;
		} else {
			if (player.zVel > 5) {
				if (player.zVel < difff * 3) {
					player.zVel = 5;
				} else {
					player.zVel -= difff * 3;
				}
			} else if (player.zVel < 5) {
				if (player.zVel > -difff * 3) {
					player.zVel = 5;
				} else {
					player.zVel += difff * 3;
				}
			}
		}
		if (!player.fallen && player.y == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			player.yVel = 4;
		}
	}
	player.yVel -= difff * 15;

	player.z += player.zVel * difff * 2;
	player.y += player.yVel * difff;
	player.x += player.xVel * difff;
	if (player.x < -1.9) {
		player.x = -1.9;
		player.xVel = 0;
	}
	else if (player.x > 1.9) {
		player.x = 1.9;
		player.xVel = 0;
	}
	if (!player.fallen && player.y < 0) {
		player.y = 0;
		player.yVel = 0;
	}
}

void render() {
	// Clear the window before we start drawing to it
	window.clear();

	renderer.render(window, tunnel, player);
	renderer.renderUI(window);

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
