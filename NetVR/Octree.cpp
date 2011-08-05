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

#include "NetVR/Octree.h"

#include "NetVR/PositionedObject.h"

#include <cmath>

#include <iostream> // DEBUG

namespace NetVR {

/***************
 * OctreeCoord
 */

OctreeCoord::OctreeCoord()
{
	for (int i = 0; i < 3; ++i)
		pos[i] = 0;
}

OctreeCoord::OctreeCoord(const Point& p)
{
	for (int i = 0; i < 3; ++i) {
#ifdef _WTF
		float f = p[i];
		std::cout << "";
#else
		const float& f = p[i];
#endif
		pos[i] = *((unsigned int*)&f);
	}
}

unsigned int OctreeCoord::maskEqualMSBs(const OctreeCoord& other) const
{
	unsigned int mask = 0x80000000;
	maskEqualMSBs(other, mask);
	return mask;
}

void OctreeCoord::maskEqualMSBs(const OctreeCoord& other, unsigned int& mask) const
{
	unsigned int diff[3];

	for (int i = 0; i < 3; ++i)
		diff[i] = pos[i] ^ other[i];

	while (!((diff[0] & mask) || (diff[1] & mask) || (diff[2] & mask))) {
		mask >>= 1;
		mask |= 0x80000000;
	}

	mask <<= 1;
}

unsigned int& OctreeCoord::operator[](const unsigned int& i)
{
	return pos[i];
}

const unsigned int& OctreeCoord::operator[](const unsigned int& i) const
{
	return pos[i];
}

void OctreeCoord::print() const
{
	for (int j = 0; j < 3; ++j) {
		std::cout << j << " :: ";
		printBin(pos[j]);
		std::cout << std::endl;
	}
}

void OctreeCoord::printBin(unsigned int bp)
{
	unsigned int mask = 0x80000000;
	for (int i = 0; i < 32; ++i) {
		std::cout << ((bp & mask) != 0);
		if (i == 0 || i == 8)
			std::cout << " ";
		mask >>= 1;
	}
}


/***************
 * Octree
 */

Octree::Octree()
	: parent(NULL), idInParent(0), level(0), numberOfOctants(0), positionedObject(NULL)
{
	for (int i = 0; i < 8; ++i)
		octants[i] = NULL;
}

Octree::~Octree()
{
	if (positionedObject != NULL)
		positionedObject->octreeLeaf = NULL;
	for (int i = 0; i < 8; ++i)
		if (octants[i] != NULL)
			delete octants[i];
}

void Octree::print() const
{
	int i = 0;
	print("", i);
}

void Octree::print(const std::string& prefix, int& count) const
{
	std::cout << prefix << "[" << this << "](" << numberOfOctants << ")";
	if (isLeaf() && !isEmpty()) {
		++count;
		std::cout << " <" << positionedObject << "> " << count;
	}
	std::cout << std::endl;
	for (int i = 0; i < 8; ++i)
		if (octants[i] != NULL) {
			octants[i]->print(prefix + "-  ", count);
		}
}

/*
 * Protected
 */

bool Octree::isRoot() const
{
	return level == 0;
}

bool Octree::isLeaf() const
{
	return numberOfOctants == 0;
}

bool Octree::isEmpty() const
{
	return (numberOfOctants == 0 && positionedObject == NULL);
}

bool Octree::containsCoord(const OctreeCoord& coord) const
{
	for (int i = 0; i < 3; ++i)
		if ((coord[i] & level) != region[i])
			return false;
	return true;
}

const Octree* Octree::getParent() const
{
	return parent;
}

Octree* Octree::getParent()
{
	return parent;
}

unsigned Octree::getIDInParent() const
{
	return idInParent;
}

PositionedObject* Octree::getPositionedObject() const
{
	return positionedObject;
}

void Octree::setOctant(unsigned octantNumber, Octree* octant) throw(Failure)
{
	if (getOctant(octantNumber) != NULL)
		throw(Failure("Cannot set an already set octant"));
	octants[octantNumber] = octant;
	++numberOfOctants;
}

void Octree::clearOctant(unsigned octantNumber) throw(Failure)
{
	if (getOctant(octantNumber) != NULL) {
		octants[octantNumber] = NULL;
		--numberOfOctants;
	}
}

Octree* Octree::getOctant(unsigned octant) const throw(Failure)
{
	if (octant > 7)
		throw(Failure("Octant out of range."));
	return octants[octant];
}

unsigned Octree::getNumberOfOctants() const
{
	return numberOfOctants;
}

unsigned Octree::getOctantID(const OctreeCoord& coord) const
{
	unsigned int octantIDMask = level;
	bool octantID[3];

	octantIDMask >>= 1;
	octantIDMask |= 0x80000000;
	octantIDMask &= ~level;
	
	for (int i = 0; i < 3; ++i)
		octantID[i] = (coord[i] & octantIDMask) != 0;

	return octantID[0] + octantID[1] * 2 + octantID[2] * 4;
}

/*
 * Private methods for use by PositionedObject
 */

void Octree::add(PositionedObject* posObj) throw(Failure)
{
	const OctreeCoord givenCoord(posObj->getPosition());

	if (!containsCoord(givenCoord)) {
		parent->add(posObj);
	} else {

		changed();

		if (isLeaf() && !isEmpty()) {
			if (positionedObject->getPosition() == posObj->getPosition())
				throw(Failure("Can't put two objects at same position"));
			if (!isRoot()) {
				givenCoord.maskEqualMSBs(OctreeCoord(positionedObject->getPosition()), level);
				for (int i = 0; i < 3; ++i)
					region[i] = givenCoord[i] & level;
			}
			createLeaf(positionedObject);
			positionedObject = NULL;
		}

		if (isEmpty()) {
			positionedObject = posObj;
			posObj->octreeLeaf = this;
		} else {
			const unsigned octantID = getOctantID(givenCoord);
			if (octants[octantID] == NULL) {
				createLeaf(posObj);
			} else {
				if (octants[octantID]->containsCoord(givenCoord)) {
					octants[octantID]->add(posObj);
				} else {
					createFiller(posObj);
				}
			}
		}
	}
}

void Octree::update(PositionedObject* posObj) throw(Failure)
{
	if (!isLeaf())
		throw(Failure("Not a leaf"));
	if (posObj != positionedObject)
		throw(Failure("Does not contain supplied object"));

	const OctreeCoord coord(posObj->getPosition());
	
	if (containsCoord(coord)) {
		changed();
	} else {
		positionedObject = NULL;
		posObj->octreeLeaf = NULL;
		parent->add(posObj);
		remove();
		delete this;
	}
}

void Octree::remove(PositionedObject* posObj) throw(Failure)
{
	if (!isLeaf())
		throw(Failure("Not a leaf"));
	if (posObj != positionedObject)
		throw(Failure("Does not contain supplied object"));

	positionedObject = NULL;
	posObj->octreeLeaf = NULL;

	if (isRoot()) {
		changed();
	} else {
		remove();
		delete this;
	}
}

void Octree::remove() throw(Failure)
{
	if (!isEmpty())
		throw(Failure("Not empty"));
	if (isRoot())
		throw(Failure("Can't remove root"));

	parent->clearOctant(idInParent);
	if (!parent->isRoot() && (parent->getNumberOfOctants() == 1)) {
		unsigned filledOctantPos = 0;
		for (unsigned i = 0; i < 8; ++i)
			if (parent->octants[i] != NULL) {
				filledOctantPos = i;
				break;
			}
		parent->octants[filledOctantPos]->parent = parent->parent;
		parent->octants[filledOctantPos]->idInParent = parent->idInParent;
		parent->parent->octants[parent->idInParent] = parent->octants[filledOctantPos];
		parent->clearOctant(filledOctantPos);
		parent->parent->changed();
		delete parent;
	}
}

/*
 * Private Constructors
 */

Octree* Octree::create()
{
	return new Octree();
}

Octree* Octree::createLeaf(PositionedObject* posObj)
{
	const OctreeCoord coord(posObj->getPosition());
	const unsigned octantID = getOctantID(coord);

	Octree* child = create();

	setOctant(octantID, child);
	child->parent = this;
	child->idInParent = octantID;

	child->level = (level >> 1);
	child->level |= 0x80000000;

	for (int i = 0; i < 3; ++i)
		child->region[i] = coord[i] & child->level;

	child->positionedObject = posObj;

	posObj->octreeLeaf = child;
	
	return child;
}

Octree* Octree::createFiller(PositionedObject* posObj) throw(Failure)
{
	const OctreeCoord coord(posObj->getPosition());
	const unsigned octantID = getOctantID(coord);

	if (octants[octantID]->containsCoord(coord))
		throw(Failure("Filler is unnecessary"));

	Octree* child = octants[octantID];
	Octree* filler = create();

	octants[octantID] = filler;
	filler->parent = this;
	child->parent = filler;
	filler->idInParent = octantID;

	filler->level = level;
	coord.maskEqualMSBs(child->region, filler->level);
	for (int i = 0; i < 3; ++i)
		filler->region[i] = coord[i] & filler->level;

	const unsigned childOctantID = filler->getOctantID(child->region);
	filler->setOctant(childOctantID, child);
	child->idInParent = childOctantID;

	filler->add(posObj);

	return filler;
}



} // namespace NetVR
