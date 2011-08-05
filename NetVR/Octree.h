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

#ifndef NETVR_OCTREE_H_INCLUDED
#define NETVR_OCTREE_H_INCLUDED

#include "NetVR/Geometry.h"
#include "NetVR/Failure.h"

#include <string>

namespace NetVR {
	class PositionedObject;
}


namespace NetVR {


class OctreeCoord
{
	public:
	OctreeCoord();
	OctreeCoord(const Point& p);

	unsigned int maskEqualMSBs(const OctreeCoord& other) const;
	void maskEqualMSBs(const OctreeCoord& other, unsigned int& mask) const;

	unsigned int& operator[](const unsigned int& i);
	const unsigned int& operator[](const unsigned int& i) const;

	void print() const;
	static void printBin(unsigned int bp);
	
	private:
	unsigned int pos[3];
};


class Octree
{
	friend class PositionedObject;

	public:
	Octree();
	virtual ~Octree();

	virtual void print() const;
	virtual void print(const std::string& prefix, int& count) const;

	protected:
	bool isRoot() const;
	bool isLeaf() const;
	bool isEmpty() const;
	bool containsCoord(const OctreeCoord& coord) const;

	const Octree* getParent() const;
	Octree* getParent();
	unsigned getIDInParent() const;

	PositionedObject* getPositionedObject() const;

	Octree* getOctant(unsigned octantID) const throw(Failure);
	unsigned getNumberOfOctants() const;
	unsigned getOctantID(const OctreeCoord& coord) const;

	virtual void changed() {};
	
	private:
	void setOctant(unsigned octantID, Octree* octant) throw(Failure);
	void clearOctant(unsigned octantID) throw(Failure);

	void add(PositionedObject* posObj) throw(Failure);
	void update(PositionedObject* posObj) throw(Failure);
	void remove(PositionedObject* posObj) throw(Failure);
	void remove() throw(Failure);

	virtual Octree* create();
	Octree* createLeaf(PositionedObject* posObj);
	Octree* createFiller(PositionedObject* posObj) throw(Failure);

	private:
	Octree* parent;
	unsigned idInParent;
	unsigned int level;
	OctreeCoord region;
	Octree* octants[8];
	unsigned numberOfOctants;
	PositionedObject* positionedObject;
};


} // namespace NetVR

#endif
