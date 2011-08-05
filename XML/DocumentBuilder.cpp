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

/**************************
 * DocumentBuilder class
 */

#include "XML/DocumentBuilder.h"

#include <ostream>


namespace XML {


DocumentBuilder::DocumentBuilder(std::ostream& stdOutStream)
	: ostream(stdOutStream), justBeganElement(false)
{
}

DocumentBuilder::~DocumentBuilder()
{
}

DocumentBuilder& DocumentBuilder::beginElement(const std::string& name,
                                               const AttributeMap& attributes)
{
	if (good())
	{
		if (justBeganElement)
			writeCData("\n");
		for (unsigned l = getDepth(); l > 0; --l)
			writeCData("\t");
		writeElementStart(name, attributes);
		justBeganElement = true;
	}
	return *this;
}

DocumentBuilder& DocumentBuilder::insertString(const std::string& str)
{
	if (good())
	{
		if (justBeganElement) {
			writeCData("\n");
			justBeganElement = false;
		}
		for (unsigned l = getDepth(); l > 0; --l)
			writeCData("\t");
		writeCData(str);
		writeCData("\n");
	}
	return *this;
}

DocumentBuilder& DocumentBuilder::endElement()
{
	if (good() && getDepth() > 0)
	{
		if (!justBeganElement)
			for (unsigned l = getDepth(); l > 1; --l)
				writeCData("\t");
		writeElementEnd();
		writeWhitespace("\n");
		justBeganElement = false;
	}
	return *this;
}

DocumentBuilder& DocumentBuilder::insertEmptyElement(const std::string& name,
													 const AttributeMap& attributes)
{
	if (good())
	{
		if (justBeganElement) {
			writeCData("\n");
			justBeganElement = false;
		}
		for (unsigned l = getDepth(); l > 0; --l)
			writeCData("\t");
		writeElementStart(name, attributes);
		writeElementEnd();
		writeCData("\n");
	}
	return *this;
}

DocumentBuilder& DocumentBuilder::insertTaggedString(const std::string& name,
													 const std::string& str,
													 const AttributeMap& attributes)
{
	if (good())
	{
		if (justBeganElement) {
			writeCData("\n");
			justBeganElement = false;
		}
		for (unsigned l = getDepth(); l > 0; --l)
			writeCData("\t");
		writeElementStart(name, attributes);
		writeCData(str);
		writeElementEnd();
		writeCData("\n");
	}		
	return *this;
}


} // namespace XML
