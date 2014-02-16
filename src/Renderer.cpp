#include "Renderer.h"
#include <sstream>

#include <cstdlib>

#define TILE_SIZE 100

const sf::Vector2f stoneTexCoords[4] = {sf::Vector2f(0, 0), sf::Vector2f(0, 64), sf::Vector2f(64, 64), sf::Vector2f(64, 0)};
const sf::Vector2f darkStoneTexCoords[4] = {sf::Vector2f(0, 64), sf::Vector2f(0, 128), sf::Vector2f(64, 128), sf::Vector2f(64, 64)};
const sf::Vector2f lavaTexCoords[4]  = {sf::Vector2f(64, 0), sf::Vector2f(64, 64), sf::Vector2f(128, 64), sf::Vector2f(128, 0)};
const sf::Vector2f rock1TexCoords[4]  = {sf::Vector2f(192, 0), sf::Vector2f(256, 0), sf::Vector2f(256, 64), sf::Vector2f(192, 64)};
const sf::Vector2f rock2TexCoords[4]  = {sf::Vector2f(128, 0), sf::Vector2f(192, 0), sf::Vector2f(192, 64), sf::Vector2f(128, 64)};
const sf::Vector2f rock3TexCoords[4]  = {sf::Vector2f(192, 64), sf::Vector2f(256, 64), sf::Vector2f(256, 128), sf::Vector2f(192, 128)};
const sf::Vector2f webTexCoords[4]   = {sf::Vector2f(64, 64), sf::Vector2f(64, 128), sf::Vector2f(128, 128), sf::Vector2f(128, 64)};
const sf::Vector2f brickTexCoords[4]  = {sf::Vector2f(128, 64), sf::Vector2f(192, 64), sf::Vector2f(192, 128), sf::Vector2f(128, 128)};

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

void Renderer::reset() {
	tim = 0;
	particlesSet.clear();
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

bool Renderer::renderUI(sf::RenderWindow& target, Tunnel& tunnel, Player& player, float rockZ) {
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

	if (player.fallen) {
		// YOU DIED
		sf::Text diedText("You died!", font);
		diedText.setCharacterSize(80);
		diedText.setColor(sf::Color(255, 0, 0, 255));
		diedText.setPosition(WINDOW_WIDTH / 2 - diedText.getLocalBounds().width / 2, WINDOW_HEIGHT / 2 - diedText.getLocalBounds().height / 2 - 100);
		target.draw(diedText);

		// TRY AGAIN
		sf::Text diedText2("Try again?", font);
		diedText2.setCharacterSize(48);
		diedText2.setColor(sf::Color(255, 0, 0, 255));
		diedText2.setPosition(WINDOW_WIDTH / 2 - diedText2.getLocalBounds().width / 2, WINDOW_HEIGHT / 2 - diedText2.getLocalBounds().height / 2 - 20);
		target.draw(diedText2);

		sf::Mouse mouse;
		sf::Vector2i pos = mouse.getPosition(target);

		// YES button
		sf::Text diedText3("YES", font);
		diedText3.setCharacterSize(48);
		diedText3.setPosition(WINDOW_WIDTH / 2 - diedText3.getLocalBounds().width / 2 - 85, WINDOW_HEIGHT / 2 - diedText3.getLocalBounds().height / 2 + 30);

		sf::FloatRect rect = diedText3.getGlobalBounds();
		if (pos.x > rect.left && pos.x < rect.left + rect.width &&
			pos.y > rect.top && pos.y < rect.top + rect.height) {
			diedText3.setColor(sf::Color(0, 255, 0, 255));
			if (mouse.isButtonPressed(sf::Mouse::Left)) {
                return true;
			}
		} else {
			diedText3.setColor(sf::Color(0, 255, 255, 255));
		}
		target.draw(diedText3);

		// separator
		sf::Text diedText4("/", font);
		diedText4.setCharacterSize(48);
		diedText4.setPosition(WINDOW_WIDTH / 2 - diedText4.getLocalBounds().width / 2, WINDOW_HEIGHT / 2 - diedText4.getLocalBounds().height / 2 + 30);
		diedText4.setColor(sf::Color(255, 0, 0, 255));
		target.draw(diedText4);

		// NO button
		sf::Text diedText5("NO", font);
		diedText5.setCharacterSize(48);
		diedText5.setPosition(WINDOW_WIDTH / 2 - diedText5.getLocalBounds().width / 2 + 65, WINDOW_HEIGHT / 2 - diedText5.getLocalBounds().height / 2 + 30);

		rect = diedText5.getGlobalBounds();
		if (pos.x > rect.left && pos.x < rect.left + rect.width &&
			pos.y > rect.top && pos.y < rect.top + rect.height) {
			diedText5.setColor(sf::Color(0, 255, 0, 255));
			diedText5.setString("YES");
			if (mouse.isButtonPressed(sf::Mouse::Left)) {
                return true;
			}
		} else {
			diedText5.setColor(sf::Color(0, 255, 255, 255));
		}
		target.draw(diedText5);
	}

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
	return false;
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

	sf::RectangleShape lightShape;
	lightShape.setSize(sf::Vector2f(100, 100));
	lightShape.setFillColor(sf::Color(lightRatio * 255.0f, lightRatio * 255.0f, lightRatio * 255.0f, 255));
	lightShape.setOrigin(50, 50);
	lightShape.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	target.draw(lightShape);

	sf::VertexArray vertexArray(sf::Quads, 4 * 16);

	for (int i = (int)z + 100; i >= (int)z; i--) {
		float z0 = i - z;
		if (i == tunnel->getLength() && tunnel->hasBrickWall()) {
			for (int jj = -2; jj < 2; jj++) {
				for (int kk = -2; kk < 2; kk++) {
					drawTileFlat(vertexArray, z0, jj, jj + 1, kk, kk + 1, brickTexCoords);
				}
			}
		}

		if (z0 < 60) {
			for (int j = 0; j < 4; j++) {
				if (t.get(i, j) == 1 && t.get(i + 1, j) != 1) {
					drawTileFlat(vertexArray, z0 + 1, j - 2, j - 1, 2, 3, darkStoneTexCoords);
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

		for (int j = 0; j < 4; j++) {
			if (tunnel->get(i, j) == 3) drawTileFlat(vertexArray, z0 + 0.5, j - 2, j - 1, 1., 2., rock1TexCoords);
			else if (tunnel->get(i, j) == 2) drawTileFlat(vertexArray, z0 + 0.5, j - 2, j - 1, 1., 2., rock2TexCoords);
			else if (tunnel->get(i, j) == 4) drawTileFlat(vertexArray, z0 + 0.5, j - 2, j - 1, 1., 2., rock3TexCoords);
		}

		if (tunnel->getWeb(i, 0)) {
			drawTileFlat(vertexArray, z0 + 1, -1, -2, -2, -1, webTexCoords);
		}
		if (tunnel->getWeb(i, 1)) {
			drawTileFlat(vertexArray, z0 + 1, 1, 2, -2, -1, webTexCoords);
		}

		target.draw(vertexArray, states);
	}

	static float sz1 = std::sqrt(1.5);
	sf::Sprite sprite;

	int frame = 0 + (int)z % 3;
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(frame * 64, 0), sf::Vector2i(64, 128)));
	sprite.setTexture(playerTexture);
	sprite.setOrigin(32, 0);
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
		y1 += .2f;
		y2 += .2f;
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
