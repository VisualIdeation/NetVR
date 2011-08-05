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

/***********************************************
 * Network class, XML input / output interface
 */

#include "Netitor/Network.h"

#include "Netitor/utilities.h"
#include "Netitor/Node.h"
#include "Netitor/GenericNode.h"
#include "Netitor/Edge.h"
#include "Netitor/DirectedEdge.h"
#include "Netitor/UndirectedEdge.h"
#include "XML/Document"

#include <map>
#include <sstream>


namespace Netitor {

Network::Network(std::istream& xmlSource) throw(Failure)
	: IDObject()
{
    load(xmlSource);
}

void Network::load(std::istream& xmlSource) throw(Failure)
{
	XML::DocumentDismantler xmlDocument(xmlSource);
	util::IDToPointer idToPtr;
	
	xmlDocument.findElement();
	if (xmlDocument.viewElementName() != "network")
		throw(Failure("Not a Netitor XML document"));

	xmlDocument.enterElement();  // Enter document root ("network")

	for (xmlDocument.findElement();
			xmlDocument.atElementStart(); xmlDocument.findElement())
	{
		if (xmlDocument.viewElementName() == "nodes")
		{
			xmlDocument.enterElement(); // enter "nodes"
			for (xmlDocument.findElement();
					xmlDocument.atElementStart(); xmlDocument.findElement())
			{
				std::map<std::string,std::pair<unsigned,const Node*> >::iterator
					pos(nodePrototypeRegister.find(xmlDocument.viewElementName()));
				Node* node;
				if (pos != nodePrototypeRegister.end()) {
					node = pos->second.second->create();
				} else {
				  std::string typeName(xmlDocument.viewElementAttribute("type"));
				  if (typeName == "")
				    typeName = "node";
				  node = new GenericNode(typeName);
				  static_cast<GenericNode*>(node)->setAttributeMap(xmlDocument.viewElementAttributes());
				}
				node->enterRootElement(xmlDocument, idToPtr);
				node->initFromXML(xmlDocument, idToPtr);
				xmlDocument.exitElement();
				addNode(node);
			}
			xmlDocument.exitElement(); // exit "nodes"
		}
		else if (xmlDocument.viewElementName() == "edges")
		{
			xmlDocument.enterElement(); // enter "edges"
			for (xmlDocument.findElement();
					xmlDocument.atElementStart(); xmlDocument.findElement())
			{
				std::map<std::string,std::pair<unsigned,const Edge*> >::iterator
					pos(edgePrototypeRegister.find(xmlDocument.viewElementName()));
				Edge* edge;
				if (pos != edgePrototypeRegister.end()) {
					edge = pos->second.second->create();
				} else {
					if (util::fromString<bool>(xmlDocument.viewElementAttribute("directed")))
						edge = new DirectedEdge();
					else
						edge = new UndirectedEdge();
				}
				edge->enterRootElement(xmlDocument, idToPtr);
				edge->initFromXML(xmlDocument, idToPtr);
				xmlDocument.exitElement();
			}
			xmlDocument.exitElement(); // exit "edges"
		}
		else
		{
			xmlDocument.skipElement(); // skip unrecognized element
		}
	}

	xmlDocument.exitElement(); // exit document
}

std::ostream& Network::outputXMLDocument(std::ostream& destination) const
{
	XML::DocumentBuilder xmlDocument(destination);
	util::PointerToID ptrToID;

	startRootElement(xmlDocument);
	outputNodesAndEdges(xmlDocument, ptrToID);
	xmlDocument.endElement();

	return destination;
}

/*
 * Protected
 */

const Network& Network::startRootElement(XML::DocumentBuilder& xmlDocument) const
{
	xmlDocument.beginElement("network");
	return *this;
}

const Network& Network::outputNodesAndEdges(XML::DocumentBuilder& xmlDocument,
											util::PointerToID& ptrToID) const
{
	xmlDocument.beginElement("nodes");
	for (NodeSet::iterator pos = nodes.begin(); pos != nodes.end(); ++pos)
	{
		(*pos)->startRootElement(xmlDocument, ptrToID);
		(*pos)->outputXML(xmlDocument, ptrToID);
		xmlDocument.endElement();
	}
	xmlDocument.endElement();
	
	xmlDocument.beginElement("edges");
	EdgeSet interiorEdges = getInteriorEdges();
	for (EdgeSet::iterator pos = interiorEdges.begin();
			pos != interiorEdges.end(); ++pos)
	{
		(*pos)->startRootElement(xmlDocument, ptrToID);
		(*pos)->outputXML(xmlDocument, ptrToID);
		xmlDocument.endElement();
	}
	xmlDocument.endElement();
	return *this;
}

} // namespace Netitor
