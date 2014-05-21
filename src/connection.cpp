#include "connection.h"
#include "config.h"

pthread_mutex_t conn_mutex;
int waiting_thread;

PGconn * Connection::getConnection(string conn_string){
	if(vpoll.find(conn_string) == vpoll.end()){
		cout << "LOADING CONNECTION" << endl;
		loadConnection(conn_string);
	}
	return vpoll[conn_string].back();
}

int Connection::getConnectionSize(){
	return poll.size();
}


int Connection::loadAllConnection(){
	int result = 0;
	Config *cfg = Config::getInstance();
	vector<string> db = cfg->getRemoteDatabases();

	int vsize = db.size();
	cout << "SIZE: "<< vsize << endl;
	for(int i = 0; i < vsize; i++){
		vpoll[db[i]].push_back(PQconnectdb(db[i].c_str()));
		result = PQstatus(vpoll[db[i]][0]);
		if(result != CONNECTION_OK){
			cout << "Connection not established: "<< PQerrorMessage(vpoll[db[i]][0]) << endl;
			
		}
	}

	return 0;
}

int Connection::loadConnection(string conn_string){

	int result = 0;
	pthread_mutex_lock(&conn_mutex);
	waiting_thread++;

	PGconn *th_conn = PQconnectdb(conn_string.c_str());
	poll[conn_string] = th_conn;
	pthread_mutex_unlock(&conn_mutex);
        result = PQstatus(th_conn);
        if(result != CONNECTION_OK){
        	cout << "Connection not established: " << conn_string << "\n" << PQerrorMessage(th_conn) << endl;
                        
        }

/*
*/
        return result;

}


