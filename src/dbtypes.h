#ifndef _DBTYPES
#define _DBTYPES 1
#include <vector>
#include <map>
using namespace std;

class FieldDesc{
        public:
                string name;
                int type;
                int size;
                int index;
                vector<FieldDesc> header;
};

class Record{
        public:
                vector<string> fields;
};

class Table{
        public:
                vector<Record> records;
};

#endif
