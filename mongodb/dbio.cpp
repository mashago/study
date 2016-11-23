#include <time.h>
#include <iostream>
#include "mongo/client/dbclient.h"
#include "Base64.h"
#include "json/json.h"

using namespace mongo;
using namespace std;

#define MONGODB_HOST "127.0.0.1"
#define MONGODB_PORT "27717"

DBClientConnection * db_connect(string &errmsg)
{
	cout << "----- DO CONNECT -----" << endl;

	// MUST do this before use driver
	client::initialize();

	bool auto_connect = true;
	double so_timeout = 3; // tcp read/write timeout
	string host = MONGODB_HOST;
	string port = MONGODB_PORT;

	// CONNECT
	DBClientConnection *pconn = new DBClientConnection(auto_connect, 0, so_timeout);
	bool flag = pconn->connect(host+":"+port, errmsg);
	if (flag == false)
	{
		cout << "connect fail" << endl;
		throw DBException("connect fail", -1);
	}
	cout << "connect ok" << endl;

	return pconn;
}

void db_query(DBClientConnection &conn, bool explain = false);
void db_query(DBClientConnection &conn, bool explain)
{
	cout << "----- DO QUERY -----" << endl;

	// target collection
	string db = "test";
	string collection = "table_a";
	string ns = db+"."+collection;

	// condition:
	// Query condition = BSON("uid"<<111); // means: uid=111
	Query condition = Query();
	if (explain)
	{
		condition.explain();
	}

	// as sql limit [nToReturn], [nToSkip]
	int nToReturn = 10; 
	int nToSkip = 0;

	// show column
	BSONObj fieldsToReturn = BSON("uid"<<1<<"username"<<1<<"age"<<1);

	int queryOptions = 0;
	int batchSize = 0;

	auto_ptr<DBClientCursor> cursor = conn.query(ns, condition, nToReturn, nToSkip, &fieldsToReturn, queryOptions, batchSize);

	if (explain)
	{
		while (cursor->more())
		{
			BSONObj p = cursor->next();
			string str = p.toString();
			cout << "str=" << str << endl;
		}
	}
	else
	{
		int count = 0;
		int uid = 0;
		string username = "";
		int age = 0;
		while (cursor->more())
		{
			BSONObj p = cursor->next();
			uid = p.getIntField("uid");
			username = p.getStringField("username");
			age = p.getIntField("age");
			cout << "uid=" << uid << " username=" << username << " age=" << age << endl;
			count++;
		}
		cout << "count = " << count << endl;
	}
}

void db_insert(DBClientConnection &conn)
{
	cout << "----- DO INSERT -----" << endl;

	string errmsg;

	// target collection
	string db = "test";
	string collection = "table_a";
	string ns = db+"."+collection;

	// obj
	BSONObj obj = BSON("uid"<<333<<"username"<<"jack"<<"age"<<40);

	int flag = 0;
	WriteConcern *mc = NULL;

	cout << "before insert" << endl;
	conn.insert(ns, obj, flag, mc);
	// conn.insert(ns, obj);
	cout << "after insert" << endl;
	errmsg = conn.getLastError();
	cout << "errmsg=" << errmsg << endl;
}

void db_update(DBClientConnection &conn)
{
	cout << "----- DO UPDATE -----" << endl;

	string errmsg;

	// target collection
	string db = "test";
	string collection = "table_a";
	string ns = db+"."+collection;

	// condition : uid=333
	Query condition = BSON("uid"<<333);

	// obj
	BSONObj obj = BSON("username"<<"jack_edit"<<"age"<<50);
	// BSONObjBuilder data;
	// data.append("$set", obj);
	BSONObj data = BSON("$set"<<obj);

	bool upsert = false;
	bool multi = false;
	WriteConcern *mc = NULL;

	// conn.update(ns, condition, data.obj(), upsert, multi, mc);
	conn.update(ns, condition, data, upsert, multi, mc);
	errmsg = conn.getLastError();
	cout << "errmsg=" << errmsg << endl;
}

void db_remove(DBClientConnection &conn)
{
	cout << "----- DO REMOVE -----" << endl;

	string errmsg;

	// target collection
	string db = "test";
	string collection = "table_a";
	string ns = db+"."+collection;

	// condition
	Query condition = BSON("uid"<<333);

	bool justOne = true;
	WriteConcern *mc = NULL;

	conn.remove(ns, condition, justOne, mc);
	errmsg = conn.getLastError();
	cout << "errmsg=" << errmsg << endl;
}

///////////////////////////////////////////////////////////////////////////////////////



