#include "Tunnel.h"
#include <cstdlib>

Tunnel::Tunnel(int length) {
    this->length = length;

    reset();
}

int Tunnel::getLength() {
	return length;
}

void Tunnel::reset() {
	tunnel.clear();
	tunnel.resize(20);
    for (int i = 0; i < 20; i++) {
		tunnel[i] = new int[5];
		for (int j = 0; j < 5; j++) {
			tunnel[i][j] = 0;
		}
    }
}

void Tunnel::genTo(int z) {
	while((int)tunnel.size() < z + 1) {
		if (rand() % 10 == 1) {
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
		} else {
			insertRow();
		}
		/*tunnel.emplace_back();
		tunnel.emplace_back();
		tunnel[tunnel.size() - 2] = new int[5];
		tunnel[tunnel.size() - 1] = new int[5];

		int* arr2 = tunnel[tunnel.size() - 2];
		for (int i = 0; i < 5; i++) {
			arr2[i] = 0;
			int r = rand() % 300;
			if (r == 1) arr2[i] = 1;
			else if (r > 270) arr2[i] = 2;
		}
		if (rand() % 10 == 1) {
			int x = (rand() % 2) * 2;
			arr[x] = 1;
			arr[x + 1] = 1;
			arr2[x] = 1;
			arr2[x + 1] = 1;
		}
		int r = rand() % 300;
		if (r == 1) {
			arr[4] = 1;
		} else if (r == 2) {
			arr[4] = 2;
		}*/
	}
}

int* Tunnel::insertRow() {
	tunnel.emplace_back();
	tunnel[tunnel.size() - 1] = new int[5];
	int* arr = tunnel[tunnel.size() - 1];
	for (int i = 0; i < 5; i++) {
		arr[i] = 0;
		if (i < 4) {
			int r = rand() % 100;
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
	return arr;
}

bool Tunnel::getWeb(int z, int x) {
	return get(z, 4) == x + 1;
}

int Tunnel::get(int z, int x) {
	if (x < 0) return 0;
	if (x > 4) return 0;
	if(z > length) return 0;
	genTo(z);
	return tunnel[z][x];
}

void Tunnel::set(int z, int x, int s) {
	tunnel[z][x] = s;
}
