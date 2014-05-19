#include <map>
#include <iostream>
#include <libpq-fe.h>
#include <pthread.h>

using namespace std;

class Connection{
	private:
		map<string, PGconn *> pool;
	public:
		PGconn *getConnection(string);
		int loadConnection(string);
		int getConnectionSize();
};


