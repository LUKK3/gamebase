#include "Tunnel.h"
#include <cstdlib>

Tunnel::Tunnel(int length) {
    this->length = length;

    tunnel.resize(4);
    for (int i = 0; i < 4; i++) {
		tunnel[i] = new int[4];
		for (int j = 0; j < 4; j++) {
			tunnel[i][j] = 0;
		}
    }
}

void Tunnel::genTo(int z) {
	while((int)tunnel.size() < z + 1) {
		tunnel.emplace_back();
		tunnel.emplace_back();
		tunnel[tunnel.size() - 2] = new int[4];
		tunnel[tunnel.size() - 1] = new int[4];
		int* arr = tunnel[tunnel.size() - 1];
		for (int i = 0; i < 4; i++) {
			arr[i] = 0;
			if (rand() % 20 == 1) arr[i] = 1;
		}
		int* arr2 = tunnel[tunnel.size() - 2];
		for (int i = 0; i < 4; i++) {
			arr[i] = 0;
			if (rand() % 20 == 1) arr[i] = 1;
			//if (rand() % 20 == 2) arr[i] = 2;
		}
		if (rand() % 10 == 1) {
			int x = (rand() % 2) * 2;
			arr[x] = 1;
			arr[x + 1] = 1;
			arr2[x] = 1;
			arr2[x + 1] = 1;
		}
	}
}

int Tunnel::get(int z, int x) {
	genTo(z);
	return tunnel[z][x];
}
