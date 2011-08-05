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

/************************************
 * Network class, general functions
 */

#include "Netitor/Network.h"

#include "Netitor/Node.h"
#include "Netitor/Edge.h"
#include "Netitor/NetworkMonitor.h"
#include "Netitor/NodeMonitor.h"
#include "Netitor/InteractionMonitor.h"
#include "Netitor/EdgeMonitor.h"

#include <list>

#include <iostream> // DEBUG


namespace Netitor {

std::map<std::string,std::pair<unsigned,const Node*> > Network::nodePrototypeRegister =
	std::map<std::string,std::pair<unsigned,const Node*> >();
std::map<std::string,std::pair<unsigned,const Edge*> > Network::edgePrototypeRegister =
	std::map<std::string,std::pair<unsigned,const Edge*> >();

Network::Network()
	: IDObject()
{
}

Network::Network(const Network& network)
	: IDObject()
{
	const NodeSet& newNodes(network.getNodes());
	for (NodeSet::iterator pos = newNodes.begin();
			pos != newNodes.end(); ++pos)
	{
		addNode(*pos);
	}
}

Network::~Network()
{
	while (!monitors.empty())
		(*monitors.begin())->detach();
	while (!nodes.empty())
		removeNode(*nodes.begin());
}

void Network::addNode(Node* const node)
{
	if (nodes.insert(node).second) {
		// Inform the node it has been added to this network
		node->networks.insert(this);
		
		// Tell the Node monitor(s) that the node has been added to this Network
		for (std::map<NetworkMonitor*,NodeMonitor*>::iterator monPos = node->monitors.begin();
				monPos != node->monitors.end(); ++monPos)
		{
			monPos->second->addedToNetwork(this);
		}

		// Tell Interaction Monitors associated with the Node about it entering this Network
		const std::map<Node*,std::map<NetworkMonitor*,InteractionMonitor*> >
				allInteractionMonitors(node->getAllInteractionMonitors());
		for (std::map<Node*,std::map<NetworkMonitor*,InteractionMonitor*> >::const_iterator pos =
					allInteractionMonitors.begin();
				pos != allInteractionMonitors.end(); ++pos)
		{
			for(std::map<NetworkMonitor*,InteractionMonitor*>::const_iterator pos2 = pos->second.begin();
					pos2 != pos->second.end(); ++pos2)
			{
				if (pos->first->isInNetwork(this)) {
					pos2->second->becameInteriorInNetwork(this);
				} else {
					pos2->second->becameIncidentToNetwork(this);
				}
			}
		}
		
		// Tell the Network Monitors about the newly added Node
		for (NetworkMonitorSet::iterator monPos = monitors.begin();
				monPos != monitors.end(); ++monPos)
		{
			(*monPos)->addedNode(node);
		}

		// Update edge info
		EdgeSet edges = node->getIncidentEdges();
		for (EdgeSet::iterator edgesPos = edges.begin();
				edgesPos != edges.end(); ++edgesPos)
		{
			bool edgeAdded = false;
			if ((*edgesPos)->isInteriorInNetwork(this)) {
				if ((*edgesPos)->isLoop()) {
					interiorEdges.insert(*edgesPos);
					edgeAdded = true;
				} else {
					incidentEdges.erase(*edgesPos);
					interiorEdges.insert(*edgesPos);
				}
			} else {
				incidentEdges.insert(*edgesPos);
				edgeAdded = true;
			}
			if (edgeAdded) {
				// Tell the Edge's monitors that the edge became incident to this Network
				for (std::map<NetworkMonitor*,EdgeMonitor*>::iterator monPos = (*edgesPos)->monitors.begin();
						monPos != (*edgesPos)->monitors.end(); ++monPos)
				{
					monPos->second->becameIncidentToNetwork(this);
				}
				// Tell the Network monitors that the edge has entered this network
				for (NetworkMonitorSet::iterator monPos = monitors.begin();
						monPos != monitors.end(); ++monPos)
				{
					(*monPos)->addedEdge(*edgesPos);
				}
			} else {
				// Tell the Edge's monitors that the edge became interior in this Network
				for (std::map<NetworkMonitor*,EdgeMonitor*>::iterator monPos = (*edgesPos)->monitors.begin();
						monPos != (*edgesPos)->monitors.end(); ++monPos)
				{
					monPos->second->becameInteriorInNetwork(this);
				}
			}
		}
	}
}

void Network::removeNode(Node* const node)
{
	if (nodes.erase(node) > 0) {
		// Inform the node it has been removed from this network.
		node->networks.erase(this);
		
		// Tell the Node Monitor(s) for this network to detach
		for (NetworkMonitorSet::iterator monPos = monitors.begin();
				monPos != monitors.end(); ++monPos)
		{
			NodeMonitor* nodeMonitor = node->getMonitor(*monPos);
			if (nodeMonitor != NULL)
				nodeMonitor->detach();
		}
		
		// Tell the Interaction Monitors of completely displaced Nodes to detach
		{
			std::list<InteractionMonitor*> toDetach;
			const std::map<Node*,std::map<NetworkMonitor*,InteractionMonitor*> >&
					allInteractionMonitors(node->getAllInteractionMonitors());
			for (std::map<Node*,std::map<NetworkMonitor*,InteractionMonitor*> >::const_iterator pos =
						allInteractionMonitors.begin();
					pos != allInteractionMonitors.end(); ++pos)
			{
				if (!(pos->first->isInNetwork(this))) {
					for(std::map<NetworkMonitor*,InteractionMonitor*>::const_iterator pos2 = pos->second.begin();
							pos2 != pos->second.end(); ++pos2)
					{
						if (monitors.count(pos2->first) > 0) {
							toDetach.push_back(pos2->second);
						}
					}
				}
			}
			for (std::list<InteractionMonitor*>::iterator pos = toDetach.begin();
					pos != toDetach.end(); ++pos)
			{
//std::cout << (*pos) << std::endl;
				(*pos)->detach();
			}
		}
		
		// Tell Interaction Monitors associated with the Node about it leaving this Network
		{
			const std::map<Node*,std::map<NetworkMonitor*,InteractionMonitor*> >&
					allInteractionMonitors(node->getAllInteractionMonitors());
			for (std::map<Node*,std::map<NetworkMonitor*,InteractionMonitor*> >::const_iterator pos =
						allInteractionMonitors.begin();
					pos != allInteractionMonitors.end(); ++pos)
			{
				for(std::map<NetworkMonitor*,InteractionMonitor*>::const_iterator pos2 = pos->second.begin();
						pos2 != pos->second.end(); ++pos2)
				{
//std::cout << "InteractionMonitor: " << pos2->second << std::endl;
					if (pos->first->isInNetwork(this)) {
						pos2->second->becameIncidentToNetwork(this);
					} else {
						pos2->second->removedFromNetwork(this);
					}
				}
			}
		}

		// Tell the Node's monitors that it was removed from this Network
		for (std::map<NetworkMonitor*,NodeMonitor*>::iterator monPos = node->monitors.begin();
				monPos != node->monitors.end(); ++monPos)
		{
			monPos->second->removedFromNetwork(this);
		}

		// Update edge info
		EdgeSet edges = node->getIncidentEdges();
		for (EdgeSet::iterator edgesPos = edges.begin();
				edgesPos != edges.end(); ++edgesPos)
		{
			interiorEdges.erase(*edgesPos);
			if ((*edgesPos)->isIncidentToNetwork(this)) {
				incidentEdges.insert(*edgesPos);
				// Tell the Edge's monitors that the edge became incident to this Network
				for (std::map<NetworkMonitor*,EdgeMonitor*>::iterator monPos = (*edgesPos)->monitors.begin();
						monPos != (*edgesPos)->monitors.end(); ++monPos)
				{
					monPos->second->becameIncidentToNetwork(this);
				}
			} else {
				incidentEdges.erase(*edgesPos);
				// Tell the Edge Monitors for this network to detach
				std::list<EdgeMonitor*> edgesToDetach;
				for (std::map<NetworkMonitor*,EdgeMonitor*>::iterator monPos = (*edgesPos)->monitors.begin();
						monPos != (*edgesPos)->monitors.end(); ++monPos)
				{
					if (monitors.count(monPos->first) > 0)
						edgesToDetach.push_back(monPos->second);
				}
				for (std::list<EdgeMonitor*>::iterator dpos = edgesToDetach.begin();
						dpos != edgesToDetach.end(); ++dpos)
				{
					(*dpos)->detach();
				}
				// Tell the Edge's monitors that the edge became incident to this Network
				for (std::map<NetworkMonitor*,EdgeMonitor*>::iterator monPos = (*edgesPos)->monitors.begin();
						monPos != (*edgesPos)->monitors.end(); ++monPos)
				{
					monPos->second->removedFromNetwork(this);
				}
			}
		}
	}
}

void Network::subsumeNetwork(const Network* network)
{
	const NodeSet& newNodes(network->getNodes());
	for (NodeSet::const_iterator pos = newNodes.begin();
			pos != newNodes.end(); ++pos)
	{
		addNode(*pos);
	}
}

void Network::subtractNetwork(const Network* network)
{
	std::list<Node*> toRemove;
	for (NodeSet::iterator pos = nodes.begin();
			pos != nodes.end(); ++pos)
	{
		if ((*pos)->isInNetwork(network))
			toRemove.push_back(*pos);
	}
	for (std::list<Node*>::iterator pos = toRemove.begin();
			pos != toRemove.end(); ++pos)
	{
		removeNode(*pos);
	}
}

void Network::intersectNetwork(const Network* network)
{
	std::list<Node*> toRemove;
	for (NodeSet::iterator pos = nodes.begin();
			pos != nodes.end(); ++pos)
	{
		if (!(*pos)->isInNetwork(network))
			toRemove.push_back(*pos);
	}
	for (std::list<Node*>::iterator pos = toRemove.begin();
			pos != toRemove.end(); ++pos)
	{
		removeNode(*pos);
	}
}

void Network::clear()
{
	while (!nodes.empty()) {
		removeNode(*nodes.begin());
	}
}

void Network::erase()
{
	while (!incidentEdges.empty())
		delete *incidentEdges.begin();
	while (!interiorEdges.empty())
		delete *interiorEdges.begin();
	while (!nodes.empty())
		delete *nodes.begin();
}

void Network::close()
{
	while (!incidentEdges.empty()) {
		EdgeSet oldIncidentEdges(incidentEdges);
		for (EdgeSet::iterator pos = oldIncidentEdges.begin();
				pos != oldIncidentEdges.end(); ++pos)
		{
			Node* node = (*pos)->getNodeExteriorToNetwork(this);
			if (node != NULL) addNode(node);
		}
	}
}

void Network::includeShortestPathToExteriorNode(Node* node)
{
	if (node->isInNetwork(this))
		return;

	std::list<Network*> levels;
	Network covered(*this);

	bool nodeFound = false;
	bool moreEdges = true;
	while (!nodeFound && moreEdges) {
		nodeFound = false;
		moreEdges = false;
		Network* nextLevel = new Network;
		const EdgeSet& incidents(covered.getIncidentEdges());
		for (EdgeSet::const_iterator pos = incidents.begin();
				pos != incidents.end(); ++pos)
		{
			// *** network should just store in/out edges seperately, like nodes ***
			if ((*pos)->isOutgoingFromNetwork(&covered)) {
				moreEdges = true;
				Node* aNode = (*pos)->getNodeExteriorToNetwork(&covered);
				if (aNode != node) {
					nextLevel->addNode(aNode);
				} else {
					delete nextLevel;
					nodeFound = true;
					break;
				}
			}
		}
		if (!nodeFound) {
			for (NodeSet::iterator pos = nextLevel->getNodes().begin();
					pos != nextLevel->getNodes().end(); ++pos)
			{
				covered.addNode(*pos);
			}
			levels.push_front(nextLevel);
		}
	}

	if (nodeFound) {
		Node* nextNode = node;
		while (!levels.empty()) {
			addNode(nextNode);
			nextNode = nextNode->findEdgeFromNetwork(*levels.begin())->getOtherNode(nextNode);
			delete *levels.begin();
			levels.pop_front();
		}
		addNode(nextNode);
	} else {
		while (!levels.empty()) {
			delete *levels.begin();
			levels.pop_front();
		}
	}
	
}

std::pair<Network*,Network*> Network::getComponents(NetworkSet& components) const
{
	Network* min = NULL;
	Network* max = NULL;
	for (NetworkSet::iterator pos = components.begin();
			pos != components.end(); ++pos)
	{
		(*pos)->close();  // bad. brings in nodes exterior to original network.
		// need "bounded close"
	}
	for (NodeSet::iterator pos = nodes.begin();
			pos != nodes.end(); ++pos)
	{
		bool add = true;
		for (NetworkSet::iterator npos = components.begin();
				npos != components.end(); ++npos)
		{
			if ((*pos)->isInNetwork(*npos)) {
				add = false;
				break;
			}
		}
		if (add) {
			Network* newNetwork = new Network();
			newNetwork->addNode(*pos);
			newNetwork->close();  // bad. brings in nodes exterior to original network.
			components.insert(newNetwork);
			if (min == NULL || newNetwork->numberOfNodes() < min->numberOfNodes()) {
				min = newNetwork;
			}
			if (max == NULL || newNetwork->numberOfNodes() > max->numberOfNodes()) {
				max = newNetwork;
			}
		}
	}
	return std::make_pair(min, max);
}
	
unsigned Network::numberOfNodes() const
{
	return nodes.size();
}

bool Network::isEmpty() const
{
	return nodes.empty();
}

const NodeSet& Network::getNodes() const
{
	return nodes;
}

const EdgeSet& Network::getInteriorEdges() const
{
	return interiorEdges;
}

const EdgeSet& Network::getIncidentEdges() const
{
	return incidentEdges;
}

/*
 * Static
 */

void Network::registerNodePrototype(const Node* nodePrototype)
{
	nodePrototypeRegister[nodePrototype->getTypeName()] =
		std::make_pair(nodePrototypeRegister.size(), nodePrototype);
}

void Network::registerEdgePrototype(const Edge* edgePrototype)
{
	edgePrototypeRegister[edgePrototype->getTypeName()] =
		std::make_pair(edgePrototypeRegister.size(), edgePrototype);
}

std::pair<bool,unsigned> Network::getIDForNodeTypeName(const std::string& typeName)
{
	std::map<std::string,std::pair<unsigned,const Node*> >::iterator
		pos(nodePrototypeRegister.find(typeName));
	if (pos != nodePrototypeRegister.end()) {
		return std::make_pair(true, pos->second.first);
	} else {
		return std::make_pair(false, 0);
	}
}

std::pair<bool,unsigned> Network::getIDForEdgeTypeName(const std::string& typeName)
{
	std::map<std::string,std::pair<unsigned,const Edge*> >::iterator
		pos(edgePrototypeRegister.find(typeName));
	if (pos != edgePrototypeRegister.end()) {
		return std::make_pair(true, pos->second.first);
	} else {
		return std::make_pair(false, 0);
	}
}

} // namespace Netitor
