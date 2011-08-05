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

#ifndef XML_ISTREAM_H_INCLUDED
#define XML_ISTREAM_H_INCLUDED

// Classes and types used as a value
#include "XML/common.h"
#ifdef _DISAMB_EXPAT_H
#include "/usr/local/include/expat.h"
#else
#include <expat.h>
#endif
#include <list>
#include <string>


namespace XML {


class istream
{
	public:
	istream(std::istream& stdInStream);
	virtual ~istream();

	istream& get(const Token*& token);  // pointer ownership transferred to caller
	const Token* peek() const; // pointer still owned by istream
	istream& ignore();
	bool good() const;

	unsigned getDepth();
	std::string getParentName();

	private:
	Token* getNextTokenPtr();
	void populateTokenList();

	private:
	std::istream& stdInStream;
	XML_Parser saxParser;
	char* saxBuffer;
	std::list<std::string> ancestors;
	std::list<const Token*> xmlTokens;
	const EndToken end;

	// Static Members
	private:
	static void XMLCALL startElementHandler (void *userData, const XML_Char *name,
	                                         const XML_Char **atts);
	static void XMLCALL endElementHandler   (void *userData, const XML_Char *name); 
	static void XMLCALL characterDataHandler(void *userData, const XML_Char *str, int length);

	private:
	static const int BUFFER_SIZE = 128;
};


} // namespace XML

#endif
