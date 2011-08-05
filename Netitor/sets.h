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

#ifndef NETITOR_SET_H_INCLUDED
#define NETITOR_SET_H_INCLUDED

#include <set>
#include "Netitor/IDObject.h"

namespace Netitor {

	class Network;
	class Node;
	class Edge;
	class NetworkMonitor;
	class NodeMonitor;
	class InteractionMonitor;
	class EdgeMonitor;

	typedef std::set<Network*,IDObject::less> NetworkSet;
	typedef std::set<Node*,IDObject::less> NodeSet;
	typedef std::set<Edge*,IDObject::less> EdgeSet;

	typedef std::set<NetworkMonitor*,IDObject::less> NetworkMonitorSet;
	typedef std::set<NodeMonitor*,IDObject::less> NodeMonitorSet;
	typedef std::set<InteractionMonitor*,IDObject::less> InteractionMonitorSet;
	typedef std::set<EdgeMonitor*,IDObject::less> EdgeMonitorSet;

}

#endif
