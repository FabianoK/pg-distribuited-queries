#ifndef _DB_FUNCTIONS
#define _DB_FUNCTIONS 1
#endif

#include <cstdlib>
#include <iostream>
#include <libpq-fe.h>

using namespace std;

class DBFunctions{

	private:
		PGconn *conn;
	public:
		int executeQuery(string, bool);
		void executeRemoteQuery(string, bool);
		static void *executeRemote(void *);
		void acceptRemote();
		int connect(string);
		int connect();

};
