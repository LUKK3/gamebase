#ifndef PLAYER_H
#define PLAYER_H


class Player {
	public:
		float x, y, z;
		float xVel, yVel, zVel;
		bool fallen;
		int score;

		Player();

		void reset();

	private:
};

#endif // PLAYER_H
