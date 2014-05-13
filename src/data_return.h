#ifndef _DATA_RETURN
#define _DATA_RETURN 1
#include "dbtypes.h"
#ifndef _DB_FUNCTIONS
#include "dbfunctions.h"
#endif
#include <vector>
#include "item.h"

using namespace std;

class DataReturn{
	public:
		vector<Item> items;
		Table *table;
		string query;
		void *dbfunction;
};

#endif
