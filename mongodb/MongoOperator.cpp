#include <stdio.h>
#include <iostream>
#include "MongoOperator.h"
#include "Base64.h"


CMongoOperator::CMongoOperator()
{	
}

CMongoOperator::~CMongoOperator()
{
}

int CMongoOperator::Connect(std::vector<mongo::HostAndPort> &servers)
{	
	printf("Connect: ----- DO CONNECT -----\n");

    // MUST do this before use driver
    // mongo::client::initialize();
    mongo::client::initialize(mongo::client::Options().setCallShutdownAtExit());

	bool flag = true;

	// CONNECT
#ifdef __MONGO_REPLICA_SET
	// for Replica Set mode
	pconn = new mongo::DBClientReplicaSet("iceman", servers, 0);
	if (pconn == NULL)
	{
		printf("CMongoOperator::Connect: new connect fail\n");
		return -1;
	}
	flag = ((mongo::DBClientReplicaSet *)pconn)->connect();
#else
	// for normal single db mode
	bool auto_connect = true;
	pconn = new mongo::DBClientConnection(auto_connect, 0, 0);
	flag = ((mongo::DBClientConnection *)pconn)->connect(servers[0], errmsg);
#endif

    if (flag == false)
    {
		printf("CMongoOperator::Connect: connect fail\n");
		return -1;
    }

	flag = pconn->auth("ice_platform_db", "lingyue", "123456", errmsg);
    if (flag == false)
    {
		printf("CMongoOperator::Connect: auth fail\n");
		return -1;
    }

	printf("CMongoOperator::Connect: connect ok\n");

    return 0;	
}

int CMongoOperator::Query(std::string db_name, Json::Value &key, Json::Value &field, int limit, int skip, Json::Value &value)
{
	mongo::BSONObj key_obj;
	if (key.size() > 0)
	{
		std::string key_str = key.toStyledString();
		int key_len = key_str.length();
		key_obj = mongo::fromjson(key_str.c_str(), &key_len);
	}

    mongo::BSONObj field_obj;
	if (field.size() > 0)
	{
		std::string field_str = field.toStyledString();
		int field_len = field_str.length();
		field_obj = mongo::fromjson(field_str.c_str(), &field_len);
	}

    int count = 0;
    try 
	{
#ifdef __MONGO_REPLICA_SET
		// read preference
		mongo::BSONArrayBuilder bab;	
		std::auto_ptr<mongo::DBClientCursor> cursor = pconn->query(db_name, mongo::Query(key_obj).readPref(mongo::ReadPreference_SecondaryPreferred, bab.arr()), limit, skip, &field_obj);
#else
		std::auto_ptr<mongo::DBClientCursor> cursor = pconn->query(db_name, key_obj, limit, skip, &field_obj);
#endif
		while (cursor->more())
		{
			mongo::BSONObj obj = cursor->next();
			std::string obj_str = obj.jsonString();

			Json::Reader reader;
			Json::Value tmp;

			reader.parse(obj_str, tmp);
			value.append(tmp);

			count++;
		}
		printf("Query:count=%d\n", count);

    } 
	catch (mongo::DBException &e) 
	{
        printf("Query:caught %s\n", e.what());
        printf("Query:fail\n");
        return -1;
    }

    return count;
}


int CMongoOperator::Insert(std::string db_name, Json::Value &value)
{

    std::string value_str = value.toStyledString();
    int value_len = value_str.length();
    mongo::BSONObj value_obj = mongo::fromjson(value_str.c_str(), &value_len);

    try 
	{
        pconn->insert(db_name, value_obj);
    }
	catch (mongo::DBException &e) 
	{
        printf("Insert:caught %s\n", e.what());
        printf("Insert:fail\n");
        return -1;
    }

    printf("Insert:success\n");
    return 0;
}

