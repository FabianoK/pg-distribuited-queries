#include "queries.h"

Query::Query(){}

string Query::makeQuery(Query query){

	string fields = query.fields.size() > 0 ? query.fields : "*";

	string sql = "SELECT "+fields+" FROM "+query.table;

	if(query.where.size() > 0)
		sql += " WHERE "+where;

	if(query.order.size() > 0)
		sql += " ORDER BY "+query.order;

	return sql;	

}


string Query::makeJoinQuery(Query query, string in){

	string sql = "select * from (values "+in+" (0)) as t (key), "+query.table+" t1 "+
                "where t1."+query.key_parent+" = t.key order by key_parent";
	return sql;
}
