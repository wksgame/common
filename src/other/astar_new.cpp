#include<vector>
#include<stdlib.h>
#include<algorithm>
#include<queue>
#include<iostream>

using namespace std;

class CPos
{
public:
	CPos();
	CPos(const int v, const int h);
	CPos(const CPos&rhs);

	CPos& operator=(const CPos&rhs);
	bool operator==(const CPos& rhs)const;

	int v;
	int h;
};

CPos::CPos()
{
	v = 0;
	h = 0;
}

CPos::CPos(const int v, const int h)
{
	this->v = v;
	this->h = h;
}

CPos::CPos(const CPos&rhs)
{
	v = rhs.v;
	h = rhs.h;
}

CPos& CPos::operator =(const CPos&rhs)
{
	v = rhs.v;
	h = rhs.h;
	return *this;
}

bool CPos::operator ==(const CPos& rhs) const
{
	return(v == rhs.v && h == rhs.h);
}

////////////////////////////////////////////////////////
class CNode
{
public:
	CNode();
	virtual ~CNode();

	CNode& operator=(const CNode&rhs);
	bool operator<(const CNode& rhs) const;

	CPos pos;
	int f;
	int h;
	int g;
	int dire;
};

CNode::CNode()
{
	f = 0;
	h = 0;
	g = 0;
	dire = 0;
}

CNode::~CNode()
{

}

CNode& CNode::operator =(const CNode&rhs)
{
	pos = rhs.pos;
	f	= rhs.f;
	h	= rhs.h;
	g	= rhs.g;
	dire= rhs.dire;
	return *this;
}

bool CNode::operator<(const CNode& rhs)const
{
	return f > rhs.f;
}

	const int vcount=10;
	const int hcount=10;

//////////////////////////////////////////////////////////////////
class CMap
{
public:
	CMap()
	{
		for(int i=0; i<vcount; ++i)
		{
			for(int j=0; j<hcount; ++j)
			{
				bmpMap[i][j]=rand()%5?0:1;
				objMap[i][j]=0;
			}
		}

		for(int i=0; i<vcount; ++i)
		{
			for(int j=0; j<hcount; ++j)
			{
				cout<<(bmpMap[i][j]);
			}
			cout<<endl;
		}
	}

	int bmpMap[vcount][hcount];
	int objMap[vcount][hcount];

	bool IsValid(const CPos& pos)		// 判断位置是否有效
	{
		if(pos.v<0 || pos.h<0 || pos.v>=vcount || pos.h>=hcount)
			return false;

		if(bmpMap[pos.v][pos.h] == 1)
			return false;

		if(objMap[pos.v][pos.h] == 1)
			return false;

		return true;
	}
};

//////////////////////////////////////////////////////////////////
class CSearch
{
public:
	CSearch(CMap* tgmap);
	~CSearch();

	bool SetPos(const CPos& from, const CPos& to);		// 设置起点和终点
	bool FindPath();									// 寻路

private:

	bool TestMove();									// 测试是否能移动
	int Getg();											// 获取测试位置到终点的估算值

	static int cv[4];
	static int ch[4];
	static int len[4];

	CPos src;		//源位置
	CPos dest;		//目标位置
	CNode current;	//当前移动位置
	CNode test;		//测试位置
	int dire;		//测试方向
//	vector<CNode> opentable;
	vector<CNode> closetable;
	vector<CNode> pathtable;
	CMap* tgmap;

	priority_queue<CNode> opentable;
};

CSearch::CSearch(CMap* tgmap)
{
	this->tgmap	= tgmap;
}

CSearch::~CSearch()
{
	tgmap = NULL;
}

bool CSearch::SetPos(const CPos& from, const CPos& to)
{
	if(from == to)
		return false;

	if(!tgmap->IsValid(from))
		return false;

	if(!tgmap->IsValid(to))
		return false;

	src = from;
	dest = to;

	current.pos = src;
	current.g = Getg();
	current.h = 0;
	current.f = current.g + current.h;

	opentable.push(current);

	test.pos = src;
	return true;
}

int CSearch::cv[4]={-1, 0, 1, 0};
int CSearch::ch[4]={ 0, 1, 0,-1};
int CSearch::len[4]={10,10,10,10};

int CSearch::Getg()
{
	int vlen = abs(test.pos.v - dest.v);
	int hlen = abs(test.pos.h - dest.h);
	return (vlen+hlen)*10;
}

bool CSearch::TestMove()
{
	test.pos.v = current.pos.v + cv[dire];
	test.pos.h = current.pos.h + ch[dire];

	if(!tgmap->IsValid(test.pos))
		return false;

	for (int i=0; i<closetable.size(); i++)
	{
		if (test.pos == closetable[i].pos)
			return false;
	}

	test.dire = dire;
	test.g = Getg();
	test.h += len[dire];
	test.f = test.g + test.h;

	return true;
}

bool CSearch::FindPath()
{
	while(opentable.size()>0)
	{
		current = opentable.top();
		opentable.pop();
		closetable.push_back(current);

		for (dire=0; dire<4; dire++)
		{
			test = current;
			if (TestMove())
			{
				opentable.push(test);
				if (test.pos == dest)
				{
					return true;
				}
			}
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////////////

int main()
{
	CMap map;

	CSearch cs(&map);
	cs.SetPos(CPos(1,1),CPos(8,8));
	cs.FindPath();

	return 0;
}