#include "Renderer.h"
#include <sstream>

#include <cstdlib>

#define TILE_SIZE 100

const sf::Color mainFontColor = sf::Color(255, 120, 0, 255);
const sf::Color subFontColor = sf::Color(220, 220, 220, 255);

const sf::Vector2f stoneTexCoords[4] = {sf::Vector2f(0, 0), sf::Vector2f(0, 64), sf::Vector2f(64, 64), sf::Vector2f(64, 0)};
const sf::Vector2f dungeonTexCoords[4] = {sf::Vector2f(0, 192), sf::Vector2f(64, 192), sf::Vector2f(64, 256), sf::Vector2f(0, 256)};
const sf::Vector2f blueTexCoords[4] = {sf::Vector2f(64, 192), sf::Vector2f(128, 192), sf::Vector2f(128, 256), sf::Vector2f(64, 256)};
const sf::Vector2f cumonTexCoords[4] = {sf::Vector2f(192, 192), sf::Vector2f(256, 192), sf::Vector2f(256, 256), sf::Vector2f(192, 256)};
const sf::Vector2f darkStoneTexCoords[4] = {sf::Vector2f(0, 64), sf::Vector2f(0, 128), sf::Vector2f(64, 128), sf::Vector2f(64, 64)};
const sf::Vector2f lavaTexCoords[4]  = {sf::Vector2f(64, 0), sf::Vector2f(64, 64), sf::Vector2f(128, 64), sf::Vector2f(128, 0)};
const sf::Vector2f rock1TexCoords[4]  = {sf::Vector2f(192, 0), sf::Vector2f(256, 0), sf::Vector2f(256, 64), sf::Vector2f(192, 64)};
const sf::Vector2f rock2TexCoords[4]  = {sf::Vector2f(128, 0), sf::Vector2f(192, 0), sf::Vector2f(192, 64), sf::Vector2f(128, 64)};
const sf::Vector2f rock3TexCoords[4]  = {sf::Vector2f(192, 64), sf::Vector2f(256, 64), sf::Vector2f(256, 128), sf::Vector2f(192, 128)};
const sf::Vector2f webTexCoords[4]   = {sf::Vector2f(64, 64), sf::Vector2f(64, 128), sf::Vector2f(128, 128), sf::Vector2f(128, 64)};
const sf::Vector2f brickTexCoords[4]  = {sf::Vector2f(128, 64), sf::Vector2f(192, 64), sf::Vector2f(192, 128), sf::Vector2f(128, 128)};
const sf::Vector2f bubbleTexCoords[4]  = {sf::Vector2f(192, 128), sf::Vector2f(256, 128), sf::Vector2f(256, 192), sf::Vector2f(192, 192)};
const sf::Vector2f cgemTexCoords[4]   = {sf::Vector2f(0, 128), sf::Vector2f(64, 128), sf::Vector2f(64, 192), sf::Vector2f(0, 192)};
const sf::Vector2f rgemTexCoords[4]   = {sf::Vector2f(64, 128), sf::Vector2f(128, 128), sf::Vector2f(128, 192), sf::Vector2f(64, 192)};
const sf::Vector2f ggemTexCoords[4]   = {sf::Vector2f(128, 128), sf::Vector2f(192, 128), sf::Vector2f(192, 192), sf::Vector2f(128, 192)};
const sf::Vector2f bridgeTexCoords[4] = {sf::Vector2f(128, 192), sf::Vector2f(192, 192), sf::Vector2f(192, 256), sf::Vector2f(128, 256)};
const sf::Vector2f timeTexCoords[4]   = {sf::Vector2f(256, 0), sf::Vector2f(320, 0), sf::Vector2f(320, 64), sf::Vector2f(256, 64)};
const sf::Vector2f rubyTexCoords[4]   = {sf::Vector2f(320, 0), sf::Vector2f(384, 0), sf::Vector2f(384, 64), sf::Vector2f(320, 64)};
const sf::Vector2f hoverTexCoords[4]  = {sf::Vector2f(384, 0), sf::Vector2f(446, 0), sf::Vector2f(446, 64), sf::Vector2f(384, 64)};
const sf::Vector2f obsTexCoords[4] = {sf::Vector2f(256, 64), sf::Vector2f(320, 64), sf::Vector2f(320, 128), sf::Vector2f(256, 128)};

