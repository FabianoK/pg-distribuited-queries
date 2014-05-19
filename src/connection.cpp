#include "connection.h"
#include "config.h"

pthread_mutex_t conn_mutex;
int waiting_thread;

PGconn * Connection::getConnection(string conn_string){
	if(pool.find(conn_string) == pool.end()){
		cout << "LOADING CONNECTION" << endl;
		loadConnection(conn_string);
	}
	return pool[conn_string];
}

int Connection::getConnectionSize(){
	return pool.size();
}



int Connection::loadConnection(string conn_string){

	int result = 0;
	pthread_mutex_lock(&conn_mutex);
	waiting_thread++;

	PGconn *th_conn = PQconnectdb(conn_string.c_str());
	pool[conn_string] = th_conn;
	pthread_mutex_unlock(&conn_mutex);
        result = PQstatus(th_conn);
        if(result != CONNECTION_OK){
        	cout << "Connection not established: " << conn_string << "\n" << PQerrorMessage(th_conn) << endl;
                        
        }

/*
	int result = 0;
	Config *cfg = Config::getInstance();
	vector<string> db = cfg->getRemoteDatabases();

	int vsize = db.size();
	cout << "SIZE: "<< vsize << endl;
	for(int i = 0; i < vsize; i++){

		pool[db[i]] = PQconnectdb(db[i].c_str());
		result = PQstatus(pool[db[i]]);
		if(result != CONNECTION_OK){
			cout << "Connection not established: "<< PQerrorMessage(pool[db[i]]) << endl;
			
		}
	}
*/
        return result;

}


