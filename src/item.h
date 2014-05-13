#ifndef _ITEM
#define _ITEM 1
#endif
#ifndef _DBTYPE
#include "dbtypes.h"
#endif
using namespace std;

class Item{
        public:
                long id;
                string conn_string;
                double execution_time;
                double local_process_time;
                int records_returned;
		Table *table;
};


