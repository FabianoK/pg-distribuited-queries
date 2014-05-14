#ifndef _UTILS
#define _UTILS 1
#include <vector>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

using namespace std;

class Utils{
	public:
		void readFile();
		vector<string> getConnectionInfo();	
		static vector<string> queriesToTest();
		static bool confirmMessage(string);
		static bool confirmMessage(string, bool);
		static double timeDiff(struct timeval, struct timeval);
	private:
		vector<string> dbs_info;
		string makeConnectionString(vector<string>);
};

#endif
