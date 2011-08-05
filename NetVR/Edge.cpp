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

#include "NetVR/Edge.h"

#include "Netitor/Node.h"
#include "NetVR/StyleSelector.h"
#include "NetVR/Network.h"

#include <map>

#include <iostream> // DEBUG

namespace NetVR {

Edge::Edge(Netitor::NetworkMonitor* networkMonitor, Netitor::Node* node1, Netitor::Node* node2)
	: Netitor::InteractionMonitor(networkMonitor, node1, node2),
	  bodyColor(NULL), bodyTexture(NULL), bodyShape(NULL), label(NULL), misc(NULL)
{ }

Edge::~Edge()
{
	reset();
}

void Edge::applySpringForceToNodes()
{
	if (isAttached()) {
		Vector force;
		Node* node1 = dynamic_cast<Node*>(getNodes().first->getMonitor(getNetworkMonitor()));
		Node* node2 = dynamic_cast<Node*>(getNodes().second->getMonitor(getNetworkMonitor()));
		if (node1 && node2) {
			for (unsigned i = 0; i < 3; ++i) {
				force[i] = (node2->getPosition()[i] - node1->getPosition()[i]);
			}
			node1->applyForce(force);
			node2->applyForce(force * -1.0f);
		}
	}
}

void Edge::compile()
{
//	const std::map<float, const EdgeStyleSelector*>&
//		edgeStyleSelectors(static_cast<Network*>(getNetworkMonitor())->getEdgeStyleSelectors());
	Network* network = static_cast<Network*>(getNetworkMonitor());

	reset();

	for (std::list<EdgeStyleSelector*>::const_iterator pos = network->edgeStyleSelectors.begin();
			pos != network->edgeStyleSelectors.end(); ++pos)
	{
		if ((*pos)->applicableToEdge(this)) {
			if (bodyColor == NULL && (*pos)->bodyColor != NULL)
				bodyColor = (*pos)->bodyColor->link(this);
			if (bodyTexture == NULL && (*pos)->bodyTexture != NULL)
				bodyTexture = (*pos)->bodyTexture->link(this);
			if (bodyShape == NULL && (*pos)->bodyShape != NULL)
				bodyShape = (*pos)->bodyShape->link(this);
			if (label == NULL && (*pos)->label != NULL)
				label = (*pos)->label->link(this);
			if (misc == NULL && (*pos)->misc != NULL)
				misc = (*pos)->misc->link(this);
		}
	}
}

void Edge::draw() const
{
	if (bodyColor == NULL || bodyShape == NULL)
		return;
	(*bodyColor)();
	if (bodyTexture != NULL) (*bodyTexture)();
	(*bodyShape)();	
	if (label != NULL) (*label)();
	if (misc != NULL) (*misc)();
}

bool Edge::contains(const Point& point) const
{
	if (bodyShape != NULL) {
		return bodyShape->contains(point);
	} else {
		return false;
	}
}

void Edge::reset()
{
	if (bodyColor != NULL) {
		delete bodyColor;
		bodyColor = NULL;
	}
	if (bodyTexture != NULL) {
		delete bodyTexture;
		bodyTexture = NULL;
	}
	if (bodyShape != NULL) {
		delete bodyShape;
		bodyShape = NULL;
	}
	if (label != NULL) {
		delete label;
		label = NULL;
	}
	if (misc != NULL) {
		delete misc;
		misc = NULL;
	}
}

/*
 * Netitor::EdgeMonitor interface
 */

void Edge::detachedThis()
{
	delete this;
}

} // namespace NetVR
