#ifndef _DATA_RETURN
#define _DATA_RETURN 1
#include "dbtypes.h"
#ifndef _DB_FUNCTIONS
#include "dbfunctions.h"
#endif
#include <vector>
#include "item.h"
#include <unistd.h>

using namespace std;

class DataReturn{
	public:
		vector<Item> items;
		Table *table;
		string query;
		struct timeval start_execution_time;
		struct timeval end_execution_time;
		void *dbfunction;
};

#endif
