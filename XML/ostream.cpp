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

/******************
 * ostream class
 */

#include "XML/ostream.h"


namespace XML {


ostream::ostream(std::ostream& stdOutStream)
	: stdOutStream(stdOutStream), openTag(false), wroteRoot(false)
{
}

ostream::~ostream()
{
}

ostream& ostream::reset()
{
	ancestors.clear();
	openTag = false;
	wroteRoot = false;
	stdOutStream.clear();
	return *this;
}


ostream& ostream::put(const Token* xmlToken)
{
	switch (xmlToken->type) {
		case Token::ELEMENT_START:
			writeElementStart(static_cast<const ElementStartToken*>(xmlToken)->name,
			                  static_cast<const ElementStartToken*>(xmlToken)->attributes);
			break;
		case Token::CDATA:
			writeCData(static_cast<const CDataToken*>(xmlToken)->data);
			break;
		case Token::ELEMENT_END:
			writeElementEnd();
			break;
		default:
			throw(Failure("Malformed XML token"));
			break;
	}
	return *this;
}


bool ostream::good()
{
	return stdOutStream.good();
}


/*
 * Writing Primatives
 */

ostream& ostream::writeElementStart(const std::string& name, const AttributeMap& attributes)
																			  throw(Failure)
{
	if (!good()) {
		throw(Failure("Cannot write to stream"));
	} else if (wroteRoot) {
		throw(Failure("Cannot write another root element"));
	}
	if (openTag)
		stdOutStream << ">";
	stdOutStream << "<" << name;
	for (AttributeMap::const_iterator pos = attributes.begin();
			pos != attributes.end(); ++pos)
	{
		stdOutStream << " " << pos->first << "=\"" << pos->second << "\"";
	}
	ancestors.push_back(name);
	openTag = true;
	return *this;
}

ostream& ostream::writeCData(const std::string& cData) throw(Failure)
{
	if (!good()) {
		throw(Failure("Cannot write to stream"));
	} else if (outsideRoot()) {
		throw(Failure("Cannot write character data outside of root element"));
	}
	if (openTag) {
		stdOutStream << ">";
		openTag = false;
	}
	stdOutStream << cData;
	return *this;
}

ostream& ostream::writeElementEnd() throw(Failure)
{
	if (!good()) {
		throw(Failure("Cannot write to stream"));
	} else if (outsideRoot()) {
		throw(Failure("No element to end"));
	}
	if (openTag) {
		stdOutStream << "/>";
		openTag = false;
	} else {
		stdOutStream << "</" << ancestors.back() << ">";
	}
	ancestors.pop_back();
	if (ancestors.empty()) {
		wroteRoot = true;
	}
	return *this;
}

ostream& ostream::writeWhitespace(const std::string& whitespace) throw(Failure)
{
	if (!good()) {
		throw(Failure("Cannot write to stream"));
	}
	if (openTag) {
		stdOutStream << ">";
		openTag = false;
	}
	for (std::string::size_type pos = whitespace.find_first_of(" \t\n");
			pos != std::string::npos; pos = whitespace.find_first_of(" \t\n", pos+1))
	{
		stdOutStream << whitespace[pos];
	}
	return *this;
}


/*
 * Accessors
 */
 
unsigned ostream::getDepth()
{
	return ancestors.size();
}

std::string ostream::getParentName()
{
	if (!ancestors.empty()) {
		return ancestors.back();
	} else {
		return "";
	}
}

bool ostream::outsideRoot()
{
	return ancestors.empty();
}


} // namespace XML
