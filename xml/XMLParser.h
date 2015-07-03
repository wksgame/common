#ifndef XML_PARSER_H
#define XML_PARSER_H

//#include<string>
#include"tinyxml/tinyxml.h"
#include"tinyxml/tinystr.h"

class XMLParser
{
public:
	//XMLParser(const std::string& filename);
	XMLParser(const char* filename);
	virtual ~XMLParser();

	bool Init();
	void Parse();

protected:
	virtual void ParseElement(TiXmlElement* element);

private:
	TiXmlDocument* doc;
};


#endif//XML_PARSER_H