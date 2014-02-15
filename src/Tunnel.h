#ifndef TUNNEL_H
#define TUNNEL_H

#include <vector>

class Tunnel {
	public:
		Tunnel();

		void genTo(int z);

		int get(int z, int x);

	private:
		std::vector<int*> tunnel;
};

#endif // TUNNEL_H
