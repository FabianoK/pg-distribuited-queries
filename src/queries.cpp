#include "queries.h"

Query::Query(){
	this->join_table = NULL;
	this->fields = "*";
}

Query::Query(string table){
	this->table = table;
	this->join_table = NULL;
	this->fields = "*";
}

Query::Query(string table, string where){
	this->table = table;
	this->where = where;
	this->join_table = NULL;
	this->fields = "*";
}

Query::Query(string table, string where, string order){
	this->table = table;
	this->where = where;
	this->order = order;
	this->join_table = NULL;
}

string Query::makeQuery(Query query){

	string fields = query.fields.size() > 0 ? query.fields : "*";

	string sql = "SELECT "+fields+" FROM "+query.table;

	if(query.where.size() > 0)
		sql += " WHERE "+where;

	if(query.order.size() > 0)
		sql += " ORDER BY "+query.order;

	return sql;	

}


Query *Query::join(string table, string key, string key_parent){
	Query *q = new Query(table);
	q->foreign_key = key_parent;
	this->join_table = q;
	this->key = key;
	return this->join_table;
}

string Query::makeJoinQuery(Query query, string in){

	Query *join_table = static_cast<Query *>(query.join_table);
	string sql = "select * from (values "+in+" (0)) as t (key), "+query.table+" t1 "+
                "where t1."+join_table->table+" = t.key order by key_parent";
	return sql;
}