int do_update_login(DBClientConnection &conn, string openid);
int do_login(DBClientConnection &conn, string openid, string skey);
int do_register(DBClientConnection &conn, string openid, string skey);
int do_update_info(DBClientConnection &conn, string openid, string updateStr);
string do_get_info(DBClientConnection &conn, string openid);

int do_get_all_player_name(DBClientConnection &conn);
int do_add_player_name(DBClientConnection &conn, string playername, int playerid, string account, int gameserver, bool isdel);
int do_del_player_name(DBClientConnection &conn, string playername, bool isdel);

int do_update_login(DBClientConnection &conn, string openid)
{
	cout << "----- DO UPDATE LOGIN -----" << endl;

	string errmsg;

	// target collection
	string db = "ice_platform_db";
	string collection = "ice_account_info";
	string ns = db+"."+collection;

	// condition : account_openid=xxx
	Query condition = BSON("account_openid"<<openid);

	// obj
	int64_t now = (int64_t)time(NULL);
	BSONObj obj = BSON("account_login"<<now);
	BSONObj data = BSON("$set"<<obj);

	bool upsert = false;
	bool multi = false;
	WriteConcern *mc = NULL;

    try {
		conn.update(ns, condition, data, upsert, multi, mc);
    } catch( DBException &e ) {
        cout << "caught " << e.what() << endl;
		errmsg = conn.getLastError();
		cout << "errmsg=" << errmsg << endl;

		return -1;
    }
	cout << "do_update_login:success" << endl;

	return 0;
}

int do_login(DBClientConnection &conn, string openid, string skey)
{
	// select * from ice_account_info where openid and skey
	cout << "----- DO LOGIN -----" << endl;

	// target collection
	string db = "ice_platform_db";
	string collection = "ice_account_info";
	string ns = db+"."+collection;

	// condition:
	// means: account_openid=xxx and account_skey=xxx
	Query condition = BSON("account_openid"<<openid<<"account_skey"<<skey); 

	// as sql limit [nToReturn], [nToSkip]
	int nToReturn = 1; 
	int nToSkip = 0;

	// show column
	BSONObj fieldsToReturn = BSON("account_openid"<<1);

	int queryOptions = 0;
	int batchSize = 0;

	auto_ptr<DBClientCursor> cursor = conn.query(ns, condition, nToReturn, nToSkip, &fieldsToReturn, queryOptions, batchSize);

	int count = 0;
	int uid = 0;
	string tmp = "";
	int age = 0;
	while (cursor->more())
	{
		BSONObj p = cursor->next();
		tmp = p.getStringField("account_openid");
		cout << "account_openid=" << tmp << endl;
		count++;
	}
	cout << "count = " << count << endl;

	if (count == 0)
	{
		return -1;
	}

	do_update_login(conn, openid);

	cout << "do_login:success" << endl;
	
	return 0;
}

int do_register(DBClientConnection &conn, string openid, string skey)
{
	// 1.insert into ice_account_info
	// 2.if fail, do login
	// 3.if fail, skey mismatch

	cout << "----- DO REGISTER -----" << endl;

	// target collection
	string db = "ice_platform_db";
	string collection = "ice_account_info";
	string ns = db+"."+collection;

	int64_t now = (int64_t)time(NULL);

	// obj
	BSONObj obj = BSON("account_openid"<<openid<<"account_skey"<<skey<<"account_info"<<""<<"account_create"<<now<<"account_login"<<now);
	// BSONObj obj = BSON("account_openid"<<openid<<"account_skey"<<skey<<"account_info"<<"");

	int flag = 0;
	WriteConcern *mc = NULL;

	bool insert_flag = true;
    try {
		// note: insert fail will throw exception
		conn.insert(ns, obj, flag, mc);
    } catch( DBException &e ) {
        cout << "caught " << e.what() << endl;
		insert_flag = false;
    }

	if (insert_flag)
	{
		printf("do_register:register success\n");
		return 0;
	}

	int ret = do_login(conn, openid, skey);
	if (ret != 0)
	{
		printf("do_register:openid and skey mismatch\n");
		return -1;
	}

	printf("do_register:duplicate register\n");
	return 0;
}

int do_update_info(DBClientConnection &conn, string openid, string updateStr)
{
	cout << "----- DO UPDATE INFO -----" << endl;

	string errmsg;

	// target collection
	string db = "ice_platform_db";
	string collection = "ice_account_info";
	string ns = db+"."+collection;

	// condition : account_openid=xxx
	Query condition = BSON("account_openid"<<openid);

	// obj
	// updateStr should be a base64 string
	BSONObj obj = BSON("account_info"<<updateStr);
	BSONObj data = BSON("$set"<<obj);

	bool upsert = false;
	bool multi = false;
	WriteConcern *mc = NULL;

    try {
		conn.update(ns, condition, data, upsert, multi, mc);
    } catch( DBException &e ) {
        cout << "caught " << e.what() << endl;
		errmsg = conn.getLastError();
		cout << "errmsg=" << errmsg << endl;

		return -1;
    }

	printf("do_update_info:success\n");
	return 0;
}

