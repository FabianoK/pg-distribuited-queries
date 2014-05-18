#ifndef _DB_FUNCTIONS
#include "dbfunctions.h"
#endif

class Tests{

	public:
		Tests();
		Tests(bool);
		void insertValues();
		void insertTest(int, string);
		void insertChild1(int, string);
		void insertBlob(int, string);
		void insertMulti(int, string);
		bool confirmMessage(std::string);
		int parentSize;
		int removeValues();
	private:
		DBFunctions *db;
		int removeAll(string);
		bool confirmAll;
		int main_id;
};
