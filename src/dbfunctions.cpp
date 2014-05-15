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
	bool first;
	
	cout << "INICIANDO - " << endl;
	for(int i = 0; i < left_size; i++){
		first = true;
		while(left[i].fields[idx_l] == right[j].fields[idx_r])
		{
			if(first){
				cout << 1<< " " << j << endl;
				left[i].fields.insert(left[i].fields.begin()+j, right[j].fields.begin(), right[j].fields.end());
				first = false;
				cout << 2<< endl;
			}else{
				cout << 3<< endl;
				Record *rec = new Record();
				rec->fields = left[i].fields;
				rec->fields.insert(rec->fields.end(), right[j].fields.begin(), right[j].fields.end());
				left.push_back(*rec);
				left.insert(left.begin() + i, *rec);
				i++;
				cout << 4<< endl;
				
			}
			j++;
			if(j == right_size)
				break;
		}
	}
	cout << " - END" << endl;
	return left;
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

PGresult *DBFunctions::executeQuery(string query, bool print){
	PGresult *res;
	res = PQexec(conn, query.c_str());
	ExecStatusType status = PQresultStatus(res);
	if (status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK){
		cout << "ERROR: ";
		cout << PQerrorMessage(conn) << endl;
	}

	return res;
}


int DBFunctions::connect(){
	Config *cfg = Config::getInstance();
	return connect(cfg->getDBConfig().c_str());
}

int DBFunctions::connect(string c){

	int result; 
	conn = PQconnectdb(c.c_str());
	result = PQstatus(conn);
	if(result != CONNECTION_OK) 
		cout << "Connection not established: "<< PQerrorMessage(conn) << endl;
	
	return result;

}



void DBFunctions::executeRemoteQuery(string query, DataReturn *data_return,  bool print){

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
		data_return->dbfunction = this;
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

	DBFunctions *db = new DBFunctions();

	db->connect(item.conn_string);

	gettimeofday(&start, NULL);

	PGresult *query = db->executeQuery(te->data_return->query, false);

	gettimeofday(&end, NULL);
	
	double total = Utils::timeDiff(start, end);

	
	item.records_returned = PQntuples(query);

	gettimeofday(&start, NULL);

	item.table = new Table();

	db->loadTable(query, item.table);

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
