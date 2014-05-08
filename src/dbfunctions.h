#ifndef _DB_FUNCTIONS
#define _DB_FUNCTIONS 1
#endif

#ifndef _RESULT
#include "result.h"
#endif

#include <cstdlib>
#include <iostream>
#include <vector>
#include <libpq-fe.h>
#include "dbtypes.h"

using namespace std;

class DBFunctions{

	private:
		PGconn *conn;
		int in_execution_queries;
	public:
		DBFunctions();
		PGresult *executeQuery(string, bool);
		void executeRemoteQuery(string, Result *, bool);
		static void *executeRemote(void *);
		void waitingQueryExecution();
		void acceptRemote();
		int connect(string);
		int connect();
		int getInExecutionQueries();
		int finishExecutionQuery();
		Table *joinTable(PGresult *);
};
