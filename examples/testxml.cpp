#ifndef PEOPLE_XML_H
#define PEOPLE_XML_H

#include"game/Player.h"
#include"xml/XMLParser.h"
#include<iostream>

using namespace std;
using namespace kiss;
using namespace game;

unordered_map<int,int> userandpwd;

class PeopleXML:public XMLParser
{
public:

	PeopleXML(const std::string& filename):XMLParser(filename.c_str())
	{
	}

protected:

	void ParsePerson(TiXmlElement* element)
	{
		string name;
		int id=0;
		int hp=0; 
		int damage=0; 
		int defend=0; 
		int hit=0;
		int dodge=0; 
		int critical=0;

		auto attr = element->FirstAttribute();
		while (attr)
		{
			if(strcmp(attr->Name(),"name")==0)
				name=attr->Value();
			else if(strcmp(attr->Name(),"id")==0)
				id=attr->IntValue();
			else if(strcmp(attr->Name(),"hp")==0)
				hp=attr->IntValue();
			else if(strcmp(attr->Name(),"damage")==0)
				damage=attr->IntValue();
			else if(strcmp(attr->Name(),"defend")==0)
				defend=attr->IntValue();
			else if(strcmp(attr->Name(),"hit")==0)
				hit=attr->IntValue();
			else if(strcmp(attr->Name(),"dodge")==0)
				dodge=attr->IntValue();
			else if(strcmp(attr->Name(),"critical")==0)
				critical=attr->IntValue();

			attr = attr->Next();
		}
		
		cout<<"id:"<<id<<" name:"<<name<<" hp:"<<hp<<" damage:"<<damage<<" defend:"<<defend<<" hit:"<<hit<<" dodge:"<<dodge<<" critical:"<<critical<<endl;
		
		auto p= new Player(name, id, hp, damage, defend, hit, dodge, critical);
		
		g_allPlayers[id] = p;

// 		auto child = element->FirstChildElement();
// 		if(child)
// 			ParseElement(child);
	}

	/*void ParseName(TiXmlElement* element)
	{
		cout<<element->FirstChild()->Value()<<endl;

		auto child = element->FirstChildElement();
		if(child)
			ParseElement(child);
	}

	void ParseAge(TiXmlElement* element)
	{
		cout<<element->FirstChild()->Value()<<endl;

		auto child = element->FirstChildElement();
		if(child)
			ParseElement(child);
	}*/

	void ParsePwd(TiXmlElement* element)
	{
		int id=0;
		int pwd=0;

		auto attr = element->FirstAttribute();
		while (attr)
		{
			if(strcmp(attr->Name(),"id")==0)
				id=attr->IntValue();
			else if(strcmp(attr->Name(),"pwd")==0)
				pwd=attr->IntValue();


			attr = attr->Next();
		}
		
		cout<<"id:"<<id<<" pwd:"<<pwd<<endl;
		userandpwd[id]=pwd;
		
// 		auto child = element->FirstChildElement();
// 		if(child)
// 			ParseElement(child);
	}

	void ParseElement(TiXmlElement* element)override
	{
//		while (element)
		{
			if(strcmp(element->Value(), "person")==0)
			{
				ParsePerson(element);
			}
			/*else if(strcmp(element->Value(), "name")==0)
			{
				ParseName(element);
			}
			else if(strcmp(element->Value(), "age")==0)
			{
				ParseAge(element);
			}*/
			else if(strcmp(element->Value(), "pwd")==0)
			{
				ParsePwd(element);
			}

//			element = element->NextSiblingElement();
		}
	}
};
#endif//PEOPLE_XML_H

int main()
{
	PeopleXML xml("people.xml");
	xml.Init();
	xml.Parse();

	return 0;
}