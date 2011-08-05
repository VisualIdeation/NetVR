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

#ifndef XML_DOCUMENT_DISMANTLER_H_INCLUDED
#define XML_DOCUMENT_DISMANTLER_H_INCLUDED

// Base classes
#include "XML/istream.h"

// Classes used as a value
#include "XML/common.h"
#include <string>


namespace XML {

class DocumentDismantler : public istream
{
	public:
	DocumentDismantler(std::istream& stdInStream);
	virtual ~DocumentDismantler();

	// Advance to an element or the end of the current element
	DocumentDismantler& findElement();
	// Skip over an element
	DocumentDismantler& skipElement();

	// Advance to and enter an element (must have an element to enter!)
	DocumentDismantler& enterElement() throw(Failure);
	// Exit the current element and return to parent
	DocumentDismantler& exitElement() throw(Failure);

	// Return element's name or attributes (must be at an element!)
	const std::string& viewElementName() throw(Failure);
	const AttributeMap& viewElementAttributes() throw(Failure);
	const std::string viewElementAttribute(const std::string& name) throw(Failure);
	// Get all character data before a break in the character data
	std::string getCData();
	// Get a piece of character data before a break in the character data
	std::string getSegmentedCData();
	// Enter the element, return the first run of character data, exit
	// the element, and return the character data.
	std::string getTaggedString();

	bool atCData();
	bool atElementStart();
	bool atElementEnd();
	bool atDocumentEnd();

	static const std::string empty;
};

} // namespace XML

#endif
