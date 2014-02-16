#include "Renderer.h"
#include <sstream>

#define TILE_SIZE 100

const sf::Vector2f stoneTexCoords[4] = {sf::Vector2f(0, 0), sf::Vector2f(0, 64), sf::Vector2f(64, 64), sf::Vector2f(64, 0)};
const sf::Vector2f lavaTexCoords[4]  = {sf::Vector2f(64, 0), sf::Vector2f(64, 64), sf::Vector2f(128, 64), sf::Vector2f(128, 0)};
const sf::Vector2f rockTexCoords[4]  = {sf::Vector2f(128, 0), sf::Vector2f(192, 0), sf::Vector2f(192, 64), sf::Vector2f(128, 64)};

Renderer::Renderer() {
	// Tileset
	tileTexture.loadFromFile("assets/tile.png");
	playerTexture.loadFromFile("assets/player.png");

	// UI
	gauge.loadFromFile("assets/gauge.png");
	playerInd.loadFromFile("assets/guy_marker.png");
	boulderInd.loadFromFile("assets/boulder_marker.png");
    font.loadFromFile("assets/goldbox.ttf");
}

void Renderer::renderUI(sf::RenderTarget& target, Tunnel& tunnel, Player& player) {
	float boulderPercent = 0.0f;
	boulderPercent = boulderPercent > 1.0f ? 1.0f : boulderPercent;

	float playerPercent = player.z / tunnel.getLength();
	playerPercent = playerPercent > 1.0f ? 1.0f : playerPercent;

	float startX = (WINDOW_WIDTH - gauge.getSize().x) / 2;
	float endX = startX + gauge.getSize().x;
    float startY = 16.0f;

	sf::Sprite gaugeSprite(gauge);
	gaugeSprite.setPosition(startX, startY);
	target.draw(gaugeSprite);

	sf::Sprite playerIndSprite(playerInd);
	playerIndSprite.setOrigin(playerInd.getSize().x / 2, 0);
	playerIndSprite.setPosition(startX + playerPercent * gauge.getSize().x, startY);
	target.draw(playerIndSprite);

	sf::Sprite boulderIndSprite(boulderInd);
	boulderIndSprite.setOrigin(boulderInd.getSize().x / 2, 0);
	boulderIndSprite.setPosition(startX + boulderPercent * gauge.getSize().x, startY);
	target.draw(boulderIndSprite);

	// Render text
	std::stringstream ss;
	ss << (int)player.z << "m";
	sf::Text text(ss.str(), font);
	text.setCharacterSize(20);
	text.setPosition(startX - text.getLocalBounds().width - 24, startY + 5);
	target.draw(text);

	std::stringstream ss2;
	ss2 << (int)(playerPercent * 100) << "%";
	sf::Text text2(ss2.str(), font);
	text2.setCharacterSize(20);
	text2.setPosition(endX + 24, startY + 5);
	target.draw(text2);

	if (player.z < 20) {
		float alpha = 1.0f;
		if (player.z > 12) {
			alpha = 1.0f - (player.z - 12.0f) / 8.0f;
		}

		sf::Text infoText("Escape the", font);
		infoText.setCharacterSize(60);
		infoText.setColor(sf::Color(255, 0, 0, alpha * 255));
		infoText.setPosition(WINDOW_WIDTH / 2 - infoText.getLocalBounds().width / 2, WINDOW_HEIGHT / 2 - infoText.getLocalBounds().height / 2 - 30);
		target.draw(infoText);

		sf::Text infoText2("Boulder!", font);
		infoText2.setCharacterSize(60);
		infoText2.setColor(sf::Color(255, 0, 0, alpha * 255));
		infoText2.setPosition(WINDOW_WIDTH / 2 - infoText2.getLocalBounds().width / 2, WINDOW_HEIGHT / 2 - infoText2.getLocalBounds().height / 2 + 30);
		target.draw(infoText2);
	}

	// If playerPercent > 1.0f, YOU WIN
	if (playerPercent == 1.0f) {
		sf::Text winText("YOU WIN!", font);
		winText.setCharacterSize(80);
		winText.setColor(sf::Color(255, 0, 0, 255));
		winText.setPosition(WINDOW_WIDTH / 2 - winText.getLocalBounds().width / 2, WINDOW_HEIGHT / 2 - winText.getLocalBounds().height / 2);
		target.draw(winText);
	}
}

