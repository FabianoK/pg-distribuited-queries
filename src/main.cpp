#include <cstdlib>
#include <iostream>

#ifndef _DB_FUNCTIONS
#include "dbfunctions.h"
#endif

#include "log.h"
#include "tests.h"
#ifndef _UTILS
#include "utils.h"
#endif

using namespace std;

void organizeArgs(int argc, char **argv){
	char *tmp;
	int in_pos = -1;
	for(int i = 1; i < argc; i++){
		std::string arg = argv[i];
		if((arg == "-d" || arg == "--delete") && in_pos >= 0){
			tmp = argv[in_pos];
			argv[in_pos] = argv[i];
			argv[i] = tmp;
			return;
		}else if(arg == "-c" || arg == "--create"){
			in_pos = i;
		}
	}

}

int main(int argc, char **argv){
	organizeArgs(argc, argv);
	for (int i = 1; i < argc; ++i){ 
		std::string arg = argv[i];
		if(arg == "-c" || arg == "--create"){
			Tests::insertValues(); 
		}else if(arg == "-d" || arg == "--delete"){
			Tests::removeValues(); 
		}

		if(!Utils::confirmMessage("Continue process")){
			return 0;
		}
	}



	DBFunctions *db = new DBFunctions();
	//db->connect();

	vector<string> queries = Utils::queriesToTest();
	int size = queries.size();

	

	for(int i = 0; i < size; i++){
		cout << "Executando querie "+queries[i] << endl;
		db->executeRemoteQuery(queries[i], false);
	}
	//db->executeQuery("select test_id from test", true);
	//Log::generateLog();

}
