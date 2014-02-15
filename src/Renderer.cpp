#include "Renderer.h"

#define TILE_SIZE 100

Renderer::Renderer() {
	//ctor
}

void Renderer::render(sf::RenderTarget& target, Tunnel& tunnel, float z) {

	for (int i = (int)z; i < (int)z + 10; i++) {

		drawTile(target, i - z, 0);

	}
}

void Renderer::drawTile(sf::RenderTarget& target, float z, int x) {

	sf::ConvexShape shape;
	shape.setPointCount(4);

	float sz = std::sqrt(z);
	shape.setPoint(0, sf::Vector2f(WINDOW_WIDTH / 2 + ((2 - x) * TILE_SIZE) / sz, WINDOW_HEIGHT / 2 + (2 * TILE_SIZE) / sz));
	shape.setPoint(1, sf::Vector2f(WINDOW_WIDTH / 2 + ((1 - x) * TILE_SIZE) / sz, WINDOW_HEIGHT / 2 + (2 * TILE_SIZE) / sz));
	float sz2 = std::sqrt(z + 1);
	shape.setPoint(2, sf::Vector2f(WINDOW_WIDTH / 2 + ((1 - x) * TILE_SIZE) / sz2, WINDOW_HEIGHT / 2 + (2 * TILE_SIZE) / sz2));
	shape.setPoint(3, sf::Vector2f(WINDOW_WIDTH / 2 + ((2 - x) * TILE_SIZE) / sz2, WINDOW_HEIGHT / 2 + (2 * TILE_SIZE) / sz2));

	int c = z * 100;
	if (c < 0) c = 0;
	if (c > 255) c = 255;
	shape.setFillColor(sf::Color(c, c, c));

	target.draw(shape);
}
