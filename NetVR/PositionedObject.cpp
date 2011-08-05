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

#include "NetVR/PositionedObject.h"

#include "NetVR/Octree.h"

#include <stdlib.h>


namespace NetVR {

PositionedObject::PositionedObject(Octree& root, const Point& position)
	: position(position), octreeLeaf(NULL)
{
	root.add(this);
}

PositionedObject::PositionedObject(Octree& root, float radius)
	: octreeLeaf(NULL)
{
	for (int i = 0; i < 3; ++i)
		position[i] = (double)rand()/(double)RAND_MAX * (radius*2.0) - radius;
	root.add(this);
}

PositionedObject::~PositionedObject()
{
	if (octreeLeaf != NULL)
		octreeLeaf->remove(this);
}

const Point& PositionedObject::getPosition() const
{
	return position;
}

const Point& PositionedObject::setPosition(const Point& newPosition)
{
	position = newPosition;
	updateOctree();
	return position;
}

const Point& PositionedObject::setPosition(unsigned dimension, float newPosition) throw(Failure)
{
	if (dimension > 2)
		throw(Failure("Dimension out of range"));
	position[dimension] = newPosition;
	updateOctree();
	return position;
}

const Point& PositionedObject::setRandomPosition(float radius)
{
	for (int i = 0; i < 3; ++i)
		position[i] = (double)rand()/(double)RAND_MAX * (radius*2.0) - radius;
	updateOctree();
	return position;
}

const Point& PositionedObject::offsetPosition(const Vector& offset)
{
	position[0] += offset[0];
	position[1] += offset[1];
	position[2] += offset[2];
	updateOctree();
	return position;
}

void PositionedObject::updateOctree()
{
	if (octreeLeaf != NULL)
		octreeLeaf->update(this);
}

const Octree* PositionedObject::getOctreeLeaf() const
{
	return octreeLeaf;
}

} // namespace NetVR
