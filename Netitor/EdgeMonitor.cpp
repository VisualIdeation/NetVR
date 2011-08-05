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

#include "Netitor/EdgeMonitor.h"

#include "Netitor/Network.h"
#include "Netitor/Node.h"
#include "Netitor/Edge.h"
#include "Netitor/NetworkMonitor.h"

#ifndef NDEBUG
#include <iostream>
#endif


namespace Netitor {

EdgeMonitor::EdgeMonitor(NetworkMonitor* networkMonitor, Edge* edge)
	: IDObject(), networkMonitor(networkMonitor), edge(edge)
{
	edge->monitors.insert(std::make_pair(networkMonitor, this));
	networkMonitor->edgeMonitors.insert(this);
	attachedThis();
}

EdgeMonitor::~EdgeMonitor()
{
	detach();
	networkMonitor->edgeMonitors.erase(this);
}

void EdgeMonitor::detach()
{
	if (isAttached()) {
		edge->monitors.erase(networkMonitor);
		edge = NULL;
		detachedThis();
	}
}

/*
 * Accessors
 */

NetworkMonitor* EdgeMonitor::getNetworkMonitor() const
{
	return networkMonitor;
}

Network* EdgeMonitor::getNetwork() const
{
	return networkMonitor->getNetwork();
}

Edge* EdgeMonitor::getEdge() const throw(Failure)
{
	if (!isAttached())
		throw(Failure("Not attached to an edge"));
	return edge;
}

bool EdgeMonitor::isAttached() const
{
	return edge != NULL;
}


} // namespace Evnet
