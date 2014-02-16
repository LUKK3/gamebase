#include "Renderer.h"
#include <sstream>

#include <cstdlib>

#define TILE_SIZE 100

const sf::Vector2f stoneTexCoords[4] = {sf::Vector2f(0, 0), sf::Vector2f(0, 64), sf::Vector2f(64, 64), sf::Vector2f(64, 0)};
const sf::Vector2f lavaTexCoords[4]  = {sf::Vector2f(64, 0), sf::Vector2f(64, 64), sf::Vector2f(128, 64), sf::Vector2f(128, 0)};
const sf::Vector2f rockTexCoords[4]  = {sf::Vector2f(128, 0), sf::Vector2f(192, 0), sf::Vector2f(192, 64), sf::Vector2f(128, 64)};

Renderer::Renderer() {
	// Tileset
	tileTexture.loadFromFile("assets/tile.png");
	playerTexture.loadFromFile("assets/player.png");
	rockTexture.loadFromFile("assets/boulder.png");

	// UI
	gauge.loadFromFile("assets/gauge.png");
	playerInd.loadFromFile("assets/guy_marker.png");
	boulderInd.loadFromFile("assets/boulder_marker.png");
    font.loadFromFile("assets/goldbox.ttf");

    tim = 0;
}

void Renderer::addParticles(int num, sf::Color color, sf::Vector3f position) {
	particlesSet.emplace_back(num, color, position);
}

void Renderer::update(float diff) {
	for (auto iter = particlesSet.begin(); iter != particlesSet.end(); ++iter) {
		iter->update(diff);
	}
	tim += diff;
}

void Renderer::renderUI(sf::RenderTarget& target, Tunnel& tunnel, Player& player, float rockZ) {
	float boulderPercent = rockZ / tunnel.getLength();
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

	// If playerPercent > 1.0f, YOU WIN
	if (playerPercent == 1.0f) {
		sf::Text winText("YOU WIN!", font);
		winText.setCharacterSize(80);
		winText.setColor(sf::Color(255, 0, 0, 255));
		winText.setPosition(WINDOW_WIDTH / 2 - winText.getLocalBounds().width / 2, WINDOW_HEIGHT / 2 - winText.getLocalBounds().height / 2);
		target.draw(winText);
	}
}

void Renderer::render(sf::RenderTarget& target, Tunnel& t, Player& p, float rockZ) {
	float z = p.z;

	sf::RenderStates states;
	states.texture = &tileTexture;
	states.transform.translate(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	float quake = 1 / std::max(std::abs(z - rockZ), 1.f);
	states.transform.translate(rand() / (float)RAND_MAX * quake * 30, rand() / (float)RAND_MAX * quake * 30);

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

		if (tunnel->get(i, 0) == 2) drawTileFlat(vertexArray, z0 + 0.5, -2, -1, 1., 2., rockTexCoords);
		if (tunnel->get(i, 1) == 2) drawTileFlat(vertexArray, z0 + 0.5, -1,  0, 1., 2., rockTexCoords);
		if (tunnel->get(i, 2) == 2) drawTileFlat(vertexArray, z0 + 0.5,  0,  1, 1., 2., rockTexCoords);
		if (tunnel->get(i, 3) == 2) drawTileFlat(vertexArray, z0 + 0.5,  1,  2, 1., 2., rockTexCoords);

		target.draw(vertexArray, states);
	}

	static float sz1 = std::sqrt(1.5);
	sf::Sprite sprite;
	sprite.setTexture(playerTexture);
	sprite.setOrigin(playerTexture.getSize().x / 2, 0);
	sprite.setPosition(p.x * TILE_SIZE / std::sqrt(2.5), (0 - p.y) * TILE_SIZE / sz1);
	target.draw(sprite, states.transform);

	if (rockZ > z - 0.5) {

		int frame = (int)(tim * 30);
		int x = frame % 10;
		int y = (frame % 100) / 10;

		sf::Sprite sprite;
		sprite.setTexture(rockTexture);
		int siz = rockTexture.getSize().x / 10;
		sprite.setTextureRect(sf::IntRect(siz * x, siz * y, siz, siz));
		sprite.setOrigin(rockTexture.getSize().x / 20, rockTexture.getSize().y / 20);
		float sz = std::sqrt(-z + rockZ + 1.5);
		float s = 1 / sz * 2.5;
		sprite.setScale(s, s);

		int c = (int)(sz * 20 + 50);
		if (c < 0) c = 0;
		if (c > 255) c = 255;
		sprite.setColor(sf::Color(255 - c, 255 - c, 255 - c));

		target.draw(sprite, states.transform);
	}

	for (size_t i = 0; i < particlesSet.size(); i++) {
		Particles& particles = particlesSet[i];
		//float sz = std::sqrt(p.z - particles.getZ() + 1.5);
		float sz = std::sqrt(-z + particles.getZ() + 5);
		for (int j = 0; j < particles.getNumParticles(); j++) {
			const Particle& p = particles.getParticle(j);
			if (p.pos.y > 2) continue;
			sf::Vector2f pos(p.pos.x * TILE_SIZE / sz, p.pos.y * TILE_SIZE / sz);
			sf::RectangleShape rect;
			rect.setPosition(pos);
			rect.setSize(sf::Vector2f(10 / sz, 10 / sz));

			float c = 1 - sz * 0.1 + 0.2;
			if (c < 0) c = 0;
			if (c > 1) c = 1;

			rect.setFillColor(sf::Color(p.color.r * c, p.color.g * c, p.color.b * c));
			target.draw(rect, states.transform);
		}
	}
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

	// ratio = 0: 255 - c
	// ratio = 1: c
	// delta: 2c - 255
	//if (std::floor(player->z + 1) == z && std::floor(player->x + 2) == x) c = 0;
	float add = 1.0f - lightRatio; //lightRatio * (2 * c - 255);
	vert1.color = sf::Color(255 - add * c, 255 - add * c, 255 - add * c);
	vert2.color = sf::Color(255 - add * c, 255 - add * c, 255 - add * c);
	vert3.color = sf::Color(255 - add * c, 255 - add * c, 255 - add * c);
	vert4.color = sf::Color(255 - add * c, 255 - add * c, 255 - add * c);

	vertexArray.append(vert1);
	vertexArray.append(vert2);
	vertexArray.append(vert3);
	vertexArray.append(vert4);
}
