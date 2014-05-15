#ifndef _DB_FUNCTIONS
#define _DB_FUNCTIONS 1
#endif

#include <cstdlib>
#include <iostream>
#include <vector>
#include <libpq-fe.h>
#include "dbtypes.h"
#include "data_return.h"


using namespace std;


class DBFunctions{

	private:
		PGconn *conn;
	public:
		DBFunctions();
		~DBFunctions();
		PGresult *executeQuery(string, bool);
		void executeRemoteQuery(string, DataReturn *, bool);
		static void *executeRemote(void *);
		void acceptRemote();
		int connect(string);
		int connect();
		int finishExecutionQuery();
		void loadTable(PGresult *, Table *);
		vector<Record> merge(DataReturn *ret);
		void sort(vector<Record> *);
};