string do_get_info(DBClientConnection &conn, string openid)
{
	cout << "----- DO GET INFO -----" << endl;

	// target collection
	string db = "ice_platform_db";
	string collection = "ice_account_info";
	string ns = db+"."+collection;

	// condition:
	// means: account_openid=xxx
	Query condition = BSON("account_openid"<<openid);

	// as sql limit [nToReturn], [nToSkip]
	int nToReturn = 1; 
	int nToSkip = 0;

	// show column
	// BSONObj fieldsToReturn = BSON("account_openid"<<1);

	int queryOptions = 0;
	int batchSize = 0;

	auto_ptr<DBClientCursor> cursor = conn.query(ns, condition, nToReturn, nToSkip, NULL, queryOptions, batchSize);

	int count = 0;
	int uid = 0;
	string account_openid = "";
	string account_skey = "";
	string account_info = "";
	int64_t account_create = 0;
	int64_t account_login = 0;
	int age = 0;
	while (cursor->more())
	{
		BSONObj p = cursor->next();
		account_openid = p.getStringField("account_openid");
		account_skey = p.getStringField("account_skey");
		account_info = p.getStringField("account_info");
		account_create = p.getIntField("account_create");
		account_login = p.getIntField("account_login");
		printf("account_openid=%s account_skey=%s account_info=%s account_create=%lld account_login=%lld\n", account_openid.c_str(), account_skey.c_str(), account_info.c_str(), account_create, account_login);
		count++;
	}
	cout << "count = " << count << endl;

	return account_info;
}

int do_get_all_player_name(DBClientConnection &conn)
{
	cout << "----- DO GET ALL PLAYER NAME -----" << endl;

	// target collection
	string db = "ice_platform_db";
	string collection = "ice_player_register";
	string ns = db+"."+collection;

	// condition:
	Query condition;

	// as sql limit [nToReturn], [nToSkip]
	int nToReturn = 0; 
	int nToSkip = 0;

	int queryOptions = 0;
	int batchSize = 0;

	auto_ptr<DBClientCursor> cursor = conn.query(ns, condition, nToReturn, nToSkip, NULL, queryOptions, batchSize);

	int count = 0;
	int uid = 0;
	string player_name = "";
	int player_id = 0;
	string account = "";
	int game_server = 0;
	bool is_del = false;
	while (cursor->more())
	{
		BSONObj p = cursor->next();
		player_name = p.getStringField("player_name");
		player_id = p.getIntField("player_id");
		account = p.getStringField("account");
		game_server = p.getIntField("game_server");
		is_del = p.getBoolField("is_del");
		cout << "player_name=" << player_name << " player_id=" << player_id << " account=" << account << " game_server=" << game_server << " is_del=" << is_del << endl;
		count++;
	}
	cout << "count = " << count << endl;

	if (count == 0)
	{
		return -1;
	}
	
	return 0;
}

int do_add_player_name(DBClientConnection &conn, string playername, int playerid, string account, int gameserver, bool isdel)
{
	cout << "----- DO ADD PLAYER NAME -----" << endl;

	// target collection
	string db = "ice_platform_db";
	string collection = "ice_player_register";
	string ns = db+"."+collection;

	// obj
	BSONObj obj = BSON("player_name"<<playername<<"player_id"<<playerid<<"account"<<account<<"game_server"<<gameserver<<"is_del"<<isdel);

	int flag = 0;
	WriteConcern *mc = NULL;

	bool insert_flag = true;
    try {
		// note: insert fail will throw exception
		conn.insert(ns, obj, flag, mc);
    } catch( DBException &e ) {
        cout << "caught " << e.what() << endl;
		insert_flag = false;
    }

	if (!insert_flag)
	{
		printf("do_add_player_name:add fail\n");
		return -1;
	}

	printf("do_add_player_name:add success\n");
	return 0;
}

int do_del_player_name(DBClientConnection &conn, string playername, bool isdel)
{
	cout << "----- DO DEL PLAYER NAME -----" << endl;

	string errmsg;

	// target collection
	string db = "ice_platform_db";
	string collection = "ice_player_register";
	string ns = db+"."+collection;

	// condition :
	Query condition = BSON("player_name"<<playername);

	// obj
	BSONObj obj = BSON("is_del"<<isdel);
	BSONObj data = BSON("$set"<<obj);

	bool upsert = false;
	bool multi = false;
	WriteConcern *mc = NULL;

    try {
		conn.update(ns, condition, data, upsert, multi, mc);
    } catch( DBException &e ) {
        cout << "caught " << e.what() << endl;
		errmsg = conn.getLastError();
		cout << "errmsg=" << errmsg << endl;

		return -1;
    }
	return 0;
}

