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

#ifndef NETVR_NETWORK_H_INCLUDED
#define NETVR_NETWORK_H_INCLUDED

#include "NetVR/Geometry.h"

// Base classes
#include "Netitor/NetworkMonitor.h"

#include "NetVR/ForceOctree.h"
#include "NetVR/StyleSelector.h"
#include "NetVR/Node.h"
#include "NetVR/Edge.h"

#include <list>
#include <ostream>

// Classes used as a pointer, reference, or friend
namespace NetVR {
	class Node;
	class Edge;
}
namespace Netitor {
	class Network;
	class Node;
	class Edge;
}


namespace NetVR {


class Network : public Netitor::NetworkMonitor
{
	public:
	Network(Netitor::Network* network);

	Node* findNodeContainingPosition(const Point& point) const;
//	Edge* findEdgeContainingPosition(const Point& point) const;

	void compile();
	void resetLayout();
	void degreeBasedLayout(float multiplier);
	void forceDirectedLayout(const float& minimumForce = 0.2);
	bool forceDirectedLayoutStep(const float& minimumForce = 0);

	void outputPositions(std::ostream& out) const;
	void draw() const;

/*
	void insertOrderedNodeStyleSelector(float order,
			const NodeStyleSelector* nodeStyleSelector);
	void insertOrderedEdgeStyleSelector(float order,
			const EdgeStyleSelector* edgeStyleSelector);

	const std::map<float, const NodeStyleSelector*>& getNodeStyleSelectors() const;
	const std::map<float, const EdgeStyleSelector*>& getEdgeStyleSelectors() const;
*/

	// Netitor::NetworkMonitor interface
	protected:
	virtual void addedNode(Netitor::Node* node);
	virtual void addedEdge(Netitor::Edge* edge);

	private:
	Network* selected;
	ForceOctree octree;

	public:
	std::list<NodeStyleSelector*> nodeStyleSelectors;
	std::list<EdgeStyleSelector*> edgeStyleSelectors;

	bool gravity;
};


} // namespace NetVR

#endif
