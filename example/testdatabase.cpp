#include<db/MysqlInterface.h>
#include<iostream>
#include<db/RedisDB.h>
#include<vector>

using namespace std;
using namespace kiss::db;
using namespace hgl::redis;

int main()
{
	MysqlInterface* db = new MysqlInterface();
	if(!db->Open("ShowhandGameLog","root","showhand_log"))
	{
		printf("open test failed\n");
	}

	auto func = [](char** result, int nCol, void* arg)
	{
		vector<string>* roles = (vector<string>*)arg;

		roles->push_back(result[0]);
	};

	vector<string> result;

	db->Select("select username from Account", func, &result);

	for(auto& i :result)
		printf("%s\n",i.c_str());

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