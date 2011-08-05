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

#include "Netitor/NodeMonitor.h"

#include "Netitor/Network.h"
#include "Netitor/Node.h"
#include "Netitor/Edge.h"
#include "Netitor/NetworkMonitor.h"

#ifndef NDEBUG
#include <iostream>
#endif

namespace Netitor {

NodeMonitor::NodeMonitor(NetworkMonitor* networkMonitor, Node* node)
	: IDObject(), networkMonitor(networkMonitor), node(node)
{
	node->monitors.insert(std::make_pair(networkMonitor, this));
	networkMonitor->nodeMonitors.insert(this);
	attachedThis();
}

NodeMonitor::~NodeMonitor()
{
	detach();
	networkMonitor->nodeMonitors.erase(this);
}

void NodeMonitor::detach()
{
	if (isAttached()) {
		node->monitors.erase(networkMonitor);
		node = NULL;
		detachedThis();
	}
}

/*
 * Accessors
 */

NetworkMonitor* NodeMonitor::getNetworkMonitor() const
{
	return networkMonitor;
}

Network* NodeMonitor::getNetwork() const
{
	return networkMonitor->getNetwork();
}

Node* NodeMonitor::getNode() const throw(Failure)
{
	if (!isAttached())
		throw(Failure("Not attached to a node"));
	return node;
}

bool NodeMonitor::isAttached() const
{
	return node != NULL;
}


} // namespace Netitor
