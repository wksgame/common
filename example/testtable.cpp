#include"other/ConfigTable.h"

using namespace std;
using namespace kiss;

struct TestData
{
	int id;
	char name[32];
};

class TestTable : public ConfigTable<TestData>
{
protected:
	void CheckData(TestData* t)override
	{
		printf("%d %s\n",t->id,t->name);
	}
	
	void MapField()override
	{
		Map("id",offsetof(TestData,id));
	}
};

int main()
{
	TestTable table;
	table.Parse("test.table");
	
	return 0;
}