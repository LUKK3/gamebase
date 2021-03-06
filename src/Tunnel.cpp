#include "Tunnel.h"
#include <cstdlib>

#define WID 9

Tunnel::Tunnel(int length, int diff) {
	brickWall = true;
    reset(length, diff);
}

int Tunnel::getLength() {
	return length;
}

bool Tunnel::hasBrickWall() {
	return brickWall;
}

void Tunnel::reset(int len, int diff) {
	difficulty = diff;
	length = len;
	tunnel.clear();
	tunnel.resize(20);
    for (int i = 0; i < 20; i++) {
		tunnel[i] = new int[WID];
		for (int j = 0; j < WID; j++) {
			tunnel[i][j] = 0;
		}
    }
    brickWall = true;
}

void Tunnel::genTo(int z) {
	while((int)tunnel.size() < z + 1) {
		if (rand() % (10 - difficulty) == 1) {
			if (rand() % 10 == 1) {
				// bridge pool
				int x = rand() % 2 + 1;
				int len = rand() % 4 + 4;
				int* arrg = insertRow();
				arrg[x] = 0;
				for (int i = 0; i < len; i++) {
					int* arr = insertRow();
					for (int j = 0; j < 4; j++) {
						if (j == x) {
							arr[j] = 5;
						} else {
							if (rand() % 10 == 1) {
								arr[j] = 0;
							} else {
								arr[j] = 1;
							}
						}
					}
				}
				arrg = insertRow();
				arrg[x] = 0;
			} else {
				// pool
				int x = rand() % 4;
				int len = rand() % 4 + 4;
				for (int i = 0; i < len; i++) {
					int* arr = insertRow();
					arr[x] = 1;
					if (i != 0 && i != len - 1) {
						int x0 = x - 1;
						if (x0 >= 0) {
							arr[x0] = 1;
						}
						int x1 = x + 1;
						if (x1 <= 3) {
							arr[x1] = 1;
						}
					}
				}
			}
		} else {
			insertRow();
		}
	}
}

void Tunnel::breakWall() {
	brickWall = false;
}

int* Tunnel::insertRow() {
	tunnel.emplace_back();
	tunnel[tunnel.size() - 1] = new int[WID];
	int* arr = tunnel[tunnel.size() - 1];
	for (int i = 0; i < WID; i++) {
		arr[i] = 0;
		if (i < 4) {
			int r = rand() % (100 - difficulty * 10);
			if (r == 0) arr[i] = 2;
			else if (r == 1) arr[i] = 3;
			else if (r == 2) arr[i] = 4;
			else if (r == 6) arr[i] = 1;
		} else {
			int r = rand() % 300;
			if (r == 1) {
				arr[4] = 1;
			} else if (r == 2) {
				arr[4] = 2;
			}
		}
	}
	if (rand() % (30 + difficulty * 10) == 0) {
		arr[rand() % 4 + 5] = rand() % 3 + 1;
	} else if (rand() % 100 == 0) {
		arr[rand() % 4 + 5] = rand() % 3 + 4;
	}
	return arr;
}

int Tunnel::getDifficulty() {
	return difficulty;
}

bool Tunnel::getWeb(int z, int x) {
	return get(z, 4) == x + 1;
}

int Tunnel::get(int z, int x) {
	if (x < 0) return 0;
	if (x > WID) return 0;
	if(z > length - 2) return 0;
	genTo(z);
	return tunnel[z][x];
}

void Tunnel::set(int z, int x, int s) {
	tunnel[z][x] = s;
}
