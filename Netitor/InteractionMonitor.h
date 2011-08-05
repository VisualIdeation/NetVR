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

#ifndef INTERACTION_MONITOR_H_INCLUDED
#define INTERACTION_MONITOR_H_INCLUDED

#include "Netitor/Failure.h"
#include "Netitor/EdgeLike.h"

#include "Netitor/sets.h"

// Classes used as a pointer, reference, or friend
namespace Netitor {
	class Network;
	class Node;
	class Edge;
	class NetworkMonitor;
}


namespace Netitor {


class InteractionMonitor : public EdgeLike
{
	friend class Network;
	friend class Edge;

	public:
	InteractionMonitor(NetworkMonitor* networkMonitor, Node* node1, Node* node2) throw(Failure);
	virtual ~InteractionMonitor();

	void detach();

	// EdgeLike interface
	const std::pair<Node*, Node*>& getNodes() const;
	virtual bool isDirected() const;
	
 	// Accessors
 	public:
	NetworkMonitor* getNetworkMonitor() const;
	Network* getNetwork() const;
	const EdgeSet& getEdges() const throw(Failure);
	bool hasInteraction() const;
	bool isAttached() const;

	// Events
	protected:
	virtual void attachedThis() {}
	virtual void addedEdge(Edge* edge) {}
	virtual void becameIncidentToNetwork(Network* network) {}
	virtual void becameInteriorInNetwork(Network* network) {}
	virtual void removedFromNetwork(Network* network) {}
	virtual void removedEdge(Edge* edge) {}
	virtual void detachedThis() {}
	
	private:
	NetworkMonitor* networkMonitor;
	std::pair<Node*,Node*> nodes;
	EdgeSet edges;
};


} // namespace Netitor

#endif
