#ifndef _UTILS
#define _UTILS 1
#endif
#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;

class Utils{
	public:
		void readFile();
		vector<string> getConnectionInfo();	
		static vector<string> queriesToTest();
		static bool confirmMessage(string);
		static bool confirmMessage(string, bool);
	private:
		vector<string> dbs_info;
		string makeConnectionString(vector<string>);
};
