#include "config.h"
#include "tests.h"

using namespace std;

Tests::Tests(){

	db = new DBFunctions();
        confirmAll = false;
        //db->connect();
}

Tests::Tests(bool confirm){

	db = new DBFunctions();
        confirmAll = confirm;
}
bool Tests::confirmMessage(string message){
	string confirm;

	return Utils::confirmMessage("Confirm insert values in table "+ message, confirmAll);

}

void Tests::insertValues(){
	Config *c = Config::getInstance();

	vector<string> dbs = c->getRemoteDatabases();
	string master = c->getConfig("master_host");
	int size = dbs.size();


	for(int i = 0; i < size; i++){
		int adjust_size = 1;
		string host = dbs[i].substr(0, dbs[i].find(" user="));
		if(!confirmMessage("Insert data in connection "+host))
			continue;	

		if((int)host.find(master) >= 0){ 
			adjust_size = size -1;

			if(!confirmMessage("The host "+master+ " is master, continue?"))
				continue;	

		}
		this->db->connect(dbs[i]);
		int rec_size = atoi(c->getConfig("size_table_test").c_str());
		this->insertTest(rec_size * adjust_size);
		rec_size = atoi(c->getConfig("size_table_test_child_1").c_str());
		this->insertChild1 (rec_size * adjust_size);
		rec_size = atoi(c->getConfig("size_table_test_child_blob").c_str());
		this->insertBlob(rec_size * adjust_size);
		rec_size = atoi(c->getConfig("size_table_test_child_multi_col").c_str());
		this->insertMulti(rec_size * adjust_size);
	}
}

int Tests::removeValues(){
	
	Config *c = Config::getInstance();
	vector<string> dbs = c->getRemoteDatabases();
	int size = dbs.size();
	for(int i = 0; i < size; i++){
		this->db->connect(dbs[i]);
		string host = dbs[i].substr(0, dbs[i].find(" user="));
		if(Utils::confirmMessage("Remove data in all tables? To conn "+host, this->confirmAll))
			this->removeAll();
	}
	return 0;

}

int Tests::removeAll(){

	int ret = 0;

	string del = "delete from test";
	db->executeQuery(del.c_str(), false);
	cout << "Clead table test" << endl;

	del = "delete from test_child_1";
	db->executeQuery(del.c_str(), false);
	cout << "Clead table child 1" << endl;

	del = "delete from test_child_multi_col";
	db->executeQuery(del.c_str(), false);
	cout << "Clead table child multi" << endl;

	return ret;
}


void Tests::insertTest(int size){

	this->parentSize = size - 1;

	if(!confirmMessage("test"))
		return;

	string query;
	string tmp;
	string insert("insert into test (value_string,  val_int, val_float, val_timestamp)");	

	for(int i = 0; i < size; i++){
		char values[1000];
		sprintf(values, "('valor-%i', %i, %f, now())", i, i, ((float)i/size));
		string query(values);
		tmp = tmp + query;
		if((i % 500) == 0 || (i + 1) == size){
			query = insert + "values" +tmp;
			if(PQresultStatus(db->executeQuery(query.c_str(), false)) != PGRES_COMMAND_OK)
				return;
			tmp ="";
			cout << "\rInserting record: " << i;

		}else{
			tmp += ',';
		}
	}
	cout << endl;
}

void Tests::insertChild1(int size){

	if(!confirmMessage("test_child_1"))
		return;

	string query;
	string tmp;
	string insert("insert into test_child_1 (key_parent, val_str, val_str1)");	

	for(int i = 0; i < size; i++){
		char values[1000];
		sprintf(values, "(%i, 'the string with value %i', 'the string with other value %i')", 
				(int)rand() % parentSize + 1, i, i);
		string query(values);
		tmp = tmp + query;
		if((i % 500) == 0 || (i + 1) == size){
			query = insert + "values" +tmp;
			if(PQresultStatus(db->executeQuery(query.c_str(), false)) != PGRES_COMMAND_OK)
				return;
			tmp ="";
			cout << "\rInserting record: " << i;

		}else
			tmp += ',';
	}

	cout << endl;
}

void Tests::insertBlob(int size){

}

void Tests::insertMulti(int size){
	if(!confirmMessage("test_multi"))
		return;

	string query;
	string tmp;
	string insert("insert into test_child_multi_col (key_parent, val_str, val_str1, val_str2, val_str3, val_str4,\
		val_str5, val_str6, val_int, val_int1, val_int2, val_int3, val_int4, val_float, val_float1, val_float2) ");	



	for(int i = 0; i < size; i++){
		char values[1000];
		sprintf(values, "(%i, 'the string with value %i', 'the value %i',\
			'the value %i', 'the string3  with other value %i', \
			'the value %i', 'the value %i', \
			'the string6 with other value %i', %i, %i, %i, %i, %i, %f, %f, %f)", 
			(int)rand() % parentSize + 1, i, i, i, i, i, i, i, i, i ,i, i, i, (float)i, (float)i, (float)i);
		string query(values);
		tmp = tmp + query;
		if((i % 500) == 0 || (i + 1) == size){
			query = insert + "values" +tmp;
			if(PQresultStatus(db->executeQuery(query.c_str(), false)) != PGRES_COMMAND_OK)
				return;
			tmp ="";
			cout << "\rInserting record: " << i;

		}else
			tmp += ',';
	}

	cout << endl;


}
