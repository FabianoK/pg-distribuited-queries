#include <cstdlib>
#include <iostream>
#include "dbfunctions.h"
#include "utils.h"
#include "tests.h"
#include "queries.h"
#include "data_return.h"
#include "config.h"
#include <unistd.h>
#include <sys/time.h>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/map.hpp>

using namespace std;
void executeQuery(string, string);
void executeTest(string, DBFunctions *, vector<struct timeval> *, string);
void printValues(vector<Item>, vector<Record>);
void printResults(vector<Item>, vector<Record>);
int join(Query, DBFunctions *);

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

	//vector<string> queries = Utils::queriesToTest();

	//Config *cfg = Config::getInstance();


	DBFunctions *db = new DBFunctions();
	db->conn.loadAllConnection();

	struct timeval start, end;
	gettimeofday(&start, NULL);

	Query *q = new Query("test", "val_int > 0 and val_int < 10", "test_id");
	q->join("test_child_1", "test_id", "key_parent");

	cout<< q->join_table->table << endl;	

	join(*q, db);

	gettimeofday(&end, NULL);
	double total = Utils::timeDiff(start, end);


	cout<<total<<endl;
	
	//executeQuery("select * from test where val_int > 0 and val_int < 600", cfg->getDBConfig());

	/*
	int size = queries.size();
	for(int i = 0; i < size; i++){
		cout << "Executando querie "+queries[i] << endl;
		executeQuery(queries[i]);
	}

        
        string sql = "select * from (values "+in+" (0)) as t (key), test_child_1 t1 "+
                "where t1.key_parent = t.key order by key_parent";
        */

}

void printReader(Table *t){
	vector<FieldDesc> fd = t->header;

	cout << "H SIZE: "<<fd.size()<<endl;
	for(int i = 0; i < (int)fd.size(); i++)
		cout << fd[i].name << endl;

}


int fieldIdx(string field_name, vector<Item> values){
	vector<FieldDesc> fd  = values[0].table->header;	
	for(int i = 0; i < (int)fd.size(); i++){
		cout << fd[i].name << " " << field_name << endl;
		if(fd[i].name == field_name){
			cout << fd[i].index << endl;
			return fd[i].index;

		}
	}
	return -1;

}

int join(Query q, DBFunctions *db){

	if(q.join_table == NULL){
		cout << "ERROR JOIN TABLE NOT PRESENT";
		return -1;
	}

	string query = q.makeQuery(q);
	cout << query << endl;
	

	DataReturn *ret = new DataReturn();	
	Table *tm = new Table();
	ret->table = tm;
	db->executeRemoteQuery(query, ret);

	vector<Record> merge =  db->merge(ret);
	vector<Item> values = ret->items;


	//db->sort(&merge, 0, 0);

	printResults(values, merge);
	printValues(values, merge);

	
	string in;

	int idx = fieldIdx(q.key, values);

	for(int i = 0; i < (int)merge.size(); i++){
		in += "("+merge[i].fields[idx] + "),";
	}

	string sql = "select "+q.join_table->fields+" from (values "+in+" (0)) as t (key), "+
		q.join_table->table +" t1 "+
		"where t1."+q.join_table->foreign_key+" = t.key order by key_parent";


	
	ret = new DataReturn();	
	tm = new Table();
	ret->table = tm;

	db->executeRemoteQuery(sql, ret);

	vector<Record> join_merge =  db->merge(ret);
	vector<Item> join_values = ret->items;


	printValues(values, join_merge);

	int idx_f = fieldIdx(q.join_table->foreign_key, join_values);


	


	vector<Record> join_result = db->join(merge, join_merge, idx, idx_f);


	printResults(values, merge);
	printValues(values, join_result);

	return 0;

}

void printResults(vector<Item> values, vector<Record> merge){

	int vsize = (int)values.size();

	cout << "TOTAL SIZE: " << merge.size() << endl;

	cout << "HOST; EXECUTION TIME; LOCAL PROCESS TIME; RECORDS RETURN " << endl;

	for(int i=0; i < vsize; i++){
		Item it = values[i];
		string host = it.conn_string.substr(0, it.conn_string.find(" user="));
		cout << host << ";"<< it.execution_time << ";" << it.local_process_time << ";" << it.records_returned << endl;
	}
}

void printValues(vector<Item> values, vector<Record> merge){
	
	vector<FieldDesc> f = values[0].table->header;
	
	int hsize = (int)f.size();


	for(int i = 0; i < 100; i++)
		cout << "#";

	cout << endl;


	for(int i = 0;i < hsize; i++)
		cout << f[i].name << "|" << f[i].type << "|" << f[i].size << ";";

	cout << endl;

	
	for(int k = 0; k < (int)merge.size(); k++){
		for(int kk = 0; kk < (int)merge[k].fields.size(); kk++)
			cout << merge[k].fields[kk] << "|";

		cout << endl;
	}
}
