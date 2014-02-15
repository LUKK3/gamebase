#ifndef RENDERER_H
#define RENDERER_H

#include "Tunnel.h"
#include "main.h"

#include <SFML/Graphics.hpp>

class Renderer {
	public:
		Renderer();

		void render(sf::RenderTarget& target, Tunnel& tunnel, float z);

	private:
		void drawTile(sf::RenderTarget& target, float z, int x);
};

#endif // RENDERER_H
