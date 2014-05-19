#ifndef _DB_FUNCTIONS
#define _DB_FUNCTIONS 1
#endif

#include <cstdlib>
#include <iostream>
#include <vector>
#include <libpq-fe.h>
#include "dbtypes.h"
#include "connection.h"
#include "data_return.h"


using namespace std;


class DBFunctions{

	private:
		Connection conn_old;
	public:
		DBFunctions();
		~DBFunctions();
		PGresult *executeQuery(string, string);
		void executeRemoteQuery(string, DataReturn *);
		static void *executeRemote(void *);
		void acceptRemote();
		int finishExecutionQuery();
		void loadTable(PGresult *, Table *);
		vector<Record> merge(DataReturn *ret);
		vector<Record> join(vector<Record>, vector<Record>, int, int);
		void sort(vector<Record> *);
		Connection conn;
};
