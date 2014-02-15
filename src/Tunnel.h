#ifndef TUNNEL_H
#define TUNNEL_H

#include <vector>

class Tunnel {
	public:
		Tunnel(int length = 100);

		void genTo(int z);

		int get(int z, int x);

		int getLength();

	private:
		std::vector<int*> tunnel;
		int length;
};

#endif // TUNNEL_H
