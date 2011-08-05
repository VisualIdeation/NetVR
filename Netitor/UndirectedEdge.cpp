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

/****************************
 * UndirectedEdge class
 */

#include "Netitor/UndirectedEdge.h"

#include "Netitor/utilities.h"
#include "Netitor/Network.h"
#include "Netitor/Node.h"
#include "XML/Document"


namespace Netitor {


const std::string UndirectedEdge::typeName = "edge";

UndirectedEdge::UndirectedEdge()
	: Edge()
{
}

UndirectedEdge::UndirectedEdge(Node* const node1, Node* const node2)
	: Edge(node1, node2)
{
}

UndirectedEdge::~UndirectedEdge()
{
	detach();
}

bool UndirectedEdge::isDirected() const
{
	return false;
}

/*
 * Class info
 */

const std::string& UndirectedEdge::getTypeName() const
{
	return UndirectedEdge::typeName;
}


/********************************
 * XML input / output interface
 */

Edge* UndirectedEdge::create() const
{
	return new UndirectedEdge();
}

Edge& UndirectedEdge::initFromXML(XML::DocumentDismantler& xmlDocument,
						util::IDToPointer& idToPtr)
{
	return *this;
}

const Edge& UndirectedEdge::outputXML(XML::DocumentBuilder& xmlDocument,
							util::PointerToID& ptrToID) const
{
	return *this;
}

/*
 * Protected
 */
 
Edge& UndirectedEdge::enterRootElement(XML::DocumentDismantler& xmlDocument,
							 util::IDToPointer& idToPtr) throw(Failure)
{
	if (xmlDocument.viewElementAttribute("from") == "" ||
	    xmlDocument.viewElementAttribute("to") == "")
	{
		throw(Failure("Connections not given for edge"));
	}
	unsigned id1 = util::fromString<unsigned>(xmlDocument.viewElementAttribute("from"));
	unsigned id2 = util::fromString<unsigned>(xmlDocument.viewElementAttribute("to"));
	setLabel(xmlDocument.viewElementAttribute("label"));
	attach(static_cast<Node*>(idToPtr[id1]),
		   static_cast<Node*>(idToPtr[id2]));
	xmlDocument.enterElement();
	return *this;
}

const Edge& UndirectedEdge::startRootElement(XML::DocumentBuilder& xmlDocument,
											 util::PointerToID& ptrToID) const
{
	XML::AttributeMap attributes;
	attributes["directed"] = "false";
	attributes["from"] = util::toString(ptrToID[getNodes().first]);
	attributes["to"] = util::toString(ptrToID[getNodes().second]);
	if (!getLabel().empty()) {
		attributes["label"] = getLabel();
	}
	xmlDocument.beginElement(getTypeName(), attributes);
	return *this;
}


} // namespace Netitor
