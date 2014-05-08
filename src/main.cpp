#include <cstdlib>
#include <iostream>

#ifndef _DB_FUNCTIONS
#include "dbfunctions.h"
#endif

#include "log.h"
#include "tests.h"
#ifndef _UTILS
#include "utils.h"
#endif
#include <unistd.h>
#ifndef _RESULT
#include "result.h"
#endif

#include "test_return_list.h"
#include <boost/foreach.hpp>
#include <boost/range/adaptor/map.hpp>

using namespace std;

int organizeArgs(int argc, char **argv){
	char *tmp;
	int in_pos = -1;
	ret = 0;
	for(int i = 1; i < argc; i++){
		std::string arg = argv[i];
		if(arg == "-i" || arg == "--ignoreconfirms")
			ret = 1;
		if((arg == "-d" || arg == "--delete") && in_pos >= 0){
			tmp = argv[in_pos];
			argv[in_pos] = argv[i];
			argv[i] = tmp;
			return;
		}else if(arg == "-c" || arg == "--create"){
			in_pos = i;
		}
	}
	ret;
}

int main(int argc, char **argv){
	Tests *t;
	if(organizeArgs(argc, argv) == 1)
		t = new Tests(true);
	else
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



	DBFunctions *db = new DBFunctions();

	vector<string> queries = Utils::queriesToTest();
	int size = queries.size();

	TestReturn *ret = new TestReturn();	
	TestMap *tm = new TestMap();
	ret->setElementReturn(tm);

	for(int i = 0; i < size; i++){
		cout << "Executando querie "+queries[i] << endl;
		db->executeRemoteQuery(queries[i], ret, false);
	}

	db->waitingQueryExecution();

	
	map<string, TestList> values = tm->values;


	BOOST_FOREACH(string s, values | boost::adaptors::map_keys) {
		cout <<  s << " Valor\n";
		TestList tl = values[s];
		vector<ItemList> il = tl.values;
		for(int i = 0; i < (int)il.size(); i++){
			string host = il[i].host.substr(0, il[i].host.find(" user="));
			cout << "TABLE SIZE: "<<  il[i].table->records.size() <<  " EX. TIME: "<< il[i].execution_time << " TIME LOCAL: " << il[i].local_process_time << " HOST: "<< host << " ID: "<< il[i].id << endl;
		}
	}

}
