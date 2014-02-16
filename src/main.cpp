#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Tunnel.h"
#include "Renderer.h"

#include <cstdlib>

static const float ROCK_START_Z = -10;
static const float ROCK_START_VEL = 8.f;

Player player;
float rockZ = ROCK_START_Z;
float rockVel = ROCK_START_VEL;
Tunnel tunnel;
Renderer renderer;

sf::RenderWindow window;
sf::Event event;
sf::Clock mainClock;
sf::Time prevTime;

sf::Sound boulderSound, fallingSound, jumpingSound, feetSound, musicSound;

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

	if (player.fallen) {
		feetSound.stop();
		player.zVel -= difff * 10;
		if (player.zVel < 0) player.zVel = 0;

		if (rockZ > player.z) {
			static float parts = 0;
			parts += difff * 20;
			while(parts > 0) {
				renderer.addParticles(1, sf::Color(100, 50, 10), sf::Vector3f(rand() / (float)RAND_MAX * 2 - 1, 2, rockZ + 2));
				parts--;
			}
		}
	} else if (rockZ > player.z) {
		player.fallen = true;
		fallingSound.play();
		musicSound.pause();
		renderer.addParticles(10, sf::Color(255, 0, 0), sf::Vector3f(player.x, 2, player.z));
		player.y = -100;
	} else if (player.y < 0.01 && (tunnel.get(z1, x1) == 1 && tunnel.get(z2, x1) == 1 && tunnel.get(z1, x2) == 1 && tunnel.get(z2, x2) == 1)) {
		player.fallen = true;
		fallingSound.play();
		musicSound.pause();
		renderer.addParticles(10, sf::Color(255, 127, 0), sf::Vector3f(player.x, 3, player.z));
	} else {
		if (!player.fallen && player.y < 0.3 && (tunnel.get(z1, x1) > 1 && tunnel.get(z2, x1) > 1 && tunnel.get(z1, x2) > 1 && tunnel.get(z2, x2) > 1)) {
			player.zVel = 1.5;
			tunnel.set(z1, x1, 0);
			renderer.addParticles(10, sf::Color(100, 100, 100), sf::Vector3f(player.x, 2, player.z));
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
			if (player.zVel > 8) player.zVel = 8;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			player.zVel -= difff * 4;
			if (player.zVel < 3)
				player.zVel = 3;
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
			feetSound.pause();
		}
	}
	player.yVel -= difff * 15;

	if (player.z > tunnel.getLength() - 2) {

	} else {
		player.z += player.zVel * difff * 2;
	}
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
		if (feetSound.getStatus() == sf::Sound::Status::Paused) {
			feetSound.play();
		}
	}

	rockZ += difff * rockVel;
	rockVel += difff / 2;

	renderer.update(difff);

	sf::Listener::setPosition(player.x / 10, player.y / 10, player.z / 10);
	boulderSound.setPosition(0, 0, rockZ / 10 + 2);
}

void render() {
	// Clear the window before we start drawing to it
	window.clear();

	renderer.render(window, tunnel, player, rockZ);
	if (renderer.renderUI(window, tunnel, player, rockZ)) {
		tunnel.reset();
		player.reset();
		renderer.reset();
		rockZ = ROCK_START_Z;
		rockVel = ROCK_START_VEL;
		musicSound.setPlayingOffset(sf::Time::Zero);
		musicSound.play();
	}

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
	fallingSound.setRelativeToListener(true);

	sf::SoundBuffer sb3;
	sb3.loadFromFile("assets/jump.wav");
	jumpingSound.setBuffer(sb3);
	jumpingSound.setRelativeToListener(true);

	sf::SoundBuffer sb4;
	sb4.loadFromFile("assets/POL-flash-run-short.wav");
	musicSound.setBuffer(sb4);
	musicSound.setRelativeToListener(true);
	musicSound.setVolume(30.f);
	musicSound.setLoop(true);
	musicSound.play();

	sf::SoundBuffer sb5;
	sb5.loadFromFile("assets/footsteps.ogg");
	feetSound.setBuffer(sb5);
	feetSound.setLoop(true);
	feetSound.play();

    // Create the SFML window
	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game!");

    while (window.isOpen()) {
		events();
		logic();
		render();
    }

    return 0;
}
