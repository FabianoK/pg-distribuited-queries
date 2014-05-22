#include <cstdlib>
#include <iostream>
#include "dbfunctions.h"
#include "utils.h"
#include "tests.h"
#ifndef _QUERIES
#include "queries.h"
#endif
#include "data_return.h"
#include "config.h"
#include <unistd.h>
#include <sys/time.h>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/map.hpp>

using namespace std;

int organizeArgs(int argc, char **argv){
	char *tmp;
	int in_pos = -1;
	int ret = 0;
	for(int i = 1; i < argc; i++){
		std::string arg = argv[i];
		if(arg == "-h" || arg == "--help")
			return 2;
		if(arg == "-i" || arg == "--ignoreconfirms")
			ret = 1;
		if((arg == "-d" || arg == "--delete") && in_pos >= 0){
			tmp = argv[in_pos];
			argv[in_pos] = argv[i];
			argv[i] = tmp;
		}else if(arg == "-c" || arg == "--create"){
			in_pos = i;
		}
	}
	return ret;
}

int main(int argc, char **argv){
	Tests *t;
	
	int organized_args = organizeArgs(argc, argv);
	if(organized_args == 1)
		t = new Tests(true);
	else if(organized_args == 2){
		cout << "-c, --create        \tInsert data in data tables" << endl;
		cout << "-i, --ignoreconfirms\tIngore confirms before operation" << endl;
		cout << "-d, --delete        \tRemove records in data tables" << endl;
		cout << "-h, --help          \tHelp" << endl;
		return 0;
	}else
		t = new Tests();

	for (int i = 1; i < argc; ++i){ 
		std::string arg = argv[i];
		if(arg == "-c" || arg == "--create"){
			t->insertValues(); 
		}else if(arg == "-d" || arg == "--delete"){
			t->removeValues(); 
		}

		if(!Utils::confirmMessage("Continue process")){
			return 0;
		}
	}

	//The program start here

	//Load connections 
	DBFunctions *db = new DBFunctions();
	db->conn.loadAllConnection();

	struct timeval start, end;
	gettimeofday(&start, NULL);

	//Create query	

	Query *q = new Query("test", "val_int > 0 and val_int < 10", "test_id");
	q->join("test_child_1", "test_id", "key_parent");

	//End execution
	gettimeofday(&end, NULL);
	double total = Utils::timeDiff(start, end);


	Table *t_end = db->joinQuery(*q);
	Utils::printResults(t_end->statistics, t_end->records);
	

	cout<<total<<endl;
	
}

/**
*
* Print table fields name 
* @parame Table
*
**/

void printHeader(Table *t){
	vector<FieldDesc> fd = t->header;

	for(int i = 0; i < (int)fd.size(); i++)
		cout << fd[i].name;

	cout << endl;

}

