#ifndef __MongoOperator_H__
#define __MongoOperator_H__

#include <string>
#include "mongo/client/dbclient.h"
#include "mongo/client/dbclient_rs.h"
#include "mongo/util/net/hostandport.h"
#include "json/json.h"

class CMongoOperator
{
private:
	mongo::DBClientBase *pconn;	
	std::string errmsg;
	
public:
	CMongoOperator();
	~CMongoOperator();
	int Connect(std::vector<mongo::HostAndPort> &servers);

	int Query(std::string db_name, Json::Value &key, Json::Value &field, int limit, int skip, Json::Value &value);
	int Insert(std::string db_name, Json::Value &value);
	int Update(std::string db_name, Json::Value &key, Json::Value &value);

};

#endif