void Renderer::render(sf::RenderTarget& target, Tunnel& t, Player& p) {
	float z = p.z;

	sf::RenderStates states;
	states.texture = &tileTexture;
	states.transform.translate(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	states.transform.scale(2.5, 2.5);

	tunnel = &t;
	player = &p;

	lightRatio = 0.0f;
	if (z > tunnel->getLength() - 20 && z < tunnel->getLength()) {
        lightRatio = (z - tunnel->getLength() + 20) / 20.0f;
	} else if (z >= tunnel->getLength()) {
        lightRatio = 1.0f;
	}
	if (lightRatio > 1.0f || lightRatio < 0.0f) {
		printf("%f", lightRatio);
	}

	sf::RectangleShape lightShape;
	lightShape.setSize(sf::Vector2f(100, 100));
	lightShape.setFillColor(sf::Color(lightRatio * 255.0f, lightRatio * 255.0f, lightRatio * 255.0f, 255));
	lightShape.setOrigin(50, 50);
	lightShape.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	target.draw(lightShape);

	for (int i = (int)z + 100; i >= (int)z; i--) {

		sf::VertexArray vertexArray(sf::Quads, 4 * 16);
		float z0 = i - z;

		if (z0 < 60) {
			for (int j = 0; j < 4; j++) {
				if (t.get(i, j) == 1 && t.get(i + 1, j) != 1) {
					drawTileFlat(vertexArray, z0 + 1, j - 2, j - 1, 2, 3, stoneTexCoords);
				}
			}
		}

		drawTile(vertexArray, z0, z0 + 1, -2, -1,  2,  2, 0, i);
		drawTile(vertexArray, z0, z0 + 1,  1,  2,  2,  2, 3, i);
		drawTile(vertexArray, z0, z0 + 1, -1,  0,  2,  2, 1, i);
		drawTile(vertexArray, z0, z0 + 1,  0,  1,  2,  2, 2, i);
		drawTile(vertexArray, z0, z0 + 1, -2, -1, -2, -2);
		drawTile(vertexArray, z0, z0 + 1, -1,  0, -2, -2);
		drawTile(vertexArray, z0, z0 + 1,  0,  1, -2, -2);
		drawTile(vertexArray, z0, z0 + 1,  1,  2, -2, -2);
		drawTile(vertexArray, z0, z0 + 1,  2,  2, -2, -1);
		drawTile(vertexArray, z0, z0 + 1,  2,  2, -1,  0);
		drawTile(vertexArray, z0, z0 + 1,  2,  2,  0,  1);
		drawTile(vertexArray, z0, z0 + 1,  2,  2,  1,  2);
		drawTile(vertexArray, z0, z0 + 1, -2, -2, -2, -1);
		drawTile(vertexArray, z0, z0 + 1, -2, -2, -1,  0);
		drawTile(vertexArray, z0, z0 + 1, -2, -2,  0,  1);
		drawTile(vertexArray, z0, z0 + 1, -2, -2,  1,  2);

		if (tunnel->get(i, 0) == 2) drawTileFlat(vertexArray, z0 + 0.5, -2, -1, 1.1, 2.1, rockTexCoords);
		if (tunnel->get(i, 1) == 2) drawTileFlat(vertexArray, z0 + 0.5, -1,  0, 1.1, 2.1, rockTexCoords);
		if (tunnel->get(i, 2) == 2) drawTileFlat(vertexArray, z0 + 0.5,  0,  1, 1.1, 2.1, rockTexCoords);
		if (tunnel->get(i, 3) == 2) drawTileFlat(vertexArray, z0 + 0.5,  1,  2, 1.1, 2.1, rockTexCoords);

		target.draw(vertexArray, states);
	}

	static float sz1 = std::sqrt(1.5);
	sf::Sprite sprite;
	sprite.setTexture(playerTexture);
	sprite.setOrigin(playerTexture.getSize().x / 2, 0);
	sprite.setPosition(p.x * TILE_SIZE / std::sqrt(2.5), (0 - p.y) * TILE_SIZE / sz1);
	target.draw(sprite, states.transform);
}

void Renderer::drawTileFlat(sf::VertexArray& vertexArray, float z, int x1, int x2, float y1, float y2, const sf::Vector2f* texCoords) {
	sf::ConvexShape shape;
	shape.setPointCount(4);

	float sz = std::sqrt(z + 1.5);

	sf::Vertex vert1(sf::Vector2f((x1 * TILE_SIZE) / sz, (y1 * TILE_SIZE) / sz),  texCoords[0]);
	sf::Vertex vert2(sf::Vector2f((x2 * TILE_SIZE) / sz, (y1 * TILE_SIZE) / sz + 0.01),  texCoords[1]);
	sf::Vertex vert3(sf::Vector2f((x2 * TILE_SIZE) / sz, (y2 * TILE_SIZE) / sz + 0.01), texCoords[2]);
	sf::Vertex vert4(sf::Vector2f((x1 * TILE_SIZE) / sz, (y2 * TILE_SIZE) / sz), texCoords[3]);
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

void Renderer::drawTile(sf::VertexArray& vertexArray, float z1, float z2, int x1, int x2, float y1, float y2, int x, int z) {

	sf::ConvexShape shape;
	shape.setPointCount(4);
	//shape.setTexture(&texture);
	//shape.setTextureRect(sf::IntRect(0, 0, 64, 64));

	float sz = std::sqrt(z1 + 1.5);
	float sz2 = std::sqrt(z2 + 1.5);

	const sf::Vector2f* texCoords;
	if (x != -1 && tunnel->get(z, x) == 1) {
		y1 += .1f;
		y2 += .1f;
		texCoords = lavaTexCoords;

		if (x == 0 || (x == 1 && tunnel->get(z, 0) == 0)) {
			drawTile(vertexArray, z1, z2, x1, x1, 2, 3);
		} else if (x == 3 || (x == 2 && tunnel->get(z, 3) == 0)) {
			drawTile(vertexArray, z1, z2, x2, x2, 2, 3);
		}
	} else {
		texCoords = stoneTexCoords;
	}

	sf::Vertex vert1(sf::Vector2f((x1 * TILE_SIZE) / sz,  (y1 * TILE_SIZE) / sz),  texCoords[0]);
	sf::Vertex vert2(sf::Vector2f((x2 * TILE_SIZE) / sz,  (y2 * TILE_SIZE) / sz + 0.01),  texCoords[1]);
	sf::Vertex vert3(sf::Vector2f((x2 * TILE_SIZE) / sz2, (y2 * TILE_SIZE) / sz2 + 0.01), texCoords[2]);
	sf::Vertex vert4(sf::Vector2f((x1 * TILE_SIZE) / sz2, (y1 * TILE_SIZE) / sz2), texCoords[3]);
	int c = (int)(sz * 20 + 50);
	if (c < 0) c = 0;
	if (c > 255) c = 255;

	float add = 1.0f - lightRatio;
	vert1.color = sf::Color(255 - add * c, 255 - add * c, 255 - add * c);
	vert2.color = sf::Color(255 - add * c, 255 - add * c, 255 - add * c);
	vert3.color = sf::Color(255 - add * c, 255 - add * c, 255 - add * c);
	vert4.color = sf::Color(255 - add * c, 255 - add * c, 255 - add * c);

	vertexArray.append(vert1);
	vertexArray.append(vert2);
	vertexArray.append(vert3);
	vertexArray.append(vert4);
}
