#include<db/MysqlInterface.h>
#include<iostream>

using namespace std;
using namespace kiss::db;

int main()
{
	MysqlInterface* db = new MysqlInterface();
	if(!db->Open("test"))
	{
		printf("open test failed\n");
		return -1;
	}
	return 0;
}