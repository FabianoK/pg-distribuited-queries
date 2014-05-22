#include "utils.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include "constants.h"
#include "config.h"

using namespace std;


double Utils::timeDiff(struct timeval start, struct timeval end){

	return ((( end.tv_sec - start.tv_sec ) *1000000L)
             + ((double)( end.tv_usec - start.tv_usec )))/1000000L;

}

bool Utils::confirmMessage(string msg, bool confirm_all){
	if(confirm_all){
        	cout << msg +" [Y\\n]: Y" <<endl;
		return true;
	}else
		return Utils::confirmMessage(msg);
		
}

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


void Utils::printResults(Item it, vector<Record> merge){
	string host = it.statistics.conn_string.substr(0, it.statistics.conn_string.find(" user="));
	cout << host << ";"<< it.statistics.execution_time << ";" << it.statistics.local_process_time << ";" << it.statistics.records_returned << endl;
}

void Utils::printResults(vector<ExecutionData> values, vector<Record> merge){

        int vsize = (int)values.size();

        cout << "TOTAL SIZE: " << merge.size() << endl;

        cout << "HOST; EXECUTION TIME; LOCAL PROCESS TIME; RECORDS RETURN " << endl;

        for(int i=0; i < vsize; i++){
		string host = values[i].conn_string.substr(0, values[i].conn_string.find(" user="));
		cout << host << ";"<< values[i].execution_time << ";" << values[i].local_process_time << 
		";" << values[i].records_returned << endl;
        }
}

void Utils::printValues(vector<Item> values, vector<Record> merge){

        vector<FieldDesc> f = values[0].table->header;

        int hsize = (int)f.size();


        for(int i = 0; i < 100; i++)
                cout << "#";

        cout << endl;


        for(int i = 0;i < hsize; i++)
                cout << f[i].name << "|" << f[i].type << "|" << f[i].size << ";";

        cout << endl;


        for(int k = 0; k < (int)merge.size(); k++){
                for(int kk = 0; kk < (int)merge[k].fields.size(); kk++)
                        cout << merge[k].fields[kk] << "|";

                cout << endl;
        }
}

/**
*
* Print table fields name 
* @parame Table
*
**/

void Utils::printHeader(vector<FieldDesc> fd){

        for(int i = 0; i < (int)fd.size(); i++)
                cout << fd[i].name << " | ";

        cout << endl;

}