////////////////////////////////////////////////////////////

int db_update(DBClientConnection &conn, std::string db_name, Json::Value &key, Json::Value &value)
{
	std::string key_str = key.toStyledString();
	int key_len = key_str.length();
	mongo::BSONObj key_obj = mongo::fromjson(key_str.c_str(), &key_len);

	std::string value_str = value.toStyledString();
	int value_len = value_str.length();
	mongo::BSONObj value_obj = mongo::fromjson(value_str.c_str(), &value_len);
	mongo::BSONObj data_obj = BSON("$set"<<value_obj);

    try 
	{
		conn.update(db_name, key_obj, data_obj);
    } 
	catch (DBException &e) 
	{
		printf("db_update:caught %s\n", e.what());
		printf("db_update:fail\n");
		return -1;
    }
	printf("db_update:success\n");

	return 0;
}

int db_insert(DBClientConnection &conn, std::string db_name, Json::Value &value)
{

	std::string value_str = value.toStyledString();
	int value_len = value_str.length();
	mongo::BSONObj value_obj = mongo::fromjson(value_str.c_str(), &value_len);

    try {
		conn.insert(db_name, value_obj);
    } catch( DBException &e ) {
		printf("db_insert:caught %s\n", e.what());
		printf("db_insert:fail\n");
		return -1;
	}
	printf("db_insert:success\n");

	return 0;
}

int db_query(DBClientConnection &conn, std::string db_name, Json::Value &key, Json::Value &field, int limit, int skip, Json::Value &value)
{
	std::string key_str = key.toStyledString();
	int key_len = key_str.length();
	mongo::BSONObj key_obj = mongo::fromjson(key_str.c_str(), &key_len);

	std::string field_str = field.toStyledString();
	int field_len = field_str.length();
	mongo::BSONObj field_obj = mongo::fromjson(field_str.c_str(), &field_len);

	auto_ptr<DBClientCursor> cursor = conn.query(db_name, key_obj, limit, skip, &field_obj);

	int count = 0;
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

	printf("do_query:count=%d\n", count);
	
	return count;
}

// TODO sort query

int main() 
{
    try {
		DBClientConnection *pconn;
		string errmsg = "";
		pconn = db_connect(errmsg);

		int ret;

		string openid = "masha";
		string skey = "123456";

		///////////////////////////////////////////

		ret = do_login(*pconn, openid, skey);
		printf("do_login:ret=%d\n", ret);

		///////////////////////////////////////////

		ret = do_register(*pconn, openid, skey);
		printf("do_register:ret=%d\n", ret);

		ret = do_login(*pconn, openid, skey);
		printf("do_login:ret=%d\n", ret);

		///////////////////////////////////////////

		char tmp_data[100];
		bzero(tmp_data, sizeof(tmp_data));
		for (int i=0; i<100; i++)
		{
			tmp_data[i] = ('\0' + i);
		}

		string infoStr;
		infoStr = string(tmp_data, 100);
		// printf("infoStr.length()=%d\n", infoStr.length());

		string infoStr64;
		Base64 base64;
		base64.encode(infoStr, infoStr64, false);

		// printf("infoStr64=[%s] infoStr64.length()=%d\n", infoStr64.c_str(), infoStr64.length());

		ret = do_update_info(*pconn, openid, infoStr64);
		printf("do_update_info:ret=%d\n", ret);

		///////////////////////////////////////////

		string retStr64 = do_get_info(*pconn, openid);
		printf("do_update_info:retStr64=%s\n", retStr64.c_str());

		string retStr;
		base64.decode(retStr64, retStr);
		printf("retStr.length()=%lu\n", retStr.length());

		///////////////////////////////////////////

		ret = do_get_all_player_name(*pconn);
		printf("do_get_all_player_name:ret=%d\n", ret);

		///////////////////////////////////////////
		
		string playername = "masha151216a";
		int playerid = 123456789;
		string account = "masha";
		int gameserver = 1;
		bool isdel = false;
		ret = do_add_player_name(*pconn, playername, playerid, account, gameserver, isdel);
		printf("do_add_player_name:ret=%d\n", ret);

		///////////////////////////////////////////

		isdel = true;
		ret = do_del_player_name(*pconn, playername, isdel);
		printf("do_add_player_name:ret=%d\n", ret);

 		delete pconn;

    } catch( DBException &e ) {
        cout << "caught " << e.what() << endl;
    }
    return 0;
}
