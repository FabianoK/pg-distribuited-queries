#include <cstdlib>
#include <iostream>
#include "config.h"
#include <fstream>
#include <boost/property_tree/ini_parser.hpp>
#include "constants.h"

using namespace std;

Config* Config::conf = NULL;

Config::Config(){
	id=0;
	setConfig();
}

Config *Config::getInstance(){
	if(conf == NULL){
		conf = new Config();
	}

	return conf;
		
}

long Config::getId(){
	return id++;
}

string Config::getConfig(string key){
	return pt.get<string>(key);
}

string Config::getDBConfig(){
	return dataConfig;
}

void Config::setConfig(){

	boost::property_tree::ini_parser::read_ini(CONFIG_FILE, pt);
	dataConfig = "dbname="+pt.get<string>("dbname");
	dataConfig += " host="+pt.get<string>("host");
	dataConfig += " user="+pt.get<string>("user");
	dataConfig += " password="+pt.get<string>("password");
	dataConfig += " port="+pt.get<string>("port");
}

vector<string> Config::getRemoteDatabases(){
	if(remote_databases.size() < 1)
		remote_databases =  (new Utils())->getConnectionInfo();

	return remote_databases;
}

