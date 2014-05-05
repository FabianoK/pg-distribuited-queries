#ifndef _DB_FUNCTIONS
#include "dbfunctions.h"
#endif

class Tests{

	public:
		Tests();
		static void insertValues();
		void insertTest(int);
		void insertChild1(int);
		void insertBlob(int);
		void insertMulti(int);
		bool confirmMessage(std::string);
		int parentSize;
		static int removeValues();
	private:
		DBFunctions *db;
		int removeAll();
};
