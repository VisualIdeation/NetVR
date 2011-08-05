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

#include "NetVR/Network.h"

#include "Netitor/Network.h"
#include "Netitor/Node.h"
#include "Netitor/Edge.h"
#include "Netitor/NetworkMonitor.h"
#include "Netitor/NodeMonitor.h"
#include "Netitor/InteractionMonitor.h"
#include "Netitor/EdgeMonitor.h"

#include <cmath>

#include <iostream> // DEBUG


namespace NetVR {

Network::Network(Netitor::Network* network)
  : Netitor::NetworkMonitor(network), gravity(false)
{ }

Node* Network::findNodeContainingPosition(const Point& point) const
{
	const Netitor::NodeMonitorSet& nodeMonitors(getNodeMonitors());
	for (Netitor::NodeMonitorSet::iterator pos = nodeMonitors.begin();
			pos != nodeMonitors.end(); ++pos)
	{
		if (static_cast<Node*>(*pos)->contains(point))
			return static_cast<Node*>(*pos);
	}
	return NULL;
}

/*
Edge* Network::findEdgeContainingPosition(const Point& point) const
{
	const Netitor:EdgeMonitorSet& edgeMonitors(getEdgeMonitors());
	for (Netitor::EdgeMonitorSet::iterator pos = edgeMonitors.begin();
			pos != edgeMonitors.end(); ++pos)
	{
		if (static_cast<Edge*>(*pos)->contains(point))
			return static_cast<Edge*>(*pos);
	}
	return NULL;
}
*/

void Network::compile()
{
	const Netitor::NodeMonitorSet& nodeMonitors(getNodeMonitors());
	const Netitor::InteractionMonitorSet& interactionMonitors(getInteractionMonitors());
 
	for (Netitor::NodeMonitorSet::iterator pos = nodeMonitors.begin();
			pos != nodeMonitors.end(); ++pos)
	{
		static_cast<Node*>(*pos)->compile();
	}
	for (Netitor::InteractionMonitorSet::iterator pos = interactionMonitors.begin();
			pos != interactionMonitors.end(); ++pos)
	{
		static_cast<Edge*>(*pos)->compile();
	}
}

void Network::resetLayout()
{
	const Netitor::NodeMonitorSet& nodeMonitors(getNodeMonitors());
	for (Netitor::NodeMonitorSet::iterator pos = nodeMonitors.begin();
			pos != nodeMonitors.end(); ++pos)
	{
		static_cast<Node*>(*pos)->setRandomPosition(25.0);
	}
}

void Network::degreeBasedLayout(float multiplier)
{
	for (Netitor::NodeMonitorSet::iterator pos = getNodeMonitors().begin();
			pos != getNodeMonitors().end(); ++pos)
	{
		static_cast<Node*>(*pos)->setPosition(2, multiplier *
			((*pos)->getNode()->outDegree() - (*pos)->getNode()->inDegree()));
	}
}

void Network::forceDirectedLayout(const float& minimumForce)
{
  //	int i = 0;
	while (forceDirectedLayoutStep(minimumForce)) {
	  //++i;
	}
	//std::cout << i << std::endl;
}

bool Network::forceDirectedLayoutStep(const float& minimumForce)
{
	octree.storeAverages();
	
	const Netitor::NodeMonitorSet& nodeMonitors(getNodeMonitors());
	for (Netitor::NodeMonitorSet::iterator pos = nodeMonitors.begin();
			pos != nodeMonitors.end(); ++pos)
	{
		static_cast<Node*>(*pos)->zeroForce();
		static_cast<Node*>(*pos)->applyForcesFromOtherNodes();
	}

	const Netitor::InteractionMonitorSet& interactionMonitors(getInteractionMonitors());
	for (Netitor::InteractionMonitorSet::iterator pos = interactionMonitors.begin();
			pos != interactionMonitors.end(); ++pos)
	{
		static_cast<Edge*>(*pos)->applySpringForceToNodes();
	}

	bool moved = false;
	for (Netitor::NodeMonitorSet::iterator pos = nodeMonitors.begin();
			pos != nodeMonitors.end(); ++pos)
	{
		const Vector& force(static_cast<Node*>(*pos)->getForce());
		for (int i = 0; i < 3; ++i)
			if (std::abs(force[i]) > minimumForce) {
				static_cast<Node*>(*pos)->movesTowardsMinimumEnergy();
				moved = true;
				break;
			}
	}
	return moved;
}

void Network::outputPositions(std::ostream& out) const
{
	octree.print();
	for (Netitor::NodeMonitorSet::iterator pos = getNodeMonitors().begin();
			pos != getNodeMonitors().end(); ++pos)
	{
		out << static_cast<Node*>(*pos)->getPosition()[0] << "  ";
		out << static_cast<Node*>(*pos)->getPosition()[1] << "  ";
		out << static_cast<Node*>(*pos)->getPosition()[2] << std::endl;
	}
}

void Network::draw() const
{
	const Netitor::NodeMonitorSet& nodeMonitors(getNodeMonitors());
	const Netitor::InteractionMonitorSet& interactionMonitors(getInteractionMonitors());
 
	for (Netitor::NodeMonitorSet::iterator pos = nodeMonitors.begin();
			pos != nodeMonitors.end(); ++pos)
	{
		static_cast<Node*>(*pos)->draw();
	}
	for (Netitor::InteractionMonitorSet::iterator pos = interactionMonitors.begin();
			pos != interactionMonitors.end(); ++pos)
	{
		static_cast<Edge*>(*pos)->draw();
	}
}

/*
void Network::insertOrderedNodeStyleSelector(float order,
		const NodeStyleSelector* nodeStyleSelector)
{
	nodeStyleSelectors.insert(std::make_pair(order, nodeStyleSelector));
}

void Network::insertOrderedEdgeStyleSelector(float order,
		const EdgeStyleSelector* edgeStyleSelector)
{
	edgeStyleSelectors.insert(std::make_pair(order, edgeStyleSelector));
}

const std::map<float, const NodeStyleSelector*>& Network::getNodeStyleSelectors() const
{
	return nodeStyleSelectors;
}

const std::map<float, const EdgeStyleSelector*>& Network::getEdgeStyleSelectors() const
{
	return edgeStyleSelectors;
}
*/

void Network::addedNode(Netitor::Node* node)
{
	new Node(this, node, octree, 25.0);
//	octree.print();
//	std::cout << "************\n";
}

void Network::addedEdge(Netitor::Edge* edge)
{
	if (edge->getNodes().first->getInteractionMonitor(edge->getNodes().second, this) == NULL)
		new Edge(this, edge->getNodes().first, edge->getNodes().second);
}

} // namespace NetVR
