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

#ifndef NETITOR_NETWORK_MONITOR_H_INCLUDED
#define NETITOR_NETWORK_MONITOR_H_INCLUDED

// Classes used as a value
#include "Netitor/sets.h"


// Classes used as a pointer, reference, or friend
namespace Netitor {
	class Network;
	class Node;
	class Edge;
	class NodeMonitor;
	class InteractionMonitor;
	class EdgeMonitor;
}

namespace Netitor {

class NetworkMonitor : public IDObject
{
	friend class Network;
	friend class Edge;
	friend class NodeMonitor;
	friend class InteractionMonitor;
	friend class EdgeMonitor;

	public:
	NetworkMonitor(Network* network);
	virtual ~NetworkMonitor();

	void detach();

	// Accessors
	public:
	Network* getNetwork() const;
	const NodeMonitorSet& getNodeMonitors() const;
	const InteractionMonitorSet& getInteractionMonitors() const;
	const EdgeMonitorSet& getEdgeMonitors() const;
	bool isAttached() const;

	// Events
	protected:
	virtual void addedNode(Node* node) { }
	virtual void addedEdge(Edge* edge) { }
	virtual void detachedThis() { }

	private:
	Network* network;
	NodeMonitorSet nodeMonitors;
	InteractionMonitorSet interactionMonitors;
	EdgeMonitorSet edgeMonitors;
};


} // namespace Netitor

#endif
