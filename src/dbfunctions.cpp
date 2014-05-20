#include "dbfunctions.h"
#include "config.h" 
#include <pthread.h> 
#include "constants.h"
#include <sys/time.h>
#include <unistd.h>
#include "thread_execution_item.h"

using namespace std;

pthread_mutex_t item_mutex;
int values;

DBFunctions::~DBFunctions(){
	values = 0;
}

DBFunctions::DBFunctions(){
}

void DBFunctions::acceptRemote(){

}

struct orderInt{
	int idx;
  	bool operator() (Record r,  Record rr) { 
		char *i, *j;
		i = (char *)r.fields[1].c_str();
		j = (char *)rr.fields[1].c_str();
		return (i < j);
	}

} orderInt;


void DBFunctions::sort(vector<Record> *table){
	std::sort(table->begin(), table->end(), orderInt);
}

vector<Record> DBFunctions::join(vector<Record> left, vector<Record> right, int idx_l, int idx_r){
	int left_size = (int)left.size();
	int right_size = (int)right.size();
	int j = 0;
	vector<Record> ret;

	
	for(int i = 0; i < left_size; i++){

		Record *rec = new Record();

		*rec = left[i];

		if(j < right_size){
			if(left[i].fields[idx_l] == right[j].fields[idx_r]){
				rec->fields.insert(rec->fields.end(), right[j].fields.begin(), right[j].fields.end());
				j++;
			}
			ret.push_back(*rec);
		}else{
			ret.push_back(*rec);
			continue;
		}

		if(j >= right_size)
			continue;

		while(left[i].fields[idx_l] == right[j].fields[idx_r]){
			rec = new Record();
			*rec = left[i];
			rec->fields.insert(rec->fields.end(), right[j].fields.begin(), right[j].fields.end());
			ret.push_back(*rec);
			j++;
			if(j >= right_size)
				break;
		}
	}
	return ret;
}

vector<Record> DBFunctions::merge(DataReturn *ret){

	int vsize = (int)ret->items.size();
	vector<Record> join =  ret->items[0].table->records;

	for(int i=1; i < vsize; i++){
                vector<Record> tmp = ret->items[i].table->records;
                int tmp_size = tmp.size();

                for(int j = 0; j < tmp_size; j++)
                        join.push_back(tmp[j]);
        }

	return join;
}

PGresult *DBFunctions::executeQuery(string query, string conn_string){
	PGresult *res;
	res = PQexec(conn.getConnection(conn_string), query.c_str());
	ExecStatusType status = PQresultStatus(res);
	if (status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK){
		cout << "ERROR: ";
		cout << PQerrorMessage(conn.getConnection(conn_string)) << endl;
	}

	return res;
}

void DBFunctions::executeRemoteQuery(string query, DataReturn *data_return){

	gettimeofday(&data_return->start_execution_time, NULL);

	using namespace boost;

	Config *cfg = Config::getInstance();
	vector<string> db = cfg->getRemoteDatabases();
	int vsize = db.size();
	pthread_t t[100];
	data_return->dbfunction = this;
	data_return->query = query;
	for(int i = 0; i < vsize; i++){
		ThreadExecutionItem *thread_item = new ThreadExecutionItem();
		Item *item = new Item();
		item->conn_string = db[i];
		item->id = cfg->getId();
		thread_item->item = *item;
		thread_item->data_return = data_return;
		pthread_create(&t[i], NULL, &DBFunctions::executeRemote, thread_item);
	}
	for(int i = 0; i < vsize; i++){
		pthread_join(t[i], NULL);
	}
	
}


void *DBFunctions::executeRemote(void *arg){

	struct timeval start, end;

	ThreadExecutionItem *te = static_cast<ThreadExecutionItem *>(arg);
	Item item = te->item;
	DataReturn *data_return = te->data_return;
	DBFunctions *db = static_cast<DBFunctions *>(te->data_return->dbfunction);

	db->conn.getConnection(item.conn_string);
	//db->connect(item.conn_string);

	gettimeofday(&start, NULL);

	PGresult *query = db->executeQuery(te->data_return->query, item.conn_string);

	gettimeofday(&end, NULL);
	
	double total = Utils::timeDiff(start, end);

	item.execution_time = total;
	item.records_returned = PQntuples(query);

	gettimeofday(&start, NULL);

	item.table = new Table();
	//cout << "START LOAD" << endl;
	db->loadTable(query, item.table);
	//cout << "END LOAD" << endl;

	gettimeofday(&end, NULL);

	total = Utils::timeDiff(start, end);

	item.local_process_time = total;

	//Time to 
	pthread_mutex_lock(&item_mutex);
	values++;
	data_return->items.push_back(item);
	pthread_mutex_unlock(&item_mutex);

	return NULL;

}

void DBFunctions::loadTable(PGresult *query, Table *t){

	int i, j;
        int nTuples = PQntuples(query);
        int nFields = PQnfields(query);
	char *tst;


        FieldDesc *fd = new FieldDesc();
        for(j = 0; j < nFields; j++){
                fd->name = PQfname(query, j);
                fd->type = PQftype(query, j);
                fd->size = PQfsize(query, j);
                fd->index = PQfnumber(query, fd->name.c_str());
                t->header.push_back(*fd);
        }

	
        for (i = 0; i < nTuples; i++){
                Record *rr = new Record();
                for(j = 0; j < nFields; j++){
                        tst = PQgetvalue(query, i, j);
                        rr->fields.push_back(tst);
                }
                t->records.push_back(*rr);
        }
}
