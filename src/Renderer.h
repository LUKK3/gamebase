#ifndef RENDERER_H
#define RENDERER_H

#include "Tunnel.h"
#include "main.h"
#include "Player.h"
#include "Particles.h"

#include <SFML/Graphics.hpp>

class Renderer {
	public:
		Renderer();

		void render(sf::RenderTarget& target, Tunnel& tunnel, Player& player);
		void renderUI(sf::RenderTarget& target, Tunnel& tunnel, Player& player);

	private:
		void drawTile(sf::VertexArray& vertexArray, float z1, float z2, int x1, int x2, float y1, float y2, int x = -1, int z = 0);
		void drawTileFlat(sf::VertexArray& vertexArray, float z, int x1, int x2, float y1, float y2, const sf::Vector2f* texCoords);

		sf::Texture tileTexture;
		sf::Texture playerTexture;
		sf::Texture gauge, playerInd, boulderInd;
		sf::Font font;

		float lightRatio;
		Player* player;
		Tunnel* tunnel;

		std::vector<Particles> particles;
};

#endif // RENDERER_H
