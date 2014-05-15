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
void executeQueries(string);
void printValues(vector<Item>, vector<Record>);

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

	vector<string> queries = Utils::queriesToTest();
	int size = queries.size();

	for(int i = 0; i < size; i++){
		cout << "Executando querie "+queries[i] << endl;
		executeQueries(queries[i]);
	}



}

void executeQueries(string query){

	DBFunctions *db = new DBFunctions();



	DataReturn *ret = new DataReturn();	
	Table *tm = new Table();
	ret->table = tm;

	DataReturn *ret1 = new DataReturn();	
	Table *tm1 = new Table();
	ret1->table = tm1;
		

		
	db->executeRemoteQuery(query, ret, false);

	vector<Record> merge =  db->merge(ret);

	vector<Item> values = ret->items;

	printValues(values, merge);


	string in;

	for(int i = 0; i < (int)merge.size(); i++){
		in += merge[i].fields[0] + ",";
	}

	string sql = "select * from test_child_1 where key_parent in ("+in+"0)";

	cout << sql << endl;

	db->executeRemoteQuery(sql, ret1, false);

	vector<Record> merge1 =  db->merge(ret1);

	printValues(values, merge1);

	merge = db->join(merge, merge1, 0, 1);


	values = ret1->items;

	printValues(values, merge);

	free(tm);
	free(ret);
}

void printValues(vector<Item> values, vector<Record> merge){
	
	int vsize = (int)values.size();
	vector<FieldDesc> f = values[0].table->header;	
	int hsize = (int)f.size();


	for(int i = 0; i < 100; i++)
		cout << "#";

	cout << endl;


	for(int i = 0;i < hsize; i++)
		cout << f[i].name << "|" << f[i].type << "|" << f[i].size << ";";

	cout << endl;



	//vector<Record> merge =  db->merge(ret);
	//cout << "START ORDER" << endl;
	//db->sort(&merge);
	//cout << "END ORDER" << endl;
	
	for(int k = 0; k < (int)merge.size(); k++){
		for(int kk = 0; kk < (int)merge[k].fields.size(); kk++)
			cout << merge[k].fields[kk] << "|";

		cout << endl;
	}
		
	
	

	cout << "TOTAL SIZE: " << merge.size() << endl;

	for(int i=0; i < vsize; i++){
		Item it = values[i];
		string host = it.conn_string.substr(0, it.conn_string.find(" user="));
		cout << host << ";"<< it.execution_time << ";" << it.local_process_time << ";" << it.records_returned << endl;
	}

}
