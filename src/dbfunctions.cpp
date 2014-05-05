#include "dbfunctions.h"
#include "config.h" 
#include <pthread.h> 
#include "constants.h"
#include "remote_execution.h"

using namespace std;


void DBFunctions::executeRemoteQuery(string query, bool print){

	using namespace boost;

	//this->executeQuery(query, print);

	Config *cfg = Config::getInstance();
	vector<string> db = cfg->getRemoteDatabases();
	int vsize = db.size();
	RemoteExecution *r = new RemoteExecution();
	r->query = query;
	r->db_functions = this;
	for(int i = 0; i < vsize; i++){
		r->conn = db[i];
		r->id = cfg->getId();
		pthread_t t;
		pthread_create(&t, NULL, &DBFunctions::executeRemote, r);
		pthread_join(t, NULL);
	}

}


void *DBFunctions::executeRemote(void *arg){


	//std::string *value = static_cast<std::string*>(arg);
	RemoteExecution *re = static_cast<RemoteExecution *>(arg);

	DBFunctions *db = new DBFunctions();

	db->connect(re->conn);

	cout << "ID: "<< endl;
	cout << re->id << endl;
	db->executeQuery(re->query, false);
	
	return NULL;

}


void DBFunctions::acceptRemote(){

}



int DBFunctions::executeQuery(string query, bool print){
	PGresult *res;
	res = PQexec(conn, query.c_str());
	ExecStatusType status = PQresultStatus(res);
	if (status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK){
		cout << "ERROR: ";
		cout << PQerrorMessage(conn) << endl;
		return PQresultStatus(res);
	}


	if (print == true){
		int i, j; 
		int nTuples = PQntuples(res);
		int nFields = PQnfields(res);
			
		for (i = 0; i < nTuples; i++){
			for(j = 0; j < nFields; j++)
				cout << PQgetvalue(res, i, j) << " | ";
			cout << endl;
		}
	}
	
	return 0;
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
