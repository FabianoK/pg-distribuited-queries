#include <map>
#include <iostream>
#include <libpq-fe.h>
#include <pthread.h>
#include <vector>

using namespace std;

class Connection{
	typedef vector<PGconn *> pgpoll; 
	private:
		map<string, pgpoll> vpoll;
		map<string, PGconn *> poll;
	public:
		PGconn *getConnection(string);
		int loadConnection(string);
		int getConnectionSize();
		int loadAllConnection();
};


