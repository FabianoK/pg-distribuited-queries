#include "dbfunctions.h"
#include "config.h" 
#include <pthread.h> 
#include "constants.h"
#include <sys/time.h>
#include <unistd.h>
#include "thread_execution_item.h"

using namespace std;


DBFunctions::~DBFunctions(){
	free(conn);
}

DBFunctions::DBFunctions(){
	in_execution_queries = 0;;
}

int DBFunctions::getInExecutionQueries(){
	return in_execution_queries;
}

int DBFunctions::finishExecutionQuery(){
	int ret;
	pthread_mutex_lock(&count_mutex);
	ret = --in_execution_queries;
	pthread_mutex_unlock(&count_mutex);
	return ret;
}

void DBFunctions::waitingQueryExecution(){
	while(1){
		sleep(1);
		if(this->getInExecutionQueries() < 1)
			return;
	}

}

void DBFunctions::acceptRemote(){

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
	//PQfinish(conn);
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
		pthread_mutex_lock(&count_mutex);
		in_execution_queries++;
		pthread_mutex_unlock(&count_mutex);
		data_return->dbfunction = this; 
		pthread_create(&t[i], NULL, &DBFunctions::executeRemote, thread_item);
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
	
	double total = ((( end.tv_sec - start.tv_sec ) *1000000L)
             + ((double)( end.tv_usec - start.tv_usec )))/1000000L;


	
	item.records_returned = PQntuples(query);
	gettimeofday(&start, NULL);
	
	db->joinTable(query, data_return);
	gettimeofday(&end, NULL);

	total = ((( end.tv_sec - start.tv_sec ) *1000000L)
	+ ((double)( end.tv_usec - start.tv_usec )))/1000000L;
	item.local_process_time = total;



	(static_cast<DBFunctions *>(data_return->dbfunction))->finishExecutionQuery();

	data_return->items.push_back(item);

	free(query);
	return NULL;

}

void DBFunctions::joinTable(PGresult *query, DataReturn *data_return){

	int i, j, k=0;
        int nTuples = PQntuples(query);
        int nFields = PQnfields(query);
	char *tst;

	


	Table *t = data_return->table;


        FieldDesc *fd = new FieldDesc();
        for(j = 0; j < nFields; j++){
                fd->name = PQfname(query, j);
                fd->type = PQftype(query, j);
                fd->size = PQfsize(query, j);
                fd->index = PQfnumber(query, fd->name.c_str());
                fd->header.push_back(*fd);
        }

	free(fd);
	
	cout << t << endl;
        for (i = 0; i < nTuples; i++){
                Record *rr = new Record();
                for(j = 0; j < nFields; j++){
                        k++;
                        tst = PQgetvalue(query, i, j);
                        rr->fields.push_back(tst);
                }
		pthread_mutex_lock(&insert_mutex);
                t->records.push_back(*rr);
		pthread_mutex_unlock(&insert_mutex);
        }
}
