#include "dbfunctions.h"
#include "config.h" 
#include <pthread.h> 
#include "constants.h"
#include "remote_execution.h"
#include <sys/time.h>
#include <unistd.h>
#include "test_return_list.h"
#include "log.h"

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



void DBFunctions::executeRemoteQuery(string query, Result *process_class,  bool print){

	using namespace boost;

	Config *cfg = Config::getInstance();
	vector<string> db = cfg->getRemoteDatabases();
	int vsize = db.size();
	pthread_t t[100];
	for(int i = 0; i < vsize; i++){
		RemoteExecution *r = new RemoteExecution();
		r->query = query;
		r->db_functions = this;
		r->conn = db[i];
		r->id = cfg->getId();
		r->process_result = process_class;
		pthread_mutex_lock(&count_mutex);
		//cout << "Obtendo id " << r->id << endl;
		in_execution_queries++;
		pthread_mutex_unlock(&count_mutex);
		pthread_create(&t[i], NULL, &DBFunctions::executeRemote, r);
	}

}


void *DBFunctions::executeRemote(void *arg){

	struct timeval start, end;

	RemoteExecution *re = static_cast<RemoteExecution *>(arg);

	DBFunctions *db = new DBFunctions();

	db->connect(re->conn);


	gettimeofday(&start, NULL);

	PGresult *query = db->executeQuery(re->query, false);

	gettimeofday(&end, NULL);
	
	double total = ((( end.tv_sec - start.tv_sec ) *1000000L)
             + ((double)( end.tv_usec - start.tv_usec )))/1000000L;
	
	Result *res = static_cast<Result *>(re->process_result);
	
	if(res->getClassType() == 2){
		TestMap *tm = static_cast<TestMap *>(res->element_return);
		tm->item.execution_time = total;
		tm->item.id = re->id;
		tm->item.host = re->conn;
		tm->item.records_returned = PQntuples(query);
		gettimeofday(&start, NULL);
		//Config *c = Config::getInstance();
		
		//if((int)re->conn.find(c->getConfig("master_host")) < 0)
		cout << "Bf " << re->id << endl;;
		//tm->item.table = db->joinTable(query, tm, re->query); // tm->values[re->query].values);
		db->joinTable(query, tm, re->query); // tm->values[re->query].values);
		gettimeofday(&end, NULL);

		double total = ((( end.tv_sec - start.tv_sec ) *1000000L)
             	+ ((double)( end.tv_usec - start.tv_usec )))/1000000L;
		tm->item.local_process_time = total;
		res->element_return = tm;

	}
	//res->processReturn(re);
	re->db_functions->finishExecutionQuery();

	free(query);
	return NULL;

}

Table *DBFunctions::joinTable(PGresult *query, TestMap *tm, string query_str){

	int i, j, k=0;
        int nTuples = PQntuples(query);
        int nFields = PQnfields(query);
	char *tst;

	

	cout << query_str << endl;

	Table *t;
	TestList itens	= tm->values[query_str];
	if(itens.values.size() < 1){
		t = new Table();
		tm->item.table = t;
		itens.values.push_back(tm->item);
		cout << t << endl;
	}else{
		t = itens.values[0].table;
		cout << t << endl;

	}

	cout << "ITEM ID " << endl;;
	cout << tm->item.id << endl;

        FieldDesc *fd = new FieldDesc();
        for(j = 0; j < nFields; j++){
                fd->name = PQfname(query, j);
                fd->type = PQftype(query, j);
                fd->size = PQfsize(query, j);
                fd->index = PQfnumber(query, fd->name.c_str());
                fd->header.push_back(*fd);
        }

	free(fd);
	
        for (i = 0; i < nTuples; i++){
                Record *rr = new Record();
                for(j = 0; j < nFields; j++){
                        k++;
                        tst = PQgetvalue(query, i, j);
                        rr->fields.push_back(tst);
                }
                t->records.push_back(*rr);
        }
	//Log::log(DEBUG, t->records.size() + " | " + nTuples);
	return t;
	

}
