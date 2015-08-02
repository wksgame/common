#ifndef KISS_XML_PARSER_H
#define KISS_XML_PARSER_H

#include<tinyxml/tinyxml.h>
#include<tinyxml/tinystr.h>

namespace kiss
{
	class XMLParser
	{
	public:
		XMLParser(const char* filename);
		virtual ~XMLParser();

		bool Init();
		void Parse();
		
	protected:
		void Parse(TiXmlElement* element);

	protected:
		virtual void ParseElement(TiXmlElement* element);

	private:
		TiXmlDocument* doc;
	};
}//namespace kiss

#endif//KISS_XML_PARSER_H