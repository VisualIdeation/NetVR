/*******************************************************************
Copyright (c) 2006-2008 Braden Pellett and Jordan Van Aalsburg

This file is part of NetVR.

NetVR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

NetVR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with NetVR.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************/

/*****************
 * istream class
 */

#include "XML/istream.h"

#include <istream>


namespace XML {


istream::istream(std::istream& stdInStream)
	: stdInStream(stdInStream)
{
	saxParser = XML_ParserCreate(NULL);
	XML_SetUserData(saxParser, this);
	XML_SetElementHandler(saxParser, startElementHandler, endElementHandler);
	XML_SetCharacterDataHandler(saxParser, characterDataHandler);
	saxBuffer = new char[BUFFER_SIZE];
	populateTokenList();
}

istream::~istream()
{
	delete [] saxBuffer;
	XML_ParserFree(saxParser);
	for (int i = xmlTokens.size(); i > 0 ; --i)
	{
		delete xmlTokens.front();
		xmlTokens.pop_front();
	}
}


istream& istream::get(const Token*& token)
{
	populateTokenList();
	if (good()) {
		token = xmlTokens.front();
		xmlTokens.pop_front();
		populateTokenList();
	} else {
		token = new EndToken();
	}
	return *this;
}

const Token* istream::peek() const
{
	if (good()) {
		return xmlTokens.front();
	} else {
		return &end;
	}
}

istream& istream::ignore()
{
	if (good()) {
		const Token* token;
		get(token);
		delete token;
	}
	return *this;
}

bool istream::good() const
{
	return !xmlTokens.empty() && (xmlTokens.front()->type != Token::ERROR);
}

unsigned istream::getDepth()
{
	return ancestors.size();
}

std::string istream::getParentName()
{
	if (!ancestors.empty()) {
		return ancestors.back();
	} else {
		return "";
	}
}

/*
 * Private
 */

void istream::populateTokenList()
{
	while (xmlTokens.empty() && stdInStream.good()) {
		stdInStream.read(saxBuffer, BUFFER_SIZE);
		if(XML_Parse(saxParser, saxBuffer, stdInStream.gcount(),
				!stdInStream.good()) == XML_STATUS_ERROR)
		{
			xmlTokens.push_back(new ErrorToken(XML_GetErrorCode(saxParser),
				XML_ErrorString(XML_GetErrorCode(saxParser))));
		}
	}
}


/*
 * Static Handler Functions
 */

void XMLCALL istream::startElementHandler(void *userData, 
										  const XML_Char *name,
										  const XML_Char **atts)
{
	ElementStartToken* token = new ElementStartToken(name);
	unsigned int index = 0;
	while ( NULL != atts[index] ) {
		token->attributes[atts[index]] = atts[index + 1];
		index += 2;
	}
	static_cast<istream*>(userData)->ancestors.push_back(name);
	static_cast<istream*>(userData)->xmlTokens.push_back(token);
}

void XMLCALL istream::endElementHandler(void *userData, 
									    const XML_Char *name)
{
	static_cast<istream*>(userData)->
		xmlTokens.push_back(new ElementEndToken());
	static_cast<istream*>(userData)->ancestors.pop_back();
}

void XMLCALL istream::characterDataHandler(void *userData, 
										   const XML_Char *str,
										   int length)
{
	static_cast<istream*>(userData)->
		xmlTokens.push_back(new CDataToken(std::string(str, length)));
}


} // namespace XML
