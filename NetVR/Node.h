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

#ifndef NETVR_NODE_H_INCLUDED
#define NETVR_NODE_H_INCLUDED

#include "NetVR/Geometry.h"

#include "Netitor/NodeMonitor.h"
#include "NetVR/Action.h"
#include "NetVR/PositionedObject.h"

#include <iostream> // DEBUG

namespace NetVR {
	class ForceOctree;
}
namespace Netitor {
	class NetworkMonitor;
}


namespace NetVR {

class Node : public Netitor::NodeMonitor, public PositionedObject
{
	public:
	Node(Netitor::NetworkMonitor* networkMonitor, Netitor::Node* node,
			ForceOctree& root, float radius = 2.0);
	~Node();

	float getWeight() const;

	void zeroForce();
	const Vector& applyForce(const Vector& appliedForce);
	const Vector& applyRandomForce(float max);
	const Vector& applyForcesFromOtherNodes();
	void movesTowardsMinimumEnergy();
	const Vector& getForce() const;

	void compile();
	void draw() const;

	bool contains(const Point& point) const;
	HitResult intersectionsWithRay(const Ray& ray) const;

	void reset();

	// Netitor::NodeMonitor interface
	void detachedThis();

	private:
	const Action* bodyColor;
	const Action* bodyTexture;
	const ShapeAction* bodyShape;
	const Action* label;
	const Action* misc;
	const bool* weightFromDegree;
	const float* baseWeight;

//	Vector velocity;
	Vector force;
};


} // namespace NetVR

#endif
