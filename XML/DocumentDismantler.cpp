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

/*****************************
 * DocumentDismantler class 
 */

#include "XML/DocumentDismantler.h"

#include <istream>



namespace XML {


const std::string DocumentDismantler::empty = "";

DocumentDismantler::DocumentDismantler(std::istream& stdInStream)
	: istream(stdInStream)
{
}

DocumentDismantler::~DocumentDismantler()
{
}


DocumentDismantler& DocumentDismantler::findElement()
{
	while (!atElementStart() && !atElementEnd()) {
		ignore();
	}
	return *this;
}

DocumentDismantler& DocumentDismantler::skipElement()
{
	findElement();
	if (atElementStart()) {
		ignore();
		exitElement();
	}
	return *this;
}

DocumentDismantler& DocumentDismantler::enterElement() throw(Failure)
{
	findElement();
	if (!good()) {
		throw Failure("Stream is not readable");
	} else if (!atElementStart()) {
		throw Failure("No element to enter");
	}
	ignore();
	return *this;
}

DocumentDismantler& DocumentDismantler::exitElement() throw(Failure)
{
	unsigned depth = 1;
	while (good() && depth > 0) {
		switch (peek()->type) {
			case Token::ELEMENT_START:
				++depth;
				break;
			case Token::ELEMENT_END:
				--depth;
				break;
			default:
				break;
		}
		ignore();
	}
	if (!good() && depth > 0) {
		throw Failure("Stream is not readable");
	}
	return *this;
}

const std::string& DocumentDismantler::viewElementName() throw(Failure)
{
	if (!good()) {
		throw Failure("Stream is not readable");
	} else if (!atElementStart()) {
		throw Failure("No element to read");
	}
	return static_cast<const ElementStartToken*>(peek())->name;
}

const AttributeMap& DocumentDismantler::viewElementAttributes() throw(Failure)
{
	if (!good()) {
		throw Failure("Stream is not readable");
	} else if (!atElementStart()) {
		throw Failure("No element to read");
	}
	return static_cast<const ElementStartToken*>(peek())->attributes;
}

const std::string DocumentDismantler::viewElementAttribute(const std::string& name)
																	  throw(Failure)
{
	if (!good()) {
		throw Failure("Stream is not readable");
	} else if (!atElementStart()) {
		throw Failure("No element to read");
	}
	AttributeMap attrs = static_cast<const ElementStartToken*>(peek())->attributes;
	AttributeMap::const_iterator pos = attrs.find(name);
	if (pos == attrs.end()) {
		return DocumentDismantler::empty;
	} else {
		return pos->second;
	}
}

std::string DocumentDismantler::getCData()
{
	std::string str;
	while (atCData()) {
		str += static_cast<const CDataToken*>(peek())->data;
		ignore();
	}
	return str;
}

std::string DocumentDismantler::getSegmentedCData()
{
	std::string str;
	if (atCData()) {
		str = static_cast<const CDataToken*>(peek())->data;
		ignore();
	}
	return str;
}

std::string DocumentDismantler::getTaggedString()
{
	std::string str;
	enterElement();
	str = getCData();
	exitElement();
	return str;
}


bool DocumentDismantler::atCData()
{
	return (peek()->type == Token::CDATA);
}
bool DocumentDismantler::atElementStart()
{
	return (peek()->type == Token::ELEMENT_START);
}
bool DocumentDismantler::atElementEnd()
{
	return (peek()->type == Token::ELEMENT_END);
}
bool DocumentDismantler::atDocumentEnd()
{
	return (peek()->type == Token::END);
}


} // namespace XML
