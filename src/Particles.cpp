#include "Particles.h"

#include <cstdlib>

Particles::Particles(int num, sf::Color color, sf::Vector3f position) {
	particles.resize(num);
	z = position.z;
	for (int i = 0; i < num; i++) {
		particles[i].pos.x = position.x;
		particles[i].pos.y = position.y;
		particles[i].xVel = rand() / (float)RAND_MAX - 0.5f;
		particles[i].yVel = rand() / (float)RAND_MAX * -2 - 4;

		int r = color.r + rand() % 20 - 10;
		if (r < 0) r = 0;
		if (r > 255) r = 255;
		int g = color.g + rand() % 20 - 10;
		if (g < 0) g = 0;
		if (g > 255) g = 255;
		int b = color.b + rand() % 20 - 10;
		if (b < 0) b = 0;
		if (b > 255) b = 255;

		particles[i].color.r = r;
		particles[i].color.g = g;
		particles[i].color.b = b;
	}
}

int Particles::getNumParticles() {
	return particles.size();
}
const Particle& Particles::getParticle(int i) {
	return particles[i];
}

float Particles::getZ() {
	return z;
}

void Particles::update(float diff) {
	for (int i = 0; i < getNumParticles(); i++) {
		particles[i].pos.x += particles[i].xVel * diff;
		particles[i].pos.y += particles[i].yVel * diff;
		particles[i].yVel += diff * 10;
	}
}
