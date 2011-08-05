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

#include "NetVR/Node.h"

#include "NetVR/StyleSelector.h"
#include "NetVR/Network.h"
#include "NetVR/ForceOctree.h"

#include "Netitor/Node.h"
#include "Netitor/Edge.h"

#include <cmath>
#include <map>
#include <set>
#include <list>

#include <iostream>


namespace NetVR {

Node::Node(Netitor::NetworkMonitor* networkMonitor, Netitor::Node* node,
           ForceOctree& root, float radius)
	: Netitor::NodeMonitor(networkMonitor, node),
	  PositionedObject(root, radius),
	  bodyColor(NULL), bodyTexture(NULL), bodyShape(NULL), label(NULL), misc(NULL),
	  weightFromDegree(NULL), baseWeight(NULL)
{ }

Node::~Node()
{
	reset();
}

float Node::getWeight() const
{
	if (weightFromDegree && baseWeight)
		if (*weightFromDegree)
			return *baseWeight * (float)getNode()->degree();
		else
			return *baseWeight;
	else
		return 0;
}

void Node::zeroForce()
{
	for (unsigned i = 0; i < 3; ++i)
		force[i] = 0;
}

const Vector& Node::applyForce(const Vector& appliedForce)
{
	for (unsigned i = 0; i < 3; ++i)
		force[i] += appliedForce[i];
	return force;
}

const Vector& Node::applyRandomForce(float max)
{
  for (int i = 0; i < 3; ++i)
    force[i] += (float)rand()/(float)RAND_MAX * (max * 2.0) - max;
  return force;
}

const Vector& Node::applyForcesFromOtherNodes()
{

//std::cout << "octree leaf: " << getOctreeLeaf() << std::endl;

	// Force from nodes
	static_cast<const ForceOctree*>(getOctreeLeaf())->forceOnObj(this, 0.5, force);

	for (unsigned i = 0; i < 3; ++i)
		force[i] *= getWeight();

/*
std::cout << "-----------------------------\n";
	std::cout << "non-edge force: ";
	for (unsigned i = 0; i < 3; ++i)
		std::cout << force[i] << " ";
	std::cout << std::endl;
*/

	// Force from edges

/*
	Netitor::EdgeSet edges = getNode()->getIncidentEdges();
	for (Netitor::EdgeSet::iterator pos = edges.begin();
			pos != edges.end(); ++pos)
	{
		Node* node = static_cast<Node*>((*pos)->getOtherNode(getNode())->getMonitor(getNetworkMonitor()));
		if (node != NULL) {
			force[0] -= (getPosition()[0] - node->getPosition()[0]);
			force[1] -= (getPosition()[1] - node->getPosition()[1]);
			if ((*pos)->isDirected()) {
				if ((*pos)->comesFromNode(getNode())) {
					force[2] += (node->getPosition()[2] - getPosition()[2]) + 3.0;
				} else {
					force[2] += (node->getPosition()[2] - getPosition()[2]) - 3.0;
				}
			} else {
				force[2] -= (getPosition()[2] - node->getPosition()[2]);
			}
		}
	}
*/

/*
	Netitor::NodeSet netitorNodes(getNode()->getRelatedNodes());
	for (Netitor::NodeSet::iterator pos = netitorNodes.begin();
			pos != netitorNodes.end(); ++pos)
	{
		Node* node = static_cast<Node*>((*pos)->getMonitor(getNetworkMonitor()));
		if (node != NULL)
			for (unsigned i = 0; i < 3; ++i)
				force[i] -= (getPosition()[i] - node->getPosition()[i]);
	}

*/
	if (static_cast<Network*>(getNetworkMonitor())->gravity) {
	  force[2] +=  4.0 * (float)getNode()->outDegree();
	  force[2] += -4.0 * (float)getNode()->inDegree();
	}

/*
	std::cout << " w/ edge force: ";
	for (unsigned i = 0; i < 3; ++i)
		std::cout << force[i] << " ";
	std::cout << std::endl;
*/
	return force;
}

void Node::movesTowardsMinimumEnergy()
{
  Vector offset;
  for (int i = 0; i < 3; ++i) {
    // const int sign = (force[i] > 0) * 2 - 1;
    // offset[i] = 0.05 * sign;
    // offset[i] = 0.1 * sign * std::log(std::abs(force[i]) + 1);
    offset[i] = 0.05 * std::tanh(force[i]);
    // std::cout << force[i] << " --> " << offset[i] << "\n";
  }
  offsetPosition(offset);
}

const Vector& Node::getForce() const
{
	return force;
}

void Node::compile()
{
//	const std::map<float, const NodeStyleSelector*>&
//		nodeStyleSelectors(static_cast<Network*>(getNetworkMonitor())->getNodeStyleSelectors());
	Network* network = static_cast<Network*>(getNetworkMonitor());

	reset();

	for (std::list<NodeStyleSelector*>::const_iterator pos = network->nodeStyleSelectors.begin();
			pos != network->nodeStyleSelectors.end(); ++pos)
	{
		if ((*pos)->applicableToNode(this)) {
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
			if (weightFromDegree == NULL && (*pos)->weightFromDegree != NULL)
				weightFromDegree = new bool(*(*pos)->weightFromDegree);
			if (baseWeight == NULL && (*pos)->baseWeight != NULL)
				baseWeight = new float(*(*pos)->baseWeight);
		}
	}
}

void Node::draw() const
{
//	if (bodyColor == NULL || bodyShape == NULL)
//		return;
	if (bodyColor != NULL) (*bodyColor)();
	if (bodyTexture != NULL) (*bodyTexture)();
	if (bodyShape != NULL) (*bodyShape)();	
	if (label != NULL) (*label)();
	if (misc != NULL) (*misc)();
}

bool Node::contains(const Point& point) const
{
	if (bodyShape != NULL) {
		return bodyShape->contains(point);
	} else {
		return false;
	}
}

HitResult Node::intersectionsWithRay(const Ray& ray) const
{
	if (bodyShape != NULL) {
		return bodyShape->intersectionsWithRay(ray);
	} else {
		return HitResult();
	}
}


void Node::reset()
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
	if (weightFromDegree != NULL) {
		delete weightFromDegree;
		weightFromDegree = NULL;
	}
	if (baseWeight != NULL) {
		delete baseWeight;
		baseWeight = NULL;
	}
}


/*
 * Netitor::NodeMonitor interface
 */

void Node::detachedThis()
{
	delete this;
}


} // namespace NetVR
