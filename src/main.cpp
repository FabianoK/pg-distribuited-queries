#include <cstdlib>
#include <iostream>
#include "dbfunctions.h"
#include "utils.h"
#include "tests.h"
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

	Config *cfg = Config::getInstance();

/*
        cout << cfg->getDBConfig();

	DBFunctions *db = new DBFunctions();
	db->connect(cfg->getDBConfig().c_str());

	vector<struct timeval> start;
	struct timeval time1;

	gettimeofday(&time1, NULL);
	start.push_back(time1);

	executeTest("select * from test_child_1 where key_parent in (1,2,3,4,5)", db, &start);
	//executeTest("select test_id from test where val_int > 0 and val_int < 600", db, &start);

	gettimeofday(&time1, NULL);
	start.push_back(time1);

	//executeTest("select * from test ", db, &start);

	gettimeofday(&time1, NULL);
	start.push_back(time1);

	cout << endl;

	double total = Utils::timeDiff(start[0], start[1]);
	cout << "TEMPO TOTAL EXECUCAO: "<< total << endl;
	total = Utils::timeDiff(start[1], start[2]);
	cout << "TEMPO TOTAL 2 EXECUCAO: "<< total << endl;

	total = Utils::timeDiff(start[1], start[2]);
	cout << "TOTAL 1: "<< total << endl;

	total = Utils::timeDiff(start[2], start[3]);
	cout << "TOTAL 2: "<< total << endl;

	total = Utils::timeDiff(start[3], start[4]);
	cout << "TOTAL 2: "<< total << endl;
*/

	executeQuery("select * from test where val_int > 0 and val_int < 600", cfg->getDBConfig());

	/*
	int size = queries.size();
	for(int i = 0; i < size; i++){
		cout << "Executando querie "+queries[i] << endl;
		executeQuery(queries[i]);
	}*/



}

void executeTest(string query, DBFunctions *db, vector<struct timeval> *v_start, string conn_string){


	struct timeval start;
	gettimeofday(&start, NULL);
	//v_start->push_back(start);
	vector<Record> vec;

	
	PGresult *res;

	res = db->executeQuery(query, conn_string);

	int nTuples = PQntuples(res);
        int nFields = PQnfields(res);
	int i, j;

        char *tst;

	gettimeofday(&start, NULL);
	//v_start->push_back(start);

 
        for (i = 0; i < nTuples; i++){
                Record *rr = new Record();
                for(j = 0; j < nFields; j++){
                        tst = PQgetvalue(res, i, j);
                        rr->fields.push_back(tst);
                }
		vec.push_back(*rr);
        }


	gettimeofday(&start, NULL);
	//v_start->push_back(start);


}

void executeQuery(string query, string conn_string){

	struct timeval start, end;

	gettimeofday(&start, NULL);
	DBFunctions *db = new DBFunctions();

	DataReturn *ret = new DataReturn();	
	Table *tm = new Table();
	ret->table = tm;

	DataReturn *ret1 = new DataReturn();	
	Table *tm1 = new Table();
	ret1->table = tm1;
		

	//"select * from test_child_1 t1, test t where t.test_id = t1.key_parent and t.val_int > 0 and t.val_int < 500;"

	gettimeofday(&start, NULL);
		
	db->executeRemoteQuery(query, ret);

	gettimeofday(&end, NULL);
	double total = Utils::timeDiff(start, end);
	cout << total << endl;
	vector<Record> merge =  db->merge(ret);

	vector<Item> values = ret->items;

	//printValues(values, merge);
	printResults(values, merge);


	string in;
	string sql;

	cout << "START MERGE " << endl;
	if((int)merge.size() < 5000){
		for(int i = 0; i < (int)merge.size(); i++){
			in += "("+merge[i].fields[0] + "),";
			//in += merge[i].fields[0] + ",";
		}

		//sql = "select * from test_child_1 where key_parent in ("+in+"0) order by key_parent";
		sql = "select * from (values "+in+" (0)) as t (key), test_child_1 t1 where t1.key_parent = t.key order by key_parent";

	}else
		sql = "select * from test_child_1 order by key_parent";

	cout << "END MERGE " << endl;

	cout << sql << endl;

	db->executeRemoteQuery(sql, ret1);

	gettimeofday(&end, NULL);
	total = Utils::timeDiff(start, end);
	cout << total << endl;

	vector<Record> merge1 =  db->merge(ret1);
	values = ret1->items;

	printValues(values, merge1);
	printResults(values, merge1);

	//merge = db->join(merge, merge1, 0, 1);


	//values = ret1->items;

	//printValues(values, merge);
	//printResults(values, merge);

	free(tm);
	free(ret);
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
