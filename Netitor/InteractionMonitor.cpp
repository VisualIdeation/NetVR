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

#include "Netitor/InteractionMonitor.h"

#include "Netitor/Network.h"
#include "Netitor/Node.h"
#include "Netitor/Edge.h"
#include "Netitor/NetworkMonitor.h"

#ifndef NDEBUG
#include <iostream>
#endif


namespace Netitor {

InteractionMonitor::InteractionMonitor(NetworkMonitor* networkMonitor, Node* node1, Node* node2) throw(Failure)
	: EdgeLike(), networkMonitor(networkMonitor), nodes(node1, node2)
{

	{
		std::map<Node*,std::map<NetworkMonitor*,InteractionMonitor*> >::iterator pos =
				nodes.first->interactionMonitors.find(nodes.second);
		if (pos == nodes.first->interactionMonitors.end()) {
			pos = nodes.first->interactionMonitors.insert(std::make_pair(nodes.second,
					std::map<NetworkMonitor*,InteractionMonitor*>())).first;
		}
		if (!(pos->second.insert(std::make_pair(networkMonitor, this)).second))
			throw(Failure("There already exists an interaction monitor for these nodes."));
	}

	if (nodes.first != nodes.second) {
		std::map<Node*,std::map<NetworkMonitor*,InteractionMonitor*> >::iterator pos =
				nodes.second->interactionMonitors.find(nodes.first);
		if (pos == nodes.second->interactionMonitors.end()) {
			pos = nodes.second->interactionMonitors.insert(std::make_pair(nodes.first,
					std::map<NetworkMonitor*,InteractionMonitor*>())).first;
		}
		if (!pos->second.insert(std::make_pair(networkMonitor, this)).second)
			throw(Failure("There already exists an interaction monitor for these nodes."));
	}

	networkMonitor->interactionMonitors.insert(this);

	// Add any edges that already exist to this
	edges = nodes.first->getEdgesRelatingNode(nodes.second);
	
	attachedThis();
}

InteractionMonitor::~InteractionMonitor()
{
	detach();
	networkMonitor->interactionMonitors.erase(this);
}

void InteractionMonitor::detach()
{
	if (isAttached()) {
		{

			std::map<Node*,std::map<NetworkMonitor*,InteractionMonitor*> >::iterator pos =
					nodes.first->interactionMonitors.find(nodes.second);
			pos->second.erase(networkMonitor);
			if (pos->second.empty())
				nodes.first->interactionMonitors.erase(pos);
		}

		if (nodes.first != nodes.second) {
			std::map<Node*,std::map<NetworkMonitor*,InteractionMonitor*> >::iterator pos =
					nodes.second->interactionMonitors.find(nodes.first);
			pos->second.erase(networkMonitor);
			if (pos->second.empty())
				nodes.second->interactionMonitors.erase(pos);
		}

		nodes.first = NULL;
		nodes.second = NULL;
		edges.clear();

		detachedThis();
	}
}

/*
 * NodeLike interface
 */

const std::pair<Node*,Node*>& InteractionMonitor::getNodes() const
{
	return nodes;
}

bool InteractionMonitor::isDirected() const
{
	bool directed = true;
	Node* fromNode = NULL;
	for (EdgeSet::iterator pos = edges.begin();
			pos != edges.end(); ++pos)
	{
		if (!(*pos)->isDirected()) {
			directed = false;
		} else if (fromNode == NULL) {
			fromNode = (*pos)->getNodes().first;
		} else if (fromNode != (*pos)->getNodes().first) {
			directed = false;
		}
	}
	return directed && (fromNode != NULL);
}


/*
 * Accessors
 */

NetworkMonitor* InteractionMonitor::getNetworkMonitor() const
{
	return networkMonitor;
}

Network* InteractionMonitor::getNetwork() const
{
	return networkMonitor->getNetwork();
}

const EdgeSet& InteractionMonitor::getEdges() const throw(Failure)
{
	if (!isAttached())
		throw(Failure("Not attached to any nodes"));
	return edges;
}

bool InteractionMonitor::hasInteraction() const
{
	return !edges.empty();
}

bool InteractionMonitor::isAttached() const
{
	return nodes.first != NULL;
}



} // namespace Netitor
