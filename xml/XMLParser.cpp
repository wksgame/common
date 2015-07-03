#include"XMLParser.h"
#include<iostream>

using namespace std;

//XMLParser::XMLParser(const std::string& filename)
//{
//	doc = new TiXmlDocument(filename.c_str());
//}

XMLParser::XMLParser(const char* filename)
{
	doc = new TiXmlDocument(filename);
}

XMLParser::~XMLParser()
{
	delete doc;
}

bool XMLParser::Init()
{
	if(!doc || !doc->LoadFile())
	{
		cout<<"file error"<<endl;
		return false;
	}

	if(doc->Error())
	{
		cout<<doc->ErrorDesc()<<endl;
		return false;
	}

	return true;
}

void XMLParser::Parse()
{
	auto rootElement = doc->RootElement();
	auto childElement = rootElement->FirstChildElement();
	ParseElement(childElement);
}

void XMLParser::ParseElement(TiXmlElement* element)
{
}