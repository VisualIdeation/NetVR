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
 * Edge (Undirected) class
 */

#include "Netitor/Edge.h"

#include "Netitor/utilities.h"
#include "Netitor/Network.h"
#include "Netitor/Node.h"
#include "Netitor/NetworkMonitor.h"
#include "Netitor/NodeMonitor.h"
#include "Netitor/InteractionMonitor.h"
#include "Netitor/EdgeMonitor.h"
#include "XML/Document"

#ifndef NDEBUG
#include <iostream>
#include <assert.h>
#endif

namespace Netitor {


Edge::Edge()
	: EdgeLike(), nodes(NULL, NULL)
{
}

Edge::Edge(Node* const node1, Node* const node2)
	: EdgeLike()
{
	attach(node1, node2);
}

Edge::~Edge()
{ }

void Edge::attach(Node* const node1, Node* const node2)
{
	// Detach first if we are already attached
	detach();

	// Set pointers to the nodes
	nodes.first = node1;
	nodes.second = node2;

	// Update into on the attached Nodes
	node1->attachEdge(this, node2);
	node2->attachEdge(this, node1);

	// Update info on Networks
	NetworkSet networks(getNetworks());
	for (NetworkSet::iterator pos = networks.begin(); 
			pos != networks.end(); ++pos)
	{
		if (isInteriorInNetwork(*pos)) {
			(*pos)->interiorEdges.insert(this);
		} else {
			(*pos)->incidentEdges.insert(this);
		}
	}

	// Notify interaction monitors
	const std::map<NetworkMonitor*,InteractionMonitor*>&
			interactionMonitors(node1->getInteractionMonitors(node2));
	for (std::map<NetworkMonitor*,InteractionMonitor*>::const_iterator pos = interactionMonitors.begin();
			pos != interactionMonitors.end(); ++pos)
	{
		pos->second->edges.insert(this);
		pos->second->addedEdge(this);
	}

	// Notify network monitors
	for (NetworkSet::iterator pos = networks.begin(); 
			pos != networks.end(); ++pos)
	{
		for (NetworkMonitorSet::iterator pos2 = (*pos)->monitors.begin();
				pos2 != (*pos)->monitors.end(); ++pos2)
		{
			(*pos2)->addedEdge(this);
		}
	}

	// Notify nodes' monitors
	for (std::map<NetworkMonitor*,NodeMonitor*>::iterator pos = node1->monitors.begin();
			pos != node1->monitors.end(); ++pos)
	{
		pos->second->attachedEdge(this);
	}
	if (node1 != node2)
		for (std::map<NetworkMonitor*,NodeMonitor*>::iterator pos = node2->monitors.begin();
				pos != node2->monitors.end(); ++pos)
		{
			pos->second->attachedEdge(this);
		}
}

void Edge::detach()
{
	if (isAttached()) {
		// Update info on Networks
		NetworkSet networks(getNetworks());
		for (NetworkSet::iterator pos = networks.begin(); 
				pos != networks.end(); ++pos)
		{
			if (isInteriorInNetwork(*pos)) {
				(*pos)->interiorEdges.erase(this);
			} else {
				(*pos)->incidentEdges.erase(this);
			}
		}

		// Update info on the attached Nodes
		nodes.first->detachEdge(this, nodes.second);
		nodes.second->detachEdge(this, nodes.first);

		// Detach Edge Monitors
		while (!monitors.empty())
			monitors.begin()->second->detach();

		// Notify interaction monitors
		const std::map<NetworkMonitor*,InteractionMonitor*>&
				interactionMonitors(nodes.first->getInteractionMonitors(nodes.second));
		for (std::map<NetworkMonitor*,InteractionMonitor*>::const_iterator pos = interactionMonitors.begin();
				pos != interactionMonitors.end(); ++pos)
		{
			pos->second->edges.erase(this);
			pos->second->removedEdge(this);
		}

		// Notify nodes' monitors
		for (std::map<NetworkMonitor*,NodeMonitor*>::iterator pos = nodes.first->monitors.begin();
				pos != nodes.first->monitors.end(); ++pos)
		{
			pos->second->detachedEdge(this);
		}
		if (nodes.first != nodes.second) {
			for (std::map<NetworkMonitor*,NodeMonitor*>::iterator pos = nodes.second->monitors.begin();
					pos != nodes.second->monitors.end(); ++pos)
			{
				pos->second->detachedEdge(this);
			}
		}

		nodes.first = NULL;
		nodes.second = NULL;
	}
}

const std::pair<Node*, Node*>& Edge::getNodes() const
{
	return nodes;
}

/*
 * Class info
 */

void Edge::setLabel(const std::string& newLabel)
{
	label = newLabel;
}

const std::string& Edge::getLabel() const
{
	return label;
}

EdgeMonitor* Edge::getMonitor(const NetworkMonitor* networkMonitor) const
{
	std::map<NetworkMonitor*,EdgeMonitor*>::const_iterator pos =
		monitors.find(const_cast<NetworkMonitor*>(networkMonitor)); // WTF??
	if (pos != monitors.end()) {
		return pos->second;
	} else {
		return NULL;
	}
}


} // namespace Netitor
