#include "dbfunctions.h"
#include "config.h" 
#include <pthread.h> 
#include "constants.h"
#include <sys/time.h>
#include <unistd.h>
#include "thread_execution_item.h"

using namespace std;

pthread_mutex_t item_mutex;

DBFunctions::~DBFunctions(){
}

DBFunctions::DBFunctions(){
}

void DBFunctions::acceptRemote(){

}

typedef struct orderInt{
	int idx;
  	bool operator() (Record r,  Record rr) { 
		char *i, *j;
		i = (char *)r.fields[idx].c_str();
		j = (char *)rr.fields[idx].c_str();
		return (atoi(i) < atoi(j));
	}

} orderInt;


void DBFunctions::sortByInt(vector<Record> *table, int column){
	orderInt oi;
	oi.idx = column;
	std::sort(table->begin(), table->end(), oi);
}

/**
*
* Get field index by name
* @param string 
* @param vector<Item>
*
**/

int DBFunctions::fieldIdx(string field_name, vector<Item> values){
        vector<FieldDesc> fd  = values[0].table->header;        
        for(int i = 0; i < (int)fd.size(); i++){
                if(fd[i].name == field_name){
                        return fd[i].index;

                }
        }
        return -1;

}


/**
*
* Join tables records to return a new records set
* @parm vector<Record>
* @parm vector<Record>
* @parm int
* @parm int
* @return vector<Record> 
**/

vector<Record> DBFunctions::join(vector<Record> left, vector<Record> right, int idx_l, int idx_r){
	int left_size = (int)left.size();
	int right_size = (int)right.size();
	int j = 0;
	vector<Record> ret;

	
	for(int i = 0; i < left_size; i++){

		Record *rec = new Record();

		*rec = left[i];

		if(j < right_size){
			if(left[i].fields[idx_l] == right[j].fields[idx_r]){
				rec->fields.insert(rec->fields.end(), right[j].fields.begin(), right[j].fields.end());
				j++;
				ret.push_back(*rec);
			}
		}
		if(j >= right_size)
			return ret;

		while(left[i].fields[idx_l] == right[j].fields[idx_r]){
			rec = new Record();
			*rec = left[i];
			rec->fields.insert(rec->fields.end(), right[j].fields.begin(), right[j].fields.end());
			ret.push_back(*rec);
			j++;
			if(j >= right_size)
				return ret;
		}
	}
	return ret;
}


/**
*
* Join tables records to return a left joined new records set 
* @parm vector<Record>
* @parm vector<Record>
* @parm int
* @parm int
* @return vector<Record> 
**/

vector<Record> DBFunctions::leftJoin(vector<Record> left, vector<Record> right, int idx_l, int idx_r){
	int left_size = (int)left.size();
	int right_size = (int)right.size();
	int j = 0;
	vector<Record> ret;

	
	for(int i = 0; i < left_size; i++){

		Record *rec = new Record();

		*rec = left[i];

		if(j < right_size){
			if(left[i].fields[idx_l] == right[j].fields[idx_r]){
				rec->fields.insert(rec->fields.end(), right[j].fields.begin(), right[j].fields.end());
				j++;
			}
			ret.push_back(*rec);
		}else{
			continue;
		}

		if(j >= right_size)
			continue;

		while(left[i].fields[idx_l] == right[j].fields[idx_r]){
			rec = new Record();
			*rec = left[i];
			rec->fields.insert(rec->fields.end(), right[j].fields.begin(), right[j].fields.end());
			ret.push_back(*rec);
			j++;
			if(j >= right_size)
				break;
		}
	}
	return ret;
}

/**
* Join returns in all hosts an retur a new record set
* @parm DataReturn
* @return vector<Record>
**/

vector<Record> DBFunctions::merge(DataReturn *ret){

	int vsize = (int)ret->items.size();
	vector<Record> join =  ret->items[0].table->records;

	for(int i=1; i < vsize; i++){
                vector<Record> tmp = ret->items[i].table->records;
                int tmp_size = tmp.size();

                for(int j = 0; j < tmp_size; j++)
                        join.push_back(tmp[j]);
        }

	return join;
}

/**
* Execute Query in postgreSQL SGBD and return a pointer to result memory address
* @parm string
* @parm string
* @PGresult *
**/

PGresult *DBFunctions::executeQuery(string query, string conn_string){
	PGresult *res;
	res = PQexec(conn.getConnection(conn_string), query.c_str());
	ExecStatusType status = PQresultStatus(res);
	if (status != PGRES_COMMAND_OK && status != PGRES_TUPLES_OK){
		cout << "ERROR: ";
		cout << PQerrorMessage(conn.getConnection(conn_string)) << endl;
	}

	return res;
}

/**
* Load all thread to execute queries in PostreSQL
* @parm string
* @parm DataReturn*
**/

