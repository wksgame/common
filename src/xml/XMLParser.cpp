#include"XMLParser.h"
#include<logger/logger.h>

namespace kiss
{
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
			syslogger.error("file error");
			return false;
		}

		if(doc->Error())
		{
			syslogger.error(doc->ErrorDesc());
			return false;
		}

		return true;
	}

	void XMLParser::Parse()
	{
		TiXmlElement* rootElement = doc->RootElement();
		TiXmlElement* childElement = rootElement->FirstChildElement();
		
		Parse(childElement);
	}
	
	void XMLParser::Parse(TiXmlElement* element)
	{
		while (element)
		{
			ParseElement(element);

			auto child = element->FirstChildElement();
			if(child)
				Parse(child);
		
			element = element->NextSiblingElement();
		}
	}

	void XMLParser::ParseElement(TiXmlElement* element)
	{
	}
}