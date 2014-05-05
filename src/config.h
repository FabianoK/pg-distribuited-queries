#define _CONFIG 1
#include <cstdlib>
#include <boost/property_tree/ptree.hpp>
#ifndef _UTILS
#include "utils.h"
#endif
#define CONFIGFILE = sharding.conf


using namespace std;

class Config{
	private:
		static Config *conf;
		Config();
		string dbConfig;
		string dataConfig;
		void setConfig();
		boost::property_tree::ptree pt;
		vector<string> remote_databases;
		long id;
	public:
		~Config();
		static Config *getInstance();
		long getId();
		string getDBConfig();
		string getConfig(string);
		vector<string> getRemoteDatabases();

};

