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
		int in_execution_queries;
		pthread_mutex_t count_mutex;
		pthread_mutex_t insert_mutex;
	public:
		DBFunctions();
		~DBFunctions();
		PGresult *executeQuery(string, bool);
		void executeRemoteQuery(string, DataReturn *, bool);
		static void *executeRemote(void *);
		void waitingQueryExecution();
		void acceptRemote();
		int connect(string);
		int connect();
		int getInExecutionQueries();
		int finishExecutionQuery();
		void loadTable(PGresult *, Table *);
};
