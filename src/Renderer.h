#ifndef RENDERER_H
#define RENDERER_H

#include "Tunnel.h"
#include "main.h"
#include "Player.h"

#include <SFML/Graphics.hpp>

class Renderer {
	public:
		Renderer();

		void render(sf::RenderTarget& target, Tunnel& tunnel, Player& player);

	private:
		void drawTile(sf::VertexArray& vertexArray, float z, int x1, int x2, int y1, int y2, bool color);

		sf::Texture tileTexture;
		sf::Texture playerTexture;
};

#endif // RENDERER_H
