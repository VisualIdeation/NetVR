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

#include "Netitor/EdgeLike.h"

#include "Netitor/Network.h"
#include "Netitor/Node.h"
#include "Netitor/NetworkMonitor.h"
#include "Netitor/NodeMonitor.h"
#include "Netitor/InteractionMonitor.h"
#include "Netitor/EdgeMonitor.h"


namespace Netitor {


Node* EdgeLike::getOtherNode(const Node* theNode) const
{
	if (getNodes().first == theNode) {
		return getNodes().second;
	} else if (getNodes().second == theNode) {
		return getNodes().first;
	} else {
		return NULL;
	}
}

Node* EdgeLike::getNodeExteriorToNetwork(const Network* network) const
{
	if (!isAttached()) {
		return NULL;
	} else if (!getNodes().first->isInNetwork(network)) {
		return getNodes().first;
	} else if (!getNodes().second->isInNetwork(network)) {
		return getNodes().second;
	} else {
		return NULL;
	}
}

bool EdgeLike::isLoop() const
{
	return (getNodes().first == getNodes().second);
}

bool EdgeLike::isAttached() const
{
	return (getNodes().first != NULL);
}

bool EdgeLike::involvesNode(const Node* node) const
{
	return (getNodes().first == node || getNodes().second == node);
}

bool EdgeLike::comesFromNode(const Node* node) const
{
	if (isDirected()) {
		return (getNodes().first == node);
	} else {
		return involvesNode(node);
	}
}

bool EdgeLike::goesToNode(const Node* node) const
{
	if (isDirected()) {
		return (getNodes().second == node);
	} else {
		return involvesNode(node);
	}
}

NetworkSet EdgeLike::getNetworks() const
{
	if (isAttached()) {
		NetworkSet networks(getNodes().first->getNetworks());
		networks.insert(getNodes().second->getNetworks().begin(), getNodes().second->getNetworks().end());
		return networks;
	} else {
		return NetworkSet();
	}
}

bool EdgeLike::isInNetwork(const Network* network) const
{
	if (isAttached()) {
		return getNodes().first->isInNetwork(network) || getNodes().second->isInNetwork(network);
	} else {
		return false;
	}
}

bool EdgeLike::isInteriorInNetwork(const Network* network) const
{
	if (isAttached()) {
		bool n1 = getNodes().first->isInNetwork(network);
		bool n2 = getNodes().second->isInNetwork(network);
		return n1 && n2;
	} else {
		return false;
	}
}

bool EdgeLike::isIncidentToNetwork(const Network* network) const
{
	if (isAttached()) {
		bool n1 = getNodes().first->isInNetwork(network);
		bool n2 = getNodes().second->isInNetwork(network);
		return (n1 || n2) && !(n1 && n2);
	} else {
		return false;
	}
}

bool EdgeLike::isIncomingIntoNetwork(const Network* network) const
{
	if (isDirected()) {
		if (isAttached()) {
			bool n1 = getNodes().first->isInNetwork(network);
			bool n2 = getNodes().second->isInNetwork(network);
			return !n1 && n2;
		} else {
			return false;
		}
	} else {
		return isIncidentToNetwork(network);
	}
}

bool EdgeLike::isOutgoingFromNetwork(const Network* network) const
{
	if (isDirected()) {
		if (isAttached()) {
			bool n1 = getNodes().first->isInNetwork(network);
			bool n2 = getNodes().second->isInNetwork(network);
			return n1 && !n2;
		} else {
			return false;
		}
	} else {
		return isIncidentToNetwork(network);
	}
}

} // namespace Netitor
