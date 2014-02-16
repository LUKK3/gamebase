#ifndef TUNNEL_H
#define TUNNEL_H

#include <vector>

class Tunnel {
	public:
		Tunnel(int length = 300, int difficulty = 0);

		void genTo(int z);

		int get(int z, int x);
		void set(int z, int x, int s);

		bool getWeb(int z, int x);

		int getLength();

		bool hasBrickWall();
		void breakWall();

		void reset(int difficulty);

	private:
		int* insertRow();

		std::vector<int*> tunnel;
		int length;
		bool brickWall;
		int difficulty;
};

#endif // TUNNEL_H
