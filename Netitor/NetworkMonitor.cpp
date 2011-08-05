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

#include "Netitor/NetworkMonitor.h"

#include "Netitor/Network.h"
#include "Netitor/Node.h"
#include "Netitor/Edge.h"
#include "Netitor/NodeMonitor.h"
#include "Netitor/InteractionMonitor.h"
#include "Netitor/EdgeMonitor.h"


namespace Netitor {

NetworkMonitor::NetworkMonitor(Network* network)
	: IDObject(), network(network)
{
	network->monitors.insert(this).second;
	for (NodeSet::iterator nodesPos = network->nodes.begin();
			nodesPos != network->nodes.end(); ++nodesPos)
	{
		addedNode(*nodesPos);
	}
	for (EdgeSet::iterator edgesPos = network->interiorEdges.begin();
			edgesPos != network->interiorEdges.end(); ++edgesPos)
	{
		addedEdge(*edgesPos);
	}
	for (EdgeSet::iterator edgesPos = network->incidentEdges.begin();
			edgesPos != network->incidentEdges.end(); ++edgesPos)
	{
		addedEdge(*edgesPos);
	}
}

NetworkMonitor::~NetworkMonitor()
{
	detach();
	while (!edgeMonitors.empty())
		delete *(edgeMonitors.begin());
	while (!interactionMonitors.empty())
		delete *(interactionMonitors.begin());
	while (!nodeMonitors.empty())
		delete *(nodeMonitors.begin());
}

void NetworkMonitor::detach()
{
	if (isAttached()) {
		for (EdgeMonitorSet::iterator edgesPos = edgeMonitors.begin();
				edgesPos != edgeMonitors.end(); ++edgesPos)
		{
			(*edgesPos)->detach();
		}
		for (InteractionMonitorSet::iterator edgesPos = interactionMonitors.begin();
				edgesPos != interactionMonitors.end(); ++edgesPos)
		{
			(*edgesPos)->detach();
		}
		for (NodeMonitorSet::iterator nodesPos = nodeMonitors.begin();
				nodesPos != nodeMonitors.end(); ++nodesPos)
		{
			(*nodesPos)->detach();
		}
		network->monitors.erase(this);
		network = NULL;
		detachedThis();
	}
}

/*
 * Accessors
 */

Network* NetworkMonitor::getNetwork() const
{
	return network;
}

const NodeMonitorSet& NetworkMonitor::getNodeMonitors() const
{
	return nodeMonitors;
}

const InteractionMonitorSet& NetworkMonitor::getInteractionMonitors() const
{
	return interactionMonitors;
}

const EdgeMonitorSet& NetworkMonitor::getEdgeMonitors() const
{
	return edgeMonitors;
}

bool NetworkMonitor::isAttached() const
{
	return network != NULL;
}

} // namespace Netitor
