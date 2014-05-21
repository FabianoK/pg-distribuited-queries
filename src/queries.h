#ifndef _DBTYPES
#include "dbtypes.h"
#endif
#include <iostream>


using namespace std;

class Query{
	public:
		int type;
		string table;
		string fields;
		string where;
		string order;
		vector<Query> join;
		string key_parent;

		string makeQuery(Query);
		string makeJoinQuery(Query, string);
		Query();
};