int CMongoOperator::Update(std::string db_name, Json::Value &key, Json::Value &value)
{
    mongo::BSONObj key_obj;
	if (key.size() > 0)
	{
		std::string key_str = key.toStyledString();
		int key_len = key_str.length();
		key_obj = mongo::fromjson(key_str.c_str(), &key_len);
	}

    std::string value_str = value.toStyledString();
    int value_len = value_str.length();
    mongo::BSONObj value_obj = mongo::fromjson(value_str.c_str(), &value_len);
    mongo::BSONObj data_obj = BSON("$set"<<value_obj);

    try
    {
        pconn->update(db_name, key_obj, data_obj);
    }
    catch (mongo::DBException &e)
    {
        printf("Update:caught %s\n", e.what());
        printf("Update:fail\n");
        return -1;
    }
    printf("Update:success\n");

    return 0;
}


//////////////////////////////////////////////////////////////////////

// TODO move to platform server
// client logic

int do_update_login(CMongoOperator &op, std::string openid)
{
	int ret;
	std::string db_name = "ice_platform_db.ice_account_info";

	Json::Value key;
	key["account_openid"] = openid;

    time_t now = time(NULL);
	Json::Value value;
	value["account_login"] = (int32_t)now;

	ret = op.Update(db_name, key, value);
    return ret;
}


int do_login(CMongoOperator &op, std::string openid, std::string skey)
{
	int ret;
	std::string db_name = "ice_platform_db.ice_account_info";

	Json::Value key;
	key["account_openid"] = openid;
	key["account_skey"] = skey;

	Json::Value field;
	field["account_openid"] = 1;

	int limit = 1;
	int skip = 0;

	Json::Value value;
	ret = op.Query(db_name, key, field, limit, skip, value);

	if (ret == 0)
	{
		// openid & skey mismatch
		return -1;
	}

	std::cout << "value=" << value << std::endl;
	ret = do_update_login(op, openid);
	return 0;
}

int do_register(CMongoOperator &op, std::string openid, std::string skey)
{
	int ret;

	std::string db_name = "ice_platform_db.ice_account_info";

    time_t now = time(NULL);
	
	Json::Value value;
	value["account_openid"] = openid;
	value["account_skey"] = skey;
	value["account_info"] = "";
	value["account_create"] = (int32_t)now;
	value["account_login"] = (int32_t)now;

	ret = op.Insert(db_name, value);

	if (ret != 0)
	{
		ret = do_login(op, openid, skey);
		if (ret != 0)
		{
			// openid & skey mismatch
			return -1;
		}
		// duplicate register
		return 0;
	}
	// register success
	return 0;
}

int do_update_account_info(CMongoOperator &op, std::string openid, std::string info)
{
	int ret;
	std::string db_name = "ice_platform_db.ice_account_info";

	Json::Value key;
	key["account_openid"] = openid;

	Json::Value value;
	value["account_info"] = info;

	ret = op.Update(db_name, key, value);
    return ret;
}

int do_get_account_data(CMongoOperator &op, std::string openid, Json::Value &value)
{
	int ret;
	std::string db_name = "ice_platform_db.ice_account_info";

	Json::Value key;
	key["account_openid"] = openid;

	Json::Value field;
	field["account_openid"] = 1;
	field["account_skey"] = 1;
	field["account_info"] = 1;
	field["account_create"] = 1;
	field["account_login"] = 1;

	int limit = 1;
	int skip = 0;

	ret = op.Query(db_name, key, field, limit, skip, value);

	if (ret > 0)
	{
		std::cout << "value = " << value << std::endl;
	}

	return ret;
}

int do_get_all_player_name(CMongoOperator &op, Json::Value &value)
{
	int ret;
	std::string db_name = "ice_platform_db.ice_player_register";

	Json::Value key;

	Json::Value field;
	field["player_name"] = 1;
	field["player_id"] = 1;
	field["account"] = 1;
	field["game_server"] = 1;
	field["id_del"] = 1;

	int limit = 0;
	int skip = 0;

	ret = op.Query(db_name, key, field, limit, skip, value);

	if (ret > 0)
	{
		std::cout << "value = " << value << std::endl;
	}

	return ret;
}

int do_add_player_name(CMongoOperator &op, std::string player_name, std::string player_id, std::string account, int game_server, bool is_del)
{
	int ret;

	std::string db_name = "ice_platform_db.ice_player_register";

	Json::Value value;
	value["player_name"] = player_name;
	value["player_id"] = player_id;
	value["account"] = account;
	value["game_server"] = game_server;
	value["is_del"] = is_del;

	ret = op.Insert(db_name, value);

	return ret;
}

