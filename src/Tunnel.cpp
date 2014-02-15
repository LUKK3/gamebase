#include "Tunnel.h"
#include <cstdlib>

Tunnel::Tunnel(int length) {
    this->length = length;
}

int Tunnel::getLength() {
	return length;
}

void Tunnel::genTo(int z) {
	while((int)tunnel.size() < z + 1) {
		tunnel.emplace_back();
		tunnel[tunnel.size() - 1] = new int[4];
		int* arr = tunnel[tunnel.size() - 1];
		for (int i = 0; i < 4; i++) {
			if (rand() % 10)
				arr[i] = 0;
			else arr[i] = 1;
		}
	}
}

int Tunnel::get(int z, int x) {
	if(z > length){
		return 0;
	}
	genTo(z);
	return tunnel[z][x];
}
