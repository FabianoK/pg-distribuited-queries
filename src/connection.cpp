#include "connection.h"
#include "config.h"

PGconn * Connection::getConnection(string conn_string){
	if(pool.size() < 1){
		loadConnections();
		cout << "STARTING CONNECTION" << endl;
	}
	cout << "RETURNING PULL: "<< pool.size() << endl;
	return pool[conn_string];
}

int Connection::loadConnections(){

	int result = 0;
	Config *cfg = Config::getInstance();
	vector<string> db = cfg->getRemoteDatabases();

	int vsize = db.size();

	for(int i = 0; i < vsize; i++){

		pool[db[i]] = PQconnectdb(db[i].c_str());
		result = PQstatus(pool[db[i]]);
		if(result != CONNECTION_OK){
			cout << "Connection not established: "<< PQerrorMessage(pool[db[i]]) << endl;
			
		}
	}

        return result;

}


