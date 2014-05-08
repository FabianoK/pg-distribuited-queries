#include "dbfunctions.h"
#include "config.h" 
#include <pthread.h> 
#include "constants.h"
#include "remote_execution.h"
#include <sys/time.h>
#include <unistd.h>
#include "test_return_list.h"

using namespace std;

DBFunctions::DBFunctions(){
	in_execution_queries = 0;;
}

int DBFunctions::getInExecutionQueries(){
	return in_execution_queries;
}

int DBFunctions::finishExecutionQuery(){
	return --in_execution_queries;
}

void DBFunctions::waitingQueryExecution(){
	while(1){
		sleep(1);
		//cout << this->getInExecutionQueries() << endl;
		if(this->getInExecutionQueries() < 1)
			return;
	}

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
		//cout << "Obtendo id " << r->id << endl;
		in_execution_queries++;	
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
	
	//cout << "ID " << re->id << endl;
	double total = ((( end.tv_sec - start.tv_sec ) *1000000L)
             + ((double)( end.tv_usec - start.tv_usec )))/1000000L;
	//cout << total  << endl;
	Result *res = static_cast<Result *>(re->process_result);
	if(res->getClassType() == 2){
		TestMap *tm = static_cast<TestMap *>(res->element_return);
		tm->item.execution_time = total;
		tm->item.id = re->id;
		tm->item.host = re->conn;
		gettimeofday(&start, NULL);
		tm->item.table = db->joinTable(query);
		gettimeofday(&end, NULL);
		double total = ((( end.tv_sec - start.tv_sec ) *1000000L)
             	+ ((double)( end.tv_usec - start.tv_usec )))/1000000L;
		tm->item.local_process_time = total;
		res->element_return = tm;

	}
	res->processReturn(re);
	re->db_functions->finishExecutionQuery();
	return NULL;

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

Table *DBFunctions::joinTable(PGresult *query){

	int i, j, k=0;
        int nTuples = PQntuples(query);
        int nFields = PQnfields(query);
	char *tst;

	Table *t = new Table();

        FieldDesc *fd = new FieldDesc();
        for(j = 0; j < nFields; j++){
                fd->name = PQfname(query, j);
                fd->type = PQftype(query, j);
                fd->size = PQfsize(query, j);
                fd->index = PQfnumber(query, fd->name.c_str());
                fd->header.push_back(*fd);
        }

        for (i = 0; i < nTuples; i++){
                Record *rr = new Record();
                for(j = 0; j < nFields; j++){
                        k++;
                        tst = PQgetvalue(query, i, j);
                        rr->fields.push_back(tst);
                }
                t->records.push_back(*rr);
        }

	cout << t->records.size() << " | " << nTuples << endl;
	return t;
	

}
