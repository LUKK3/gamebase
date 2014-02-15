#include "Tunnel.h"

Tunnel::Tunnel(int length) {
    this->length = length;
}

void Tunnel::genTo(int z) {
	while((int)tunnel.size() < z) {
		tunnel.emplace_back();
		tunnel[tunnel.size() - 1] = new int[4];
		int* arr = tunnel[tunnel.size() - 1];
		for (int i = 0; i < 4; i++) {
			arr[i] = 0;
		}
	}
}
