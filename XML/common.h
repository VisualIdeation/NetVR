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

#ifndef XML_COMMON_H_INCLUDED
#define XML_COMMON_H_INCLUDED

#include <exception>
#include <string>
#include <list>
#include <map>

namespace XML {

typedef std::map<std::string,std::string> AttributeMap;

typedef std::list<std::pair<std::string,std::string> > AttributeList;

class Token
{
	public:
	enum { ELEMENT_START, CDATA, ELEMENT_END, ERROR, END };

	public:
	Token(unsigned type)
		: type(type)
	{ }
	virtual ~Token() {}

	const unsigned type;
};

class ElementStartToken : public Token
{
	public:
	ElementStartToken(const std::string& name, const AttributeMap& attributes = AttributeMap())
		: Token(Token::ELEMENT_START), name(name), attributes(attributes)
	{ }

	public:
	std::string name;
	AttributeMap attributes;
};

class CDataToken : public Token
{
	public:
	CDataToken(const std::string& data)
		: Token(Token::CDATA), data(data)
	{ }

	public:
	std::string data;
};

class ElementEndToken : public Token
{
	public:
	ElementEndToken()
		: Token(Token::ELEMENT_END)
	{ }
};

class ErrorToken : public Token
{
	public:
	ErrorToken(unsigned code, std::string detail)
		: Token(Token::ERROR), code(code), detail(detail)
	{ }
	
	public:
	unsigned code;
	std::string detail;
};

class EndToken : public Token
{
	public:
	EndToken()
		: Token(Token::END)
	{ }
};

class Failure : std::exception
{
	public:
	Failure(const std::string &detail) : Detail(detail) { }
	~Failure() throw () { }
	const char *what() const throw() { return Detail.c_str(); }
	protected:
	std::string Detail;	
};

} // namespace XML

#endif
