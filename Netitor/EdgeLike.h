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

#ifndef NETITOR_EDGE_LIKE_H_INCLUDED
#define NETITOR_EDGE_LIKE_H_INCLUDED

// Classes used as a value
#include <utility>
#include "Netitor/sets.h"

// Classes used as a pointer, reference, or friend
namespace Netitor {
	class Network;
	class Node;
	class Edge;
	class NetworkMonitor;
	class EdgeMonitor;
}


namespace Netitor {

class EdgeLike : public IDObject
{
	public:
	EdgeLike() : IDObject() { };
	virtual ~EdgeLike() { };

	virtual const std::pair<Node*, Node*>& getNodes() const = 0;
	Node*        getOtherNode(const Node* theNode) const;
	Node*        getNodeExteriorToNetwork(const Network* network) const;
	bool         isLoop      () const;
	virtual bool isDirected  () const = 0;
	bool         isAttached  () const;

	bool involvesNode(const Node* node) const;
	bool comesFromNode(const Node* node) const;
	bool goesToNode(const Node* node) const;

	NetworkSet getNetworks() const;
	bool isInNetwork(const Network* network) const;
	bool isInteriorInNetwork(const Network* network) const;
	bool isIncidentToNetwork(const Network* network) const;
	bool isIncomingIntoNetwork(const Network* network) const;
	bool isOutgoingFromNetwork(const Network* network) const;
};

} // namespace Netitor

#endif
