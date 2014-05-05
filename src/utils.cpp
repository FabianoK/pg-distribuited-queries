#include "utils.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "constants.h"
#include "config.h"

using namespace std;


bool Utils::confirmMessage(string msg){
	string confirm;
        cout << msg +" [Y\\n]: ";
        cin >> confirm;

	boost::to_upper(confirm);

        if(confirm == "Y" || confirm == "N"){
		if(confirm == "N")
                	return false;
	}else{
		cout << "Pleace press only y or Y or n or N" << endl;
		return confirmMessage(msg);
	}

        return true;


}

vector<string> Utils::getConnectionInfo(){
	if(dbs_info.size() < 1)
		readFile();
	return dbs_info;
}

void Utils::readFile(){
	ifstream hosts (CONFIG_HOSTS);
	string line;
	string tokens;
	vector<string> hosts_data;

	if (hosts.is_open()){
		while (getline(hosts,line)){
			boost::algorithm::erase_all(line, " ");
			boost::algorithm::erase_all(line, "\t");
			boost::split(hosts_data,line,boost::is_any_of("|"));
			if(line.c_str()[0] == '#' || hosts_data.size() < 4)
				continue;
			
			this->dbs_info.push_back(makeConnectionString(hosts_data));
		}
		
	}
}

vector<string> Utils::queriesToTest(){

	Config *c = Config::getInstance();

	ifstream queries (c->getConfig("query_files").c_str());
	string line;
	vector<string> ret;

	if (queries.is_open()){
		while (getline(queries,line)){
			boost::algorithm::trim(line);
			boost::algorithm::erase_all(line, "\t");
			if(line.c_str()[0] == '#')
				continue;
			
			ret.push_back(line);
		}
		
	}

	return ret;
}

string Utils::makeConnectionString(vector<string> vec){

	string dataConfig; 
	dataConfig = "dbname="+vec[4];
        dataConfig += " host="+vec[0];
        dataConfig += " user="+vec[2];
        dataConfig += " password="+vec[3];
        dataConfig += " port="+vec[1];
	
	return dataConfig;
}
