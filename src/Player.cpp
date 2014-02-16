#include "Player.h"

Player::Player() {
	reset();
}

void Player::reset() {
	x = 0;
	y = 0;
	z = 0;
	xVel = 0;
	yVel = 0;
	zVel = 5;
	fallen = false;
	hasFeather = false;
	score = 0;
	feather = 0;
	slowTime = 0;
}