int do_del_player_name(CMongoOperator &op, std::string player_name, bool is_del)
{
	int ret;
	std::string db_name = "ice_platform_db.ice_player_register";

	Json::Value key;
	key["player_name"] = player_name;

	Json::Value value;
	value["is_del"] = is_del;

	ret = op.Update(db_name, key, value);
    return ret;
}

#ifdef TTT

int main(int argc, char **argv)
{
	printf("hello MongoOperator\n");

	CMongoOperator op;
	std::vector<mongo::HostAndPort> servers;
	servers.push_back(mongo::HostAndPort("127.0.0.1:27017"));
	servers.push_back(mongo::HostAndPort("127.0.0.1:27018"));
	op.Connect(servers);

	int ret;

	std::string openid = "masha";
	std::string skey = "123456";
	std::string player_name = "masha151218a";
	std::string player_id = "1658853808680059";
	Json::Value data;

	printf("\n------ do register -------\n");
	ret = do_register(op, openid, skey);
	printf("ret = %d\n", ret);

	printf("\n------ do login -------\n");
	ret = do_login(op, openid, skey);
	printf("ret = %d\n", ret);

	printf("\n------ do register -------\n");
	ret = do_register(op, openid, skey);
	printf("ret = %d\n", ret);

	printf("\n------ do login skey mismatch -------\n");
	ret = do_login(op, openid, "123");
	printf("ret = %d\n", ret);

	printf("\n------ do register skey mismatch -------\n");
	ret = do_register(op, openid, "123");
	printf("ret = %d\n", ret);


	Base64 base64;

	printf("\n------ do get account data -------\n");
	ret = do_get_account_data(op, openid, data);
	printf("ret = %d\n", ret);
	if (ret > 0)
	{
		for (int i=0; i<data.size(); i++)
		{
			Json::Value &ttt = data[i];
			std::string real;
			std::string account_info = ttt["account_info"].asString();
			base64.decode(account_info, real);
			printf("real=%s real.length()=%lu\n", real.c_str(), real.length());
		}
	}
	
	std::string info;
	std::string info64;
	printf("\n------ do update account data -------\n");
	info = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	base64.encode(info, info64, false);
	printf("info64=%s\n", info64.c_str());
	ret = do_update_account_info(op, openid, info64);
	printf("ret = %d\n", ret);

	info = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
	base64.encode(info, info64, false);
	printf("info64=%s\n", info64.c_str());
	ret = do_update_account_info(op, openid, info64);
	printf("ret = %d\n", ret);

	info = "cccccccccccccccccccccccccccccccc";
	base64.encode(info, info64, false);
	printf("info64=%s\n", info64.c_str());
	ret = do_update_account_info(op, openid, info64);
	printf("ret = %d\n", ret);

	info = "dddddddddddddddddddddddddddddddd";
	base64.encode(info, info64, false);
	printf("info64=%s\n", info64.c_str());
	ret = do_update_account_info(op, openid, info64);
	printf("ret = %d\n", ret);

	printf("\n------ do get account data -------\n");
	data.clear();
	ret = do_get_account_data(op, openid, data);
	printf("ret = %d\n", ret);
	if (ret > 0)
	{
		for (int i=0; i<data.size(); i++)
		{
			Json::Value &ttt = data[i];
			std::string real;
			std::string account_info = ttt["account_info"].asString();
			base64.decode(account_info, real);
			printf("real=%s real.length()=%lu\n", real.c_str(), real.length());
		}
	}

	/*
	int count = 0;
	while (true)
	{

	printf("\n------ do get all player name -------\n");
	data.clear();
	ret = do_get_all_player_name(op, data);
	printf("ret = %d\n", ret);

	printf("\n------ do add player name -------\n");
	ret = do_add_player_name(op, player_name, player_id, openid, 1, false);
	printf("ret = %d\n", ret);

	printf("\n------ do del player name -------\n");
	ret = do_del_player_name(op, player_name, true);
	printf("ret = %d\n", ret);

	printf("count=%d\n", ++count);
	sleep(5);
	}
	*/

	printf("\n------ done -------\n");

	return 0;
}

#endif


