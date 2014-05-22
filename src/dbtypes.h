#ifndef _DBTYPES
#define _DBTYPES 1
#include <vector>
#include <map>
#include <iostream>

using namespace std;

class ExecutionData{
	public:
		long id;
                string conn_string;
                double execution_time;
                double local_process_time;
                int records_returned;
};

class FieldDesc{
        public:
                string name;
                int type;
                int size;
                int index;
};

class Record{
        public:
                vector<string> fields;
};

class Table{
        public:
                vector<FieldDesc> header;
                vector<Record> records;
		vector<ExecutionData> statistics;
};

#endif
