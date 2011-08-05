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

#ifndef NETITOR_EDGE_MONITOR_H_INCLUDED
#define NETITOR_EDGE_MONITOR_H_INCLUDED

#include "Netitor/Failure.h"

#include "Netitor/IDObject.h"

// Classes used as a pointer, reference, or friend
namespace Netitor {
	class Network;
	class Node;
	class Edge;
	class NetworkMonitor;
}

namespace Netitor {

class EdgeMonitor : public IDObject
{
	friend class Network;
	friend class Edge;

	public:
	EdgeMonitor(NetworkMonitor* networkMonitor, Edge* edge);
	virtual ~EdgeMonitor();
 
	void detach();

 	// Accessors
 	public:
	NetworkMonitor* getNetworkMonitor() const;
	Network* getNetwork() const;
	Edge* getEdge() const throw(Failure);
	bool isAttached() const;

	// Events
	protected:
	virtual void attachedThis() {}
	virtual void changedProperty() {}
	virtual void becameIncidentToNetwork(Network* network) {}
	virtual void becameInteriorInNetwork(Network* network) {}
	virtual void removedFromNetwork(Network* network) {}
	virtual void detachedThis() {}

	private:
	NetworkMonitor* networkMonitor;
	Edge* edge;
};


} // namespace Netitor

#endif
