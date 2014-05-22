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

#include "queries.h"

using namespace std;

class DBFunctions{

	private:
		Connection conn_old;
		int fieldIdx(string, vector<Item>);
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
		vector<Record> leftJoin(vector<Record>, vector<Record>, int, int);
		void sortByInt(vector<Record> *, int);
		Connection conn;
		Table *joinQuery(Query);
};