void DBFunctions::executeRemoteQuery(string query, DataReturn *data_return){

	gettimeofday(&data_return->start_execution_time, NULL);

	using namespace boost;

	Config *cfg = Config::getInstance();
	vector<string> db = cfg->getRemoteDatabases();
	int vsize = db.size();
	pthread_t t[EXECUTION_THREADS];
	data_return->dbfunction = this;
	data_return->query = query;
	for(int i = 0; i < vsize; i++){
		ThreadExecutionItem *thread_item = new ThreadExecutionItem();
		Item *item = new Item();
		item->statistics.conn_string = db[i];
		item->statistics.id = cfg->getId();
		thread_item->item = *item;
		thread_item->data_return = data_return;
		pthread_create(&t[i], NULL, &DBFunctions::executeRemote, thread_item);
	}
	//Wait all thread execution to next step
	for(int i = 0; i < vsize; i++){
		pthread_join(t[i], NULL);
	}
	
}

/**
* Thread to execute queries in PostgreSQL
* @parm void (ThreadExecutionItem)
* @parm void*
**/
void *DBFunctions::executeRemote(void *arg){

	struct timeval start, end;

	//Parser to correct class parameter
	ThreadExecutionItem *te = static_cast<ThreadExecutionItem *>(arg);
	Item item = te->item;
	DataReturn *data_return = te->data_return;
	//Get main DBFuncitons
	DBFunctions *db = static_cast<DBFunctions *>(te->data_return->dbfunction);

	db->conn.getConnection(item.statistics.conn_string);

	gettimeofday(&start, NULL);

	PGresult *query = db->executeQuery(te->data_return->query, item.statistics.conn_string);

	gettimeofday(&end, NULL);
	
	double total = Utils::timeDiff(start, end);

	item.statistics.execution_time = total;
	item.statistics.records_returned = PQntuples(query);

	gettimeofday(&start, NULL);

	item.table = new Table();

	//Load table into item
	db->loadTable(query, item.table);

	gettimeofday(&end, NULL);

	total = Utils::timeDiff(start, end);

	item.statistics.local_process_time = total;

	pthread_mutex_lock(&item_mutex);
	//Lock last item in vector to sure write (thread safe) 
	data_return->items.back();
	data_return->items.push_back(item);
	pthread_mutex_unlock(&item_mutex);

	return NULL;

}

void DBFunctions::loadTable(PGresult *query, Table *t){

	int i, j;
        int nTuples = PQntuples(query);
        int nFields = PQnfields(query);
	char *tst;


        FieldDesc *fd = new FieldDesc();
        for(j = 0; j < nFields; j++){
                fd->name = PQfname(query, j);
                fd->type = PQftype(query, j);
                fd->size = PQfsize(query, j);
                fd->index = PQfnumber(query, fd->name.c_str());
                t->header.push_back(*fd);
        }

        for (i = 0; i < nTuples; i++){
                Record *rr = new Record();
                for(j = 0; j < nFields; j++){
                        tst = PQgetvalue(query, i, j);
                        rr->fields.push_back(tst);
                }
                t->records.push_back(*rr);
        }
}


/**
* Execute join between two tables in Query parameter
* @param Query  DBFunctionos *
* @return int 0 if sucess other if any error
**/
Table *DBFunctions::joinQuery(Query q){

	if(q.join_table == NULL){
		cout << "ERROR JOIN TABLE NOT PRESENT";
		return NULL;
	}

	string query = q.makeQuery(q);

	DataReturn *ret = new DataReturn();	
	executeRemoteQuery(query, ret);

	vector<Record> merge = this->merge(ret);
	vector<Item> values = ret->items;
	vector<FieldDesc> head = ret->items[0].table->header;

	sortByInt(&merge, 0);

	string in;

	int idx = fieldIdx(q.key, values);

	for(int i = 0; i < (int)merge.size(); i++){
		in += "("+merge[i].fields[idx] + "),";
	}

	string where_join = q.join_table->where.size() > 0 ? " and "+q.join_table->where: "";
	string order_join = q.join_table->order.size() > 0 ? q.join_table->order : q.join_table->foreign_key;

	string sql = "select "+q.join_table->fields+" from (values "+in+" (0)) as t (key), "+
		q.join_table->table +" t1 "+
		"where t1."+q.join_table->foreign_key+" = t.key "+where_join+
		" order by "+order_join;


	
	ret = new DataReturn();	

	executeRemoteQuery(sql, ret);

	vector<Record> join_merge = this->merge(ret);
	vector<Item> join_values = ret->items;
	head.insert(head.end(), ret->items[0].table->header.begin(), ret->items[0].table->header.end());
	

	//Utils::printHeader(head);

	sortByInt(&join_merge, 0);

	//Utils::printResults(values, merge);

	int idx_f = fieldIdx(q.join_table->foreign_key, join_values);

	vector<Record> join_result = this->join(merge, join_merge, idx, idx_f);


	Table *t_ret = new Table();
	t_ret->records = join_result;
	t_ret->header = head;

	for(int i = 0; i < (int)values.size(); i++)
		t_ret->statistics.push_back(values[i].statistics);

	for(int i = 0; i < (int)join_values.size(); i++)
		t_ret->statistics.push_back(join_values[i].statistics);
	//Utils::printResults(values, merge);

	return t_ret;

}


