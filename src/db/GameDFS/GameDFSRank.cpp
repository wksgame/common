//#include"GameDFS.h"
#include<hgl/dfs/dfsClientRank.h>
#include<hgl/ut/XMLParse.h>
#include"GameDFSRank.h"

using namespace hgl;
using namespace hgl::dfs;

namespace showhand
{
	namespace
	{
		// 		money =1;
		// 		level=2;
		// 		achievement=3;
		// 		bet_success=4;
		// 		recharge=5;
		const char *dfs_rank_node_name[]=
		{
			"",

			"Wealth",
			"Level",
			"Achivement",
			"BetSuccess",
			"Recharge",

			""
		};

		dfsClientRank	**dfs_client_rank_list=nullptr;
	}//namespace

	dfsClientRank *GetDFSClientRank(GameDFSRankNode index)
	{
		if(index<=dfsNone||index>=dfsRankEnd)
			return(nullptr);

		return dfs_client_rank_list[index];
	}

	void InitDFSRank()
	{
		dfs_client_rank_list=new dfsClientRank *[dfsRankEnd];

		for(int i=0;i<dfsRankEnd;i++)
			dfs_client_rank_list[i]=nullptr;
	}

	void LoadXMLNode_DFS_Rank(const char **atts)
	{
		UTF8String name;
		UTF8String ip;
		uint port;
		UTF8String info;

		XML_START_PARSE(atts)
			xml_parse_string_u8(name)
			xml_parse_string_u8(ip)
			xml_parse_int(port)
			xml_parse_string_u8(info)
		XML_END_PARSE()

		const int index=string_to_enum<char>(dfs_rank_node_name,name.c_str());

		if(index<=dfsNone||index>=dfsRankEnd)
			return;

		if(dfs_client_rank_list[index])
			return;

		dfs_client_rank_list[index]=new dfsClientRank(name);

		dfs_client_rank_list[index]->Init(ip,port,"ShowhandServer");
	}
}//name showhand
