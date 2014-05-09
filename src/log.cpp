#include "config.h"
#include "log.h"

using namespace std;


void Log::log(short log_info, string message){
	Config *c = Config::getInstance();
	int level = atoi(c->getConfig("log_level").c_str());
	cout << level << endl;
	cout << log_info << endl;
	if(level <= log_info){
		cout << message << endl;
	}
	// c->getConfig("log_file").c_str();
}

void Log::getConfig(){

}

void Log::log(){

}
