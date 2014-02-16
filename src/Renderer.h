#ifndef RENDERER_H
#define RENDERER_H

#include "Tunnel.h"
#include "main.h"
#include "Player.h"
#include "Particles.h"

#include <set>

#include <SFML/Graphics.hpp>

struct FloatingText {
	sf::Text text;
	float time;
};

class Renderer {
	public:
		Renderer();

		void update(float diff);

		void render(sf::RenderTarget& target, Tunnel& tunnel, Player& player, float rockZ);
		bool renderUI(sf::RenderWindow& target, Tunnel& tunnel, Player& player, float rockZ);

		void addParticles(int num, sf::Color color, sf::Vector3f position);
		void addStatusText(sf::Text text);

        sf::Font& getFont();

		void reset();

	private:
		void drawTile(sf::VertexArray& vertexArray, float z1, float z2, float x1, float x2, float y1, float y2, int x = -1, int z = 0, bool lava = false);
		void drawTileFlat(sf::VertexArray& vertexArray, float z, float x1, float x2, float y1, float y2, const sf::Vector2f* texCoords);

		std::vector<FloatingText> statusTexts;

		sf::Texture tileTexture;
		sf::Texture playerTexture;
		sf::Texture rockTexture;
		sf::Texture gauge, playerInd, boulderInd;
		sf::Font font;

		float lightRatio;
		Player* player;
		Tunnel* tunnel;

		float tim;

		std::vector<Particles> particlesSet;
};

#endif // RENDERER_H
