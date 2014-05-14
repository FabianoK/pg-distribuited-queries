#include <cstdlib>
#include <iostream>
#include "dbfunctions.h"
#include "utils.h"
#include "tests.h"
#include "data_return.h"
#include <unistd.h>
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



	DBFunctions *db = new DBFunctions();

	vector<string> queries = Utils::queriesToTest();
	int size = queries.size();


	DataReturn *ret = new DataReturn();	
	Table *tm = new Table();

	ret->table = tm;
		
	for(int i = 0; i < size; i++){
		cout << "Executando querie "+queries[i] << endl;
		db->executeRemoteQuery(queries[i], ret, false);
	}



	db->waitingQueryExecution();

	vector<Item> values = ret->items;
	
	int vsize = (int)values.size();
	vector<FieldDesc> f = values[0].table->header;	
	int hsize = (int)f.size();

	for(int i = 0;i < hsize; i++)
		cout << f[i].name << "|" << f[i].type << "|" << f[i].size << ";";

	cout << endl;


	for(int i=0; i < vsize; i++){
		Item it = values[i];
		string host = it.conn_string.substr(0, it.conn_string.find(" user="));
		cout << host << ";"<< it.execution_time << ";" << it.local_process_time << ";" << it.records_returned << endl;
	}
	/*
	BOOST_FOREACH(string s, values | boost::adaptors::map_keys) {
		cout <<  s << " Valor\n";
		TestList tl = values[s];
		vector<ItemList> il = tl.values;
		for(int i = 0; i < (int)il.size(); i++){
			string host = il[i].host.substr(0, il[i].host.find(" user="));
			cout << il[i].table->records.size() << ";"<< il[i].records_returned <<  ";"<< il[i].execution_time << ";" << il[i].local_process_time << ";"<< host << ";"<< il[i].id << endl;
			//cout << "TABLE TOTAL SIZE: "<<  il[i].table->records.size() << " TABLE SIZE: "<< il[i].records_returned <<  " EX. TIME: "<< il[i].execution_time << " TIME LOCAL: " << il[i].local_process_time << " HOST: "<< host << " ID: "<< il[i].id << endl;
		}
	}
	*/
	free(tm);
	free(ret);
}
