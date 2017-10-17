#include <iostream>
#include "mongo/client/dbclient.h"

using namespace mongo;
using namespace std;

DBClientConnection * db_connect()
{
	cout << "----- DO CONNECT -----" << endl;

	// MUST do this before use driver
	client::initialize();

	bool auto_connect = true;
	double so_timeout = 3; // tcp read/write timeout
	string host = "127.0.0.1";
	string port = "27717";
	string errmsg = "";

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

int main() 
{
    try {
		DBClientConnection *pconn;
		pconn = db_connect();

		db_query(*pconn);
		db_query(*pconn, true);

		db_insert(*pconn);
		cout << "**** after insert ****" << endl;
		db_query(*pconn);

		db_update(*pconn);
		cout << "**** after update ****" << endl;
		db_query(*pconn);

		db_remove(*pconn);
		cout << "**** after remove ****" << endl;
		db_query(*pconn);
 

 		delete pconn;

    } catch( DBException &e ) {
        cout << "caught " << e.what() << endl;
    }
    return 0;
}
