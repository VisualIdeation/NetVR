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

#ifndef NETVR_POSITIONED_OBJECT_H_INCLUDED
#define NETVR_POSITIONED_OBJECT_H_INCLUDED

#include "NetVR/Geometry.h"
#include "NetVR/Failure.h"

namespace NetVR {
	class Octree;
}


namespace NetVR {

class PositionedObject
{
	friend class Octree;

	public:
	PositionedObject(Octree& root, const Point& position);
	PositionedObject(Octree& root, float radius = 1.0);
	virtual ~PositionedObject();

	const Point& getPosition() const;
	const Point& setPosition(const Point& newPosition);
	const Point& setRandomPosition(float radius = 1.0);
	const Point& setPosition(unsigned dimension, float newPosition) throw(Failure);
	const Point& offsetPosition(const Vector& offset);

	virtual float getWeight() const { return 1.0; };

	protected:
	void updateOctree();
	const Octree* getOctreeLeaf() const;

	private:
	Point position;
	Octree* octreeLeaf;
};


} // namespace NetVR

#endif
