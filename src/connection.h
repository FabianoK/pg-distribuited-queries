#include <map>
#include <iostream>
#include <libpq-fe.h>

using namespace std;

class Connection{
	private:
		map<string, PGconn *> pool;
		int loadConnections();
	public:
		PGconn *getConnection(string);
};


