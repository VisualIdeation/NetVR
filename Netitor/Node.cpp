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

/**************
 * Node class
 */

#include "Node.h"

#include "Netitor/utilities.h"
#include "Netitor/Network.h"
#include "Netitor/Edge.h"
#include "Netitor/DirectedEdge.h"
#include "Netitor/NetworkMonitor.h"
#include "Netitor/NodeMonitor.h"
#include "Netitor/InteractionMonitor.h"
#include "XML/ostream.h"
#include "XML/Document"

#ifndef NDEBUG
#include <iostream>
#endif


namespace Netitor {

const std::map<NetworkMonitor*,InteractionMonitor*> Node::emptyInteractionMonitors =
		std::map<NetworkMonitor*,InteractionMonitor*>();

const std::string Node::typeName = "node";

Node::Node()
  : IDObject(), degree_(0)
{
}

Node::~Node()
{
	while (!inEdges.empty())
		(*(inEdges.begin()))->detach();
	while (!outEdges.empty())
		(*(outEdges.begin()))->detach();
	while (!interactionMonitors.empty())
		interactionMonitors.begin()->second.begin()->second->detach();
	while (!networks.empty())
		(*(networks.begin()))->removeNode(this);
}

int Node::outDegree() const
{
	return outEdges.size();
}

int Node::inDegree() const
{
	return inEdges.size();
}

int Node::degree() const
{
	return degree_;
}

bool Node::isIsolated() const
{
	return (degree_ == 0);
}

const std::multiset<Edge*>& Node::getInEdges() const
{
	return inEdges;
}

const std::multiset<Edge*>& Node::getOutEdges() const
{
	return outEdges;
}

EdgeSet Node::getIncidentEdges() const
{
	EdgeSet incidentEdges;
	incidentEdges.insert(inEdges.begin(), inEdges.end());
	incidentEdges.insert(outEdges.begin(), outEdges.end());
	return incidentEdges;
}

Edge* Node::findEdgeIntoNetwork(Network* network) const
{
	for (EdgeSet::iterator pos = outEdges.begin();
			pos != outEdges.end(); ++pos)
	{
		if ((*pos)->isIncomingIntoNetwork(network))
			return *pos;
	}
	return NULL;
}

Edge* Node::findEdgeFromNetwork(Network* network) const
{
	for (EdgeSet::iterator pos = inEdges.begin();
			pos != inEdges.end(); ++pos)
	{
		if ((*pos)->isOutgoingFromNetwork(network))
			return *pos;
	}
	return NULL;
}

NodeSet Node::getRelatedNodes() const
{
	NodeSet relatedNodes;
	for (EdgeSet::iterator pos = inEdges.begin();
			pos != inEdges.end(); ++pos)
	{
		relatedNodes.insert((*pos)->getOtherNode(this));
	}
	for (EdgeSet::iterator pos = outEdges.begin();
			pos != outEdges.end(); ++pos)
	{
		relatedNodes.insert((*pos)->getOtherNode(this));
	}
	return relatedNodes;
}

EdgeSet Node::getEdgesRelatingNode(Node* node) const
{
	EdgeSet edges;
	for (EdgeSet::iterator pos = inEdges.begin();
			pos != inEdges.end(); ++pos)
	{
		if ((*pos)->involvesNode(node))
			edges.insert(*pos);
	}
	for (EdgeSet::iterator pos = outEdges.begin();
			pos != outEdges.end(); ++pos)
	{
		if ((*pos)->involvesNode(node))
			edges.insert(*pos);
	}
	return edges;
}

const NetworkSet& Node::getNetworks() const
{
	return networks;
}

bool Node::isInNetwork(const Network* network) const
{
	return (networks.count(const_cast<Network*>(network)) != 0); // WTF??
}

void Node::setLabel(const std::string& newLabel)
{
	label = newLabel;
}

const std::string& Node::getLabel() const
{
	return label;
}

NodeMonitor* Node::getMonitor(const NetworkMonitor* networkMonitor) const
{
	std::map<NetworkMonitor*,NodeMonitor*>::const_iterator pos =
			monitors.find(const_cast<NetworkMonitor*>(networkMonitor)); // WTF??
	if (pos != monitors.end()) {
		return pos->second;
	} else {
		return NULL;
	}
}

const std::map<Node*,std::map<NetworkMonitor*,InteractionMonitor*> > Node::getAllInteractionMonitors() const
{
	return interactionMonitors;
}

const std::map<NetworkMonitor*,InteractionMonitor*>& Node::getInteractionMonitors(const Node* node) const
{
	std::map<Node*,std::map<NetworkMonitor*,InteractionMonitor*> >::const_iterator pos =
			interactionMonitors.find(const_cast<Node*>(node));
	if (pos != interactionMonitors.end()) {
		return pos->second;
	} else {
		return Node::emptyInteractionMonitors;
	}
}

InteractionMonitor* Node::getInteractionMonitor(const Node* node, const NetworkMonitor* networkMonitor) const
{
	std::map<Node*,std::map<NetworkMonitor*,InteractionMonitor*> >::const_iterator pos =
		interactionMonitors.find(const_cast<Node*>(node));
	if (pos != interactionMonitors.end()) {
		std::map<NetworkMonitor*,InteractionMonitor*>::const_iterator pos2 =
				pos->second.find(const_cast<NetworkMonitor*>(networkMonitor));
		if (pos2 != pos->second.end()) {
			return pos2->second;
		} else {
			return NULL;
		}
	} else {
		return NULL;
	}
}

const std::string& Node::getTypeName() const
{
	return Node::typeName;
}

/*
 * Instructions from edges
 */

void Node::attachEdge(Edge* edge, Node* node2)
{
	if(edge->comesFromNode(this))
		outEdges.insert(edge);
	if(edge->goesToNode(this))
		inEdges.insert(edge);
	++degree_;
}

void Node::detachEdge(Edge* edge, Node* node2)
{
	inEdges.erase(edge);
	outEdges.erase(edge);
	--degree_;
}

/*
 * Instructions from self
 */

void Node::notifyMonitorsOfChangedProperty()
{
	for (std::map<NetworkMonitor*,NodeMonitor*>::iterator pos = monitors.begin();
			pos != monitors.end(); ++pos)
	{
		pos->second->changedProperty();
	}
}

/********************************
 * XML input / output interface
 */

Node* Node::create() const
{
	return new Node();
}

Node& Node::initFromXML(XML::DocumentDismantler& xmlDocument,
						util::IDToPointer& idToPtr)
{
	return *this;
}

const Node& Node::outputXML(XML::DocumentBuilder& xmlDocument,
							util::PointerToID& ptrToID) const
{
	return *this;
}

Node& Node::enterRootElement(XML::DocumentDismantler& xmlDocument,
							 util::IDToPointer& idToPtr)
{
	unsigned id = util::fromString<unsigned>(xmlDocument.viewElementAttribute("id"));
	idToPtr[id] = this;
	xmlDocument.enterElement();
	return *this;
}

const Node& Node::startRootElement(XML::DocumentBuilder& xmlDocument,
								   util::PointerToID& ptrToID) const
{
	XML::AttributeMap attributes;
	attributes["id"] = util::toString(ptrToID[this]);
	if (!label.empty()) {
		attributes["label"] = label;
	}
	xmlDocument.beginElement(getTypeName(), attributes);
	return *this;
}

} // namespace Netitor
