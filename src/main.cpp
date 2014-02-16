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

int difficulty = 0;

sf::RenderWindow window;
sf::Event event;
sf::Clock mainClock;
sf::Time prevTime;

sf::Sound boulderSound, fallingSound, jumpingSound, feetSound, musicSound, bumpSound, gemSound;

void resetGame() {
	std::cout << player.z << " " << tunnel.getLength() - 1 << std::endl;
	if (player.z > tunnel.getLength() - 2 && player.score >= 5) difficulty++;
	tunnel.reset((difficulty + 2) * 100, difficulty);
	player.reset();
	renderer.reset();
	rockZ = ROCK_START_Z;
	rockVel = ROCK_START_VEL;
	musicSound.setPlayingOffset(sf::Time::Zero);
	musicSound.play();
}

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

	int z1 = std::floor(player.z + 1.3);
	int z2 = std::floor(player.z + 0.9);
	int x1 = std::floor(player.x + 2.1);
	int x2 = std::floor(player.x + 1.9);

	if (rockZ > tunnel.getLength() - 1 && tunnel.hasBrickWall()) {
		//destroy bricks
		for (float i = -2; i <= 2; i += 0.1f) {
			for (float j = -2; j <= 2; j += 0.1f) {
				renderer.addParticles(1, sf::Color(108, 7, 7), sf::Vector3f(i, j, tunnel.getLength()));
			}
		}
		tunnel.breakWall();
	}

	if (player.fallen) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			resetGame();
		}
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
			int rok = tunnel.get(z1, x1);
			if (rok != 5) {
				if (rok == 2)
					player.zVel = 1.5;
				else if (rok == 3)
					player.zVel = 3.5f;
				else if (rok == 4)
					player.zVel = 0.0f;
				bumpSound.play();
				tunnel.set(z1, x1, 0);
				renderer.addParticles(10, sf::Color(100, 100, 100), sf::Vector3f(player.x, 2, player.z));
			}
		} else if (!player.fallen && player.y > 0.3 && (tunnel.get(z1, x1 + 5) >= 1 && tunnel.get(z2, x1 + 5) >= 1 && tunnel.get(z1, x2 + 5) >= 1 && tunnel.get(z2, x2 + 5) >= 1)) {
			int gem = tunnel.get(z1, x1 + 5);
			player.score += gem;
			gemSound.play();
			if (gem == 1) {
				renderer.addParticles(5, sf::Color(200, 0, 0), sf::Vector3f(player.x, .5, player.z + 1));
			} else if (gem == 2) {
				renderer.addParticles(5, sf::Color(0, 255, 0), sf::Vector3f(player.x, .5, player.z + 1));
			} else if (gem == 3) {
				renderer.addParticles(5, sf::Color(0, 255, 255), sf::Vector3f(player.x, .5, player.z + 1));
			}
			tunnel.set(z1, x1 + 5, 0);
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
		if (!player.fallen && player.y <= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			player.yVel = 4;
			player.y += 0.01;
			jumpingSound.play();
			feetSound.pause();
		}
	}

	if (!player.fallen && player.y < 0) {
		player.y = 0;
		player.yVel = 0;
		if (feetSound.getStatus() == sf::Sound::Status::Paused) {
			feetSound.play();
		}
	}

	player.yVel -= difff * 15;

	if (player.z > tunnel.getLength() - 2) {

	} else {
		player.z += player.zVel * difff * 2;
	}
	player.y += player.yVel * difff;
	player.x += player.xVel * difff * 1.5;
	if (player.x < -1.9) {
		player.x = -1.9;
		player.xVel = 0;
	}
	else if (player.x > 1.9) {
		player.x = 1.9;
		player.xVel = 0;
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
		resetGame();
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
	musicSound.setVolume(15.f);
	musicSound.setLoop(true);
	musicSound.play();

	sf::SoundBuffer sb5;
	sb5.loadFromFile("assets/footsteps.ogg");
	feetSound.setRelativeToListener(true);
	feetSound.setBuffer(sb5);
	feetSound.setLoop(true);
	feetSound.play();

	sf::SoundBuffer sb6;
	sb6.loadFromFile("assets/bump.ogg");
	bumpSound.setRelativeToListener(true);
	bumpSound.setBuffer(sb6);

	sf::SoundBuffer sb7;
	sb7.loadFromFile("assets/gem.ogg");
	gemSound.setRelativeToListener(true);
	gemSound.setBuffer(sb7);

    // Create the SFML window
	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game!");

    while (window.isOpen()) {
		events();
		logic();
		render();
    }

    return 0;
}
