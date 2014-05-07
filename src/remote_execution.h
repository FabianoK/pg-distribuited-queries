#ifndef _RESULT
#include "result.h"
#endif

#ifndef _DB_FUNCTIONS
#include "dbfunctions.h"
#endif

class RemoteExecution{
	public:
		std::string conn;
		std::string query;
		DBFunctions *db_functions;
		long id;
		Result *process_result;
};
