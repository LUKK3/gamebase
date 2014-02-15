#ifndef TUNNEL_H
#define TUNNEL_H

#include <vector>

class Tunnel {
	public:
		Tunnel();

		void genTo(int z);

	private:
		std::vector<int*> tunnel;
};

#endif // TUNNEL_H
