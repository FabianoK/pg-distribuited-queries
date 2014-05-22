#ifndef _DATA_RETURN
#define _DATA_RETURN 1
#include "dbtypes.h"
#ifndef _DB_FUNCTIONS
#include "dbfunctions.h"
#endif
#include <vector>
#ifndef _ITEM
#include "item.h"
#endif
#include <unistd.h>

using namespace std;

class DataReturn{
	public:
		vector<Item> items;
		string query;
		struct timeval start_execution_time;
		struct timeval end_execution_time;
		void *dbfunction;
};

#endif
