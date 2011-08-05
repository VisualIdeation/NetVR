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

/************************
 * Directed Edge class
 */

#include "Netitor/DirectedEdge.h"

#include "Netitor/utilities.h"
#include "Netitor/Network.h"
#include "Netitor/Node.h"
#include "XML/Document"


namespace Netitor {

const std::string DirectedEdge::typeName = "edge";

DirectedEdge::DirectedEdge()
	: Edge()
{
}

DirectedEdge::DirectedEdge(Node* const theFromNode, Node* const theToNode)
	: Edge(theFromNode, theToNode)
{
}

DirectedEdge::~DirectedEdge()
{
	detach();
}

Node* DirectedEdge::getFromNode() const
{
	return getNodes().first;
}

Node* DirectedEdge::getToNode() const
{
	return getNodes().second;
}

bool DirectedEdge::isDirected() const
{
	return true;
}

/*
 * Class info
 */
 
const std::string& DirectedEdge::getTypeName() const
{
	return DirectedEdge::typeName;
}


/********************************
 * XML input / output interface
 */
 
DirectedEdge* DirectedEdge::create() const
{
	return new DirectedEdge();
}

DirectedEdge& DirectedEdge::initFromXML(XML::DocumentDismantler& xmlDocument,
										util::IDToPointer& idToPtr)
{
	return *this;
}

const DirectedEdge& DirectedEdge::outputXML(XML::DocumentBuilder& xmlDocument,
											util::PointerToID& ptrToID) const
{
	return *this;
}

/*
 * Protected
 */

DirectedEdge& DirectedEdge::enterRootElement(XML::DocumentDismantler& xmlDocument,
                                             util::IDToPointer& idToPtr) throw(Failure)
{
	if (xmlDocument.viewElementAttribute("from") == "" ||
	    xmlDocument.viewElementAttribute("to") == "")
	{
		throw(Failure("Connections not given for edge"));
	}
	unsigned fromID = util::fromString<unsigned>(xmlDocument.viewElementAttribute("from"));
	unsigned toID = util::fromString<unsigned>(xmlDocument.viewElementAttribute("to"));
	setLabel(xmlDocument.viewElementAttribute("label"));
	attach(static_cast<Node*>(idToPtr[fromID]),
		   static_cast<Node*>(idToPtr[toID]));
	xmlDocument.enterElement();
	return *this;
}

const DirectedEdge& DirectedEdge::startRootElement(XML::DocumentBuilder& xmlDocument,
												   util::PointerToID& ptrToID) const
{
	XML::AttributeMap attributes;
	attributes["directed"] = "true";
	attributes["from"] = util::toString(ptrToID[getFromNode()]);
	attributes["to"] = util::toString(ptrToID[getToNode()]);
	if (!getLabel().empty()) {
		attributes["label"] = getLabel();
	}
	xmlDocument.beginElement(getTypeName(), attributes);
	return *this;
}

}
