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

#ifndef XML_DOCUMENT_BUILDER_INCLUDED
#define XML_DOCUMENT_BUILDER_INCLUDED

// Base classes
#include "XML/ostream.h"

// Classes used as values
#include "XML/common.h"


namespace XML {


class DocumentBuilder : public ostream
{
	public:
	DocumentBuilder(std::ostream& stdOutStream);
	virtual ~DocumentBuilder();

	DocumentBuilder& beginElement(const std::string& name,
		                          const AttributeMap& attributes = AttributeMap());
	DocumentBuilder& insertString(const std::string& str);
	DocumentBuilder& endElement();
	DocumentBuilder& insertEmptyElement(const std::string& name,
		                                const AttributeMap& attributes = AttributeMap());
	DocumentBuilder& insertTaggedString(const std::string& name,
		                                const std::string& str,
		                                const AttributeMap& attributes = AttributeMap());
	private:
	bool justBeganElement;
};


} // namespace XML

#endif
