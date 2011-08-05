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

#ifndef CLASS_H_INCLUDED
#define CLASS_H_INCLUDED


#include "XML/Document"
#include <string>

/*************
 * Student
 */

class Student : public Netitor::Node
{
	public:
	std::string first;
	std::string last;
	std::string dept;
	const static std::string typeName;

	Student() : Netitor::Node() {} // Prototype
	Student(std::string first, std::string last, std::string dept)
		: Netitor::Node(), first(first), last(last), dept(dept)
	{ }

	// Return the name of this class
	virtual const std::string& getTypeName() const
	{
		return Student::typeName;
	}

	protected:

	// Return pointer to new Student
	Student* create() const
	{
		return new Student();
	}

	// Load from XML data
	Student& initFromXML(XML::DocumentDismantler& xmlDocument,
						 util::IDToPointer& idToPtr)
	{
		// Scan through elements looking for Student specific ones
		for (xmlDocument.findElement();
				xmlDocument.atElementStart(); xmlDocument.findElement())
		{
			if (xmlDocument.viewElementName() == "first") {
				first = xmlDocument.getTaggedString();
			} else if (xmlDocument.viewElementName() == "last") {
				last = xmlDocument.getTaggedString();
			} else if (xmlDocument.viewElementName() == "dept") {
				dept = xmlDocument.getTaggedString();
			} else {
				// An unknown element.  Skip it.
				xmlDocument.skipElement();
			}
		}
		return *this;
	}

	// Save Student specific stuff as XML data
	const Student& outputXML(XML::DocumentBuilder& xmlDocument,
							 util::PointerToID& ptrToID) const
	{
		xmlDocument.insertTaggedString("first", first);
		xmlDocument.insertTaggedString("last", last);
		xmlDocument.insertTaggedString("dept", dept);
		return *this;
	}
};

//const std::string Student::typeName = "student"; // the name for this class


/*************
 * Met
 */

class Met : public Netitor::UndirectedEdge
{
	public:
	int times;

	Met() : Netitor::UndirectedEdge() {} // Prototype
	Met(Netitor::Node* node1, Netitor::Node* node2)
		: Netitor::UndirectedEdge(node1, node2)
	{ }

	virtual const std::string& getTypeName() const
	{
		return Met::typeName;
	}

	// Return pointer to new Met
	Met* create() const
	{
		return new Met();
	}

	const static std::string typeName;
};

//const std::string Met::typeName = "met";

#endif