Renderer::Renderer() {
	// Tileset
	tileTexture.loadFromFile("assets/tile.png");
	playerTexture.loadFromFile("assets/player.png");
	rockTexture.loadFromFile("assets/boulder.png");

	// UI
	gauge.loadFromFile("assets/gauge.png");
	playerInd.loadFromFile("assets/guy_marker.png");
	boulderInd.loadFromFile("assets/boulder_marker.png");
    font.loadFromFile("assets/comicbook.ttf");

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

void renderOutline(sf::RenderWindow& window, sf::Text& text, sf::Color col) {
	sf::Color tmp = text.getColor();
	text.setColor(col);

	text.setOrigin(2, 2);
	window.draw(text);
	text.setOrigin(-2, 2);
	window.draw(text);
	text.setOrigin(2, -2);
	window.draw(text);
	text.setOrigin(-2, -2);
	window.draw(text);
	text.setOrigin(-2, 0);
	window.draw(text);
	text.setOrigin(2, 0);
	window.draw(text);
	text.setOrigin(0, -2);
	window.draw(text);
	text.setOrigin(0, 2);
	window.draw(text);

	text.setColor(tmp);
	text.setOrigin(0, 0);
}

bool Renderer::renderUI(sf::RenderWindow& target, Tunnel& tunnel, Player& player, float rockZ) {
	float boulderPercent = rockZ / tunnel.getLength();
	boulderPercent = boulderPercent > 1.0f ? 1.0f : boulderPercent;
	boulderPercent = boulderPercent < 0.0f ? 0.0f : boulderPercent;

	float playerPercent = player.z / tunnel.getLength();
	playerPercent = playerPercent > 1.0f ? 1.0f : playerPercent;
	playerPercent = playerPercent < 0.0f ? 0.0f : playerPercent;

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
	renderOutline(target, text, sf::Color(0, 0, 0, 255));
	target.draw(text);

	std::stringstream ss2;
	ss2 << (int)(playerPercent * 100) << "%";
	sf::Text text2(ss2.str(), font);
	text2.setCharacterSize(20);
	text2.setPosition(endX + 24, startY + 5);
	renderOutline(target, text2, sf::Color(0, 0, 0, 255));
	target.draw(text2);

	std::stringstream ss0;
	ss0 << "Score: " << player.score;
	if (player.score < 5) {
		ss0 << " (need " << 5 - player.score << " more)";
	}
	sf::Text text0(ss0.str(), font);
	text0.setCharacterSize(30);
	text0.setPosition(startX - 24, startY + 30);
	if (player.score >= 5) text0.setColor(sf::Color::Yellow);
	renderOutline(target, text0, sf::Color(0, 0, 0, 255));
	target.draw(text0);

	if (player.fallen) {
		// YOU DIED
		std::string diedString = (player.score >= 5 && player.z > tunnel.getLength() - 2) ? "You... won?" : "You lose!";
		sf::Text diedText(diedString, font);
		diedText.setCharacterSize(80);
		diedText.setPosition(WINDOW_WIDTH / 2 - diedText.getLocalBounds().width / 2, WINDOW_HEIGHT / 2 - diedText.getLocalBounds().height / 2 - 120);
		diedText.setColor(mainFontColor);
		renderOutline(target, diedText, sf::Color(0, 0, 0, 255));
		target.draw(diedText);

		// TRY AGAIN
		diedString = (player.score >= 5 && player.z > tunnel.getLength() - 2) ? "Go to next level?" : "Try again?";
		sf::Text diedText2(diedString, font);
		diedText2.setCharacterSize(48);
		diedText2.setPosition(WINDOW_WIDTH / 2 - diedText2.getLocalBounds().width / 2, WINDOW_HEIGHT / 2 - diedText2.getLocalBounds().height / 2 - 20);
		diedText2.setColor(subFontColor);
		renderOutline(target, diedText2, sf::Color(0, 0, 0, 255));
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
		renderOutline(target, diedText3, sf::Color(0, 0, 0, 255));
		target.draw(diedText3);

		// separator
		sf::Text diedText4("/", font);
		diedText4.setCharacterSize(48);
		diedText4.setPosition(WINDOW_WIDTH / 2 - diedText4.getLocalBounds().width / 2, WINDOW_HEIGHT / 2 - diedText4.getLocalBounds().height / 2 + 30);
		diedText4.setColor(subFontColor);
		renderOutline(target, diedText4, sf::Color(0, 0, 0, 255));
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
		renderOutline(target, diedText5, sf::Color(0, 0, 0, 255));
		target.draw(diedText5);
	}

	if (player.z < 20) {
		float alpha = 1.0f;
		if (player.z > 12) {
			alpha = 1.0f - (player.z - 12.0f) / 8.0f;
		}

		sf::Text infoText("Escape the", font);
		infoText.setCharacterSize(60);
		infoText.setColor(sf::Color(mainFontColor.r, mainFontColor.g, mainFontColor.b, alpha * 255));
		infoText.setPosition(WINDOW_WIDTH / 2 - infoText.getLocalBounds().width / 2, WINDOW_HEIGHT / 2 - infoText.getLocalBounds().height / 2 - 50);
		renderOutline(target, infoText, sf::Color(0, 0, 0, alpha * 255));
		target.draw(infoText);

		sf::Text infoText2("Boulder!", font);
		infoText2.setCharacterSize(60);
		infoText2.setColor(sf::Color(mainFontColor.r, mainFontColor.g, mainFontColor.b, alpha * 255));
		infoText2.setPosition(WINDOW_WIDTH / 2 - infoText2.getLocalBounds().width / 2, WINDOW_HEIGHT / 2 - infoText2.getLocalBounds().height / 2 + 10);
		renderOutline(target, infoText2, sf::Color(0, 0, 0, alpha * 255));
		target.draw(infoText2);

		sf::Text infoText3("(And collect 5 gems)", font);
		infoText3.setCharacterSize(36);
		infoText3.setColor(sf::Color(subFontColor.r, subFontColor.g, subFontColor.b, alpha * 255));
		infoText3.setPosition(WINDOW_WIDTH / 2 - infoText3.getLocalBounds().width / 2, WINDOW_HEIGHT / 2 - infoText3.getLocalBounds().height / 2 + 70);
		renderOutline(target, infoText3, sf::Color(0, 0, 0, alpha * 255));
		target.draw(infoText3);
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
				if ((t.get(i, j) == 1 || t.get(i, j) == 5) && !(t.get(i + 1, j) == 1 || t.get(i + 1, j) == 5)) {
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

		for (int j = 0; j < 4; j++) {
			if (tunnel->get(i, j + 5) == 1) drawTileFlat(vertexArray, z0 + 0.5, j - 2, j - 1, -.5, .5, rgemTexCoords);
			else if (tunnel->get(i, j + 5) == 2) drawTileFlat(vertexArray, z0 + 0.5, j - 2, j - 1, -.5, .5, ggemTexCoords);
			else if (tunnel->get(i, j + 5) == 3) drawTileFlat(vertexArray, z0 + 0.5, j - 2, j - 1, -.5, .5, cgemTexCoords);
			else if (tunnel->get(i, j + 5) == 4) drawTileFlat(vertexArray, z0 + 0.5, j - 2, j - 1, -.5, .5, timeTexCoords);
			else if (tunnel->get(i, j + 5) == 5) drawTileFlat(vertexArray, z0 + 0.5, j - 2, j - 1, -.5, .5, rubyTexCoords);
			else if (tunnel->get(i, j + 5) == 6) drawTileFlat(vertexArray, z0 + 0.5, j - 2, j - 1, -.5, .5, hoverTexCoords);
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
	if (p.y > 0.01) frame = 1 + ((int)z / 10) % 2;
	sprite.setTextureRect(sf::IntRect(sf::Vector2i(frame * 64, 0), sf::Vector2i(64, 128)));
	sprite.setTexture(playerTexture);
	sprite.setOrigin(32, 0);
	sprite.setPosition(p.x * TILE_SIZE / std::sqrt(2.5), (0 - p.y) * TILE_SIZE / sz1);
	if (p.feather > 1 || (p.feather > 0 && frame % 2)) {
		sprite.setColor(sf::Color(255, 127, 0, 127));
	} else {
		sprite.setColor(sf::Color(255, 255, 255, 127));
	}
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

	if (p.z > tunnel->getLength() - 5) {
		sf::VertexArray bubbleVA(sf::Quads, 4 * 1);
		drawTileFlat(bubbleVA, 1, p.x, p.x + 1, -p.y -1, -p.y , bubbleTexCoords);
		target.draw(bubbleVA, states);
	}
}

void Renderer::drawTileFlat(sf::VertexArray& vertexArray, float z, float x1, float x2, float y1, float y2, const sf::Vector2f* texCoords) {
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

void Renderer::drawTile(sf::VertexArray& vertexArray, float z1, float z2, float x1, float x2, float y1, float y2, int x, int z, bool lava) {

	sf::ConvexShape shape;
	shape.setPointCount(4);
	//shape.setTexture(&texture);
	//shape.setTextureRect(sf::IntRect(0, 0, 64, 64));
	float sz = std::sqrt(z1 + 1.5);
	float sz2 = std::sqrt(z2 + 1.5);

	const sf::Vector2f* texCoords;
	if (lava || (x != -1 && tunnel->get(z, x) == 1)) {
		y1 += .2f;
		y2 += .2f;
		texCoords = lavaTexCoords;

		if (x == 0 || (x == 1 && (tunnel->get(z, 0) != 1 && tunnel->get(z, 0) != 5))) {
			drawTile(vertexArray, z1, z2, x1, x1, 2, 3);
		} else if (x == 3 || (x == 2 && (tunnel->get(z, 3) != 1 && tunnel->get(z, 3) != 5))) {
			drawTile(vertexArray, z1, z2, x2, x2, 2, 3);
		}
	} else if (tunnel->get(z, x) == 5) {
		drawTile(vertexArray, z1, z2, x1, x2, y1, y2, x, z, true);
		texCoords = bridgeTexCoords;
	} else {
		if (tunnel->getDifficulty() == 1) {
			texCoords = dungeonTexCoords;
		} else if (tunnel->getDifficulty() == 2) {
			texCoords = blueTexCoords;
		} else if (tunnel->getDifficulty() == 3) {
			texCoords = obsTexCoords;
		} else if (tunnel->getDifficulty() == 4) {
			texCoords = cumonTexCoords;
		} else if (tunnel->getDifficulty() == 5) {
			texCoords = brickTexCoords;
		} else {
			texCoords = stoneTexCoords;
		}
	}

	sf::Vertex vert1(sf::Vector2f((x1 * TILE_SIZE) / sz,  (y1 * TILE_SIZE) / sz),  texCoords[0]);
	sf::Vertex vert2(sf::Vector2f((x2 * TILE_SIZE) / sz,  (y2 * TILE_SIZE) / sz + 0.01),  texCoords[1]);
	sf::Vertex vert3(sf::Vector2f((x2 * TILE_SIZE) / sz2, (y2 * TILE_SIZE) / sz2 + 0.01), texCoords[2]);
	sf::Vertex vert4(sf::Vector2f((x1 * TILE_SIZE) / sz2, (y1 * TILE_SIZE) / sz2), texCoords[3]);
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
