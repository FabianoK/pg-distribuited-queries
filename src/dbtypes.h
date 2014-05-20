#ifndef _DBTYPES
#define _DBTYPES 1
#include <vector>
#include <map>
#include <iostream>

using namespace std;

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
};

#endif
