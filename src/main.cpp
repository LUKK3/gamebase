#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

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

sf::Sound boulderSound, fallingSound, jumpingSound;

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
		fallingSound.play();
	} else if (player.fallen) {
		player.zVel -= difff * 10;
		if (player.zVel < 0) player.zVel = 0;
	} else {
		if (!player.fallen && player.y < 0.3 && (tunnel.get(z1, x1) > 1 && tunnel.get(z2, x1) > 1 && tunnel.get(z1, x2) > 1 && tunnel.get(z2, x2) > 1)) {
			player.zVel = 1.5;
			tunnel.set(z1, x1, 0);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			player.xVel -= difff * 4;
			if (player.xVel < -1) player.xVel = -1;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			player.zVel += difff * 4;
			if (player.zVel > 7) player.zVel = 7;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			if (!player.zVel < 3)
				player.zVel -= difff * 4;
		} else {
			if (player.zVel > 5) {
				if (player.zVel < 5 + difff * 3) {
					player.zVel = 5;
				} else {
					player.zVel -= difff * 3;
				}
			} else if (player.zVel < 5) {
				if (player.zVel > 5 - difff * 3) {
					player.zVel = 5;
				} else {
					player.zVel += difff * 3;
				}
			}
		}
		if (!player.fallen && player.y == 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			player.yVel = 4;
			jumpingSound.play();
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
	renderer.renderUI(window, tunnel, player);

	// Notify the window that we're ready to render
	window.display();
}

// Program entry point
int main(int argc, char ** argv) {
	sf::SoundBuffer sb;
	sb.loadFromFile("assets/avalanche.ogg");
	boulderSound.setBuffer(sb);
	boulderSound.setLoop(true);
	boulderSound.play();

	sf::SoundBuffer sb2;
	sb2.loadFromFile("assets/death.ogg");
	fallingSound.setBuffer(sb2);

	sf::SoundBuffer sb3;
	sb3.loadFromFile("assets/jump.wav");
	jumpingSound.setBuffer(sb3);

    // Create the SFML window
	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game!");

    while (window.isOpen()) {
		events();
		logic();
		render();
    }

    return 0;
}
