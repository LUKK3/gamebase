#ifndef PARTICLES_H
#define PARTICLES_H

#include <SFML/Graphics.hpp>

struct Particle {
	sf::Vector2f pos;
	sf::Color color;
	float xVel;
	float yVel;
};

class Particles {
	public:
		Particles(int num, sf::Color color, sf::Vector3f position);

		int getNumParticles();
		const Particle& getParticle(int i);

		void update(float diff);

		float getZ();

	private:
		float z;
		std::vector<Particle> particles;
};

#endif // PARTICLES_H
