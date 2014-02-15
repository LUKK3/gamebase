#include "Renderer.h"

#define TILE_SIZE 100

Renderer::Renderer() {
	// Tileset
	tileTexture.loadFromFile("assets/tile.png");
	tileTexture.setSmooth(true);
	playerTexture.loadFromFile("assets/player.png");

	// UI
	gauge.loadFromFile("assets/gauge.png");
	playerInd.loadFromFile("assets/guy_marker.png");
	boulderInd.loadFromFile("assets/boulder_marker.png");
    font.loadFromFile("assets/goldbox.ttf");
}

void Renderer::renderUI(sf::RenderTarget& target) {
	float startX = (WINDOW_WIDTH - gauge.getSize().x) / 2;
	float endX = startX + gauge.getSize().x;
    float startY = 16.0f;

	sf::Sprite gaugeSprite(gauge);
	gaugeSprite.setPosition(startX, startY);
	target.draw(gaugeSprite);

	sf::Sprite playerIndSprite(playerInd);
	playerIndSprite.setPosition(startX - playerInd.getSize().x / 2, startY);
	target.draw(playerIndSprite);

	sf::Sprite boulderIndSprite(boulderInd);
	boulderIndSprite.setPosition(endX - boulderInd.getSize().x / 2, startY);
	target.draw(boulderIndSprite);

	// Render text
	sf::Text text("104 m", font);
	text.setCharacterSize(20);
	text.setPosition(startX - text.getLocalBounds().width - 24, startY + 5);
	target.draw(text);

	sf::Text text2("16.4 %", font);
	text2.setCharacterSize(20);
	text2.setPosition(endX + 24, startY + 5);
	target.draw(text2);
}

void Renderer::render(sf::RenderTarget& target, Tunnel& tunnel, Player& player) {

	float z = player.z;

	sf::RenderStates states;
	states.texture = &tileTexture;
	states.transform.translate(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	states.transform.scale(2.5, 2.5);

	for (int i = (int)z; i < (int)z + 100; i++) {

		bool color = false;
		if (i - z > 20) {
			//states.texture = NULL;
			color = true;
		}

		sf::VertexArray vertexArray(sf::Quads, 4 * 16);

		drawTile(vertexArray, i - z, -2, -1,  2,  2, color);
		drawTile(vertexArray, i - z, -1,  0,  2,  2, color);
		drawTile(vertexArray, i - z,  0,  1,  2,  2, color);
		drawTile(vertexArray, i - z,  1,  2,  2,  2, color);
		drawTile(vertexArray, i - z, -2, -1, -2, -2, color);
		drawTile(vertexArray, i - z, -1,  0, -2, -2, color);
		drawTile(vertexArray, i - z,  0,  1, -2, -2, color);
		drawTile(vertexArray, i - z,  1,  2, -2, -2, color);
		drawTile(vertexArray, i - z,  2,  2, -2, -1, color);
		drawTile(vertexArray, i - z,  2,  2, -1,  0, color);
		drawTile(vertexArray, i - z,  2,  2,  0,  1, color);
		drawTile(vertexArray, i - z,  2,  2,  1,  2, color);
		drawTile(vertexArray, i - z, -2, -2, -2, -1, color);
		drawTile(vertexArray, i - z, -2, -2, -1,  0, color);
		drawTile(vertexArray, i - z, -2, -2,  0,  1, color);
		drawTile(vertexArray, i - z, -2, -2,  1,  2, color);

		target.draw(vertexArray, states);
	}

	static float sz = std::sqrt(0.5);
	sf::Sprite sprite;
	sprite.setTexture(playerTexture);
	sprite.setPosition(player.x * TILE_SIZE / sz, -2 / sz);
	target.draw(sprite, states.transform);
}

sf::Vector2f texCoords[4] = {sf::Vector2f(0, 0), sf::Vector2f(0, 64), sf::Vector2f(64, 64), sf::Vector2f(64, 0)};
void Renderer::drawTile(sf::VertexArray& vertexArray, float z, int x1, int x2, int y1, int y2, bool color) {

	sf::ConvexShape shape;
	shape.setPointCount(4);
	//shape.setTexture(&texture);
	//shape.setTextureRect(sf::IntRect(0, 0, 64, 64));

	int hw = 0;//WINDOW_WIDTH / 2;
	int hh = 0;//WINDOW_HEIGHT / 2;
	float sz = std::sqrt(z + 1.5);
	float sz2 = std::sqrt(z + 2.5);

	sf::Vertex vert1(sf::Vector2f(hw + (x1 * TILE_SIZE) / sz,  hh / 2 + (y1 * TILE_SIZE) / sz),  texCoords[0]);
	sf::Vertex vert2(sf::Vector2f(hw + (x2 * TILE_SIZE) / sz,  hh / 2 + (y2 * TILE_SIZE) / sz + 0.01),  texCoords[1]);
	sf::Vertex vert3(sf::Vector2f(hw + (x2 * TILE_SIZE) / sz2, hh / 2 + (y2 * TILE_SIZE) / sz2 + 0.01), texCoords[2]);
	sf::Vertex vert4(sf::Vector2f(hw + (x1 * TILE_SIZE) / sz2, hh / 2 + (y1 * TILE_SIZE) / sz2), texCoords[3]);
	int c = (int)(sz * 20 + 50);
	if (c < 0) c = 0;
	if (c > 255) c = 255;
	vert1.color = sf::Color(255 - c, 255 - c, 255 - c);
	vert2.color = sf::Color(255 - c, 255 - c, 255 - c);
	vert3.color = sf::Color(255 - c, 255 - c, 255 - c);
	vert4.color = sf::Color(255 - c, 255 - c, 255 - c);

	vertexArray.append(vert1);
	vertexArray.append(vert2);
	vertexArray.append(vert3);
	vertexArray.append(vert4);
}
