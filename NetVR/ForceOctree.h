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

#ifndef NETVR_FORCE_OCTREE_H_INCLUDED
#define NETVR_FORCE_OCTREE_H_INCLUDED

#include "NetVR/Octree.h"

#include "NetVR/Geometry.h"
#include "NetVR/Failure.h"


namespace NetVR {

class ForceOctree : public Octree
{
	// Embedded structure
	struct AverageData {
		AverageData() : min(0,0,0), max(0,0,0), s(0), position(0,0,0), weight(0) {}
		Point min;
		Point max;
		float s;
		Point position;
		float weight;
	};

	public:
	ForceOctree();
	~ForceOctree();

	void storeAverages();

	virtual void print() const;
	virtual void print(const std::string& prefix, int& count) const;

	void forceOnObj(const PositionedObject* posObj,
	                float tolerance, Vector& force) const throw(Failure);

	private:
	const AverageData& getAverageData() const throw(Failure);
	float getS() const throw(Failure);
	const Point& getAveragePosition() const throw(Failure);
	const Point& getMinimumPosition() const throw(Failure);
	const Point& getMaximumPosition() const throw(Failure);
	float getWeight() const throw(Failure);

	ForceOctree* getForceParent();
	const ForceOctree* getForceParent() const;
	ForceOctree* getForceOctant(unsigned octantID);
	const ForceOctree* getForceOctant(unsigned octantID) const;

	void sumForces(const Point& startPosition, const OctreeCoord& startCoord,
	               float tolerance, Vector& force) const;

	virtual void changed();
	virtual ForceOctree* create();

	private:
	AverageData *average;
};

} // namespace NetVR

#endif
