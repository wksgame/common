#include<db/MysqlInterface.h>
#include<iostream>
#include<db/RedisDB.h>

using namespace std;
using namespace kiss::db;
using namespace hgl::redis;

int main()
{
	MysqlInterface* db = new MysqlInterface();
	if(!db->Open("test"))
	{
		printf("open test failed\n");
	}

	db->Close();

	RedisDB* rdb = ConnectRedisDB("127.0.0.1",6379);

	if(!rdb)
	{
		printf("rdb nullptr\n");
		return -1;
	}

	if(!rdb->SelectDB(0))
	{
		printf("redis db open failed\n");
	}

	return 0;
}