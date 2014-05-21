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
		string key;
		string foreign_key;
		Query *join_table;

		string makeQuery(Query);
		string makeJoinQuery(Query, string);
		Query();
		Query(string);
		Query(string, string);
		Query(string, string, string);
		Query *join(string, string, string);
};

