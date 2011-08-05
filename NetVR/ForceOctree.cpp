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

#include "NetVR/ForceOctree.h"

#include "NetVR/PositionedObject.h"

#include <cmath>

#include <iostream> // DEBUG


namespace NetVR {

/*
 * ForceOctree
 */

ForceOctree::ForceOctree()
	: Octree(), average(NULL)
{ }

ForceOctree::~ForceOctree()
{
	if (average != NULL)
		delete average;
}

void ForceOctree::storeAverages()
{
	if (isLeaf())
		return;
	if (average != NULL)
		return;

	average = new AverageData;
	for (unsigned i = 0; i < 8; ++i)
		if (getOctant(i) != NULL)
			getForceOctant(i)->storeAverages();

	for (unsigned i = 0; i < 8; ++i)
		if (getOctant(i) != NULL)
			average->weight += getForceOctant(i)->getWeight();

	for (unsigned i = 0; i < 8; ++i)
		if (getOctant(i) != NULL) {
			const Point& avePos(getForceOctant(i)->getAveragePosition());
			for (unsigned j = 0; j < 3; ++j)
				average->position[j] += avePos[j] *
					(getForceOctant(i)->getWeight() / average->weight);
		}

	bool first = true;
	for (unsigned i = 0; i < 8; ++i)
		if (getOctant(i) != NULL) {
			const Point& min(getForceOctant(i)->getMinimumPosition());
			const Point& max(getForceOctant(i)->getMaximumPosition());
			if (first) {
				for (unsigned j = 0; j < 3; ++j) {
					average->min[j] = min[j];
					average->max[j] = max[j];
				}
				first = false;
			} else {
				for (unsigned j = 0; j < 3; ++j) {
					if (min[j] < average->min[j])
						average->min[j] = min[j];
					if (max[j] > average->max[j])
						average->max[j] = max[j];
				}
			}
		}

	for (unsigned j = 0; j < 3; ++j)
		if ((average->max[j] - average->min[j]) > average->s)
			average->s = average->max[j] - average->min[j];
}

void ForceOctree::print() const
{
	int i = 0;
	print("", i);
}

void ForceOctree::print(const std::string& prefix, int& count) const
{
	std::cout << prefix << "[" << this << "](" << getNumberOfOctants() << ")";
	if (isLeaf() || average != NULL)
		std::cout << " " << getWeight();
	std::cout << std::endl;
	for (int i = 0; i < 8; ++i)
		if (getOctant(i) != NULL) {
			getOctant(i)->print(prefix + "-  ", count);
		}
}

void ForceOctree::forceOnObj(const PositionedObject* posObj,
		float tolerance, Vector& force) const throw(Failure)
{
	if (!isLeaf() || isEmpty())
		throw(Failure("No Positioned Object to act upon"));
	if (getPositionedObject() != posObj)
		throw(Failure("Given Positioned Object is not here"));
	tolerance = std::abs(tolerance);
	const OctreeCoord coord(getPositionedObject()->getPosition());
	getForceParent()->sumForces(getPositionedObject()->getPosition(), coord, tolerance, force);
}

/*
 * Private
 */

const ForceOctree::AverageData& ForceOctree::getAverageData() const throw(Failure)
{
	if (average != NULL)
		return *average;
	else
		throw(Failure("The average has not been calculated"));
}

float ForceOctree::getS() const throw(Failure)
{
	if (isLeaf())
		return 0;
	else
		return getAverageData().s;
}

const Point& ForceOctree::getAveragePosition() const throw(Failure)
{
	if (isLeaf())
		return getPositionedObject()->getPosition();
	else
		return getAverageData().position;
}

const Point& ForceOctree::getMinimumPosition() const throw(Failure)
{
	if (isLeaf())
		return getPositionedObject()->getPosition();
	else
		return getAverageData().min;
}

const Point& ForceOctree::getMaximumPosition() const throw(Failure)
{
	if (isLeaf())
		return getPositionedObject()->getPosition();
	else
		return getAverageData().max;
}

float ForceOctree::getWeight() const throw(Failure)
{
  if (isLeaf())
    if (isEmpty())
      return 0.0;
    else {
      return getPositionedObject()->getWeight();
    }
  else
    return getAverageData().weight;
}

ForceOctree* ForceOctree::getForceParent()
{
	return static_cast<ForceOctree*>(getParent());
}

const ForceOctree* ForceOctree::getForceParent() const
{
	return static_cast<const ForceOctree*>(getParent());
}

ForceOctree* ForceOctree::getForceOctant(unsigned octantID)
{
	return static_cast<ForceOctree*>(getOctant(octantID));
}

const ForceOctree* ForceOctree::getForceOctant(unsigned octantID) const
{
	return static_cast<const ForceOctree*>(getOctant(octantID));
}

void ForceOctree::sumForces(const Point& startPosition, const OctreeCoord& startCoord,
                            float tolerance, Vector& force) const
{
	bool desend = false;
	bool contains = containsCoord(startCoord);
	unsigned octantID = 0;
	if (contains)
		octantID = getOctantID(startCoord);

	if (contains) {
		desend = true;
	} else {
		const Point& position(getAveragePosition());
		Vector localForce;
		float distanceSquared = 0;

		for (unsigned i = 0; i < 3; ++i) {
			localForce[i] = startPosition[i] - position[i];
			distanceSquared += localForce[i] * localForce[i];
		}

		if (!isLeaf()) {
			float ratio = getS() / std::sqrt(distanceSquared);
//std::cout << getS() << " / " << std::sqrt(distanceSquared) << " = " << ratio;
			if (ratio > tolerance) {
				desend = true;
//std::cout << " (desend)";
			} else {
//std::cout << " (use)";
			}
//std::cout << " <" << getPositionedObject() << ">, " << getNumberOfOctants();
//std::cout << std::endl;
		}

		if (!desend) {
			for (unsigned i = 0; i < 3; ++i) {
				localForce[i] *= getWeight() / std::pow((double)distanceSquared, 3.0 / 2.0);
				force[i] += localForce[i];
			}
		}
	}

	if (desend) {
		for (unsigned i = 0; i < 8; ++i) {
			if (getOctant(i) != NULL && !(contains && i == octantID)) {
				getForceOctant(i)->sumForces(startPosition, startCoord, tolerance, force);
			}
		}
	}

	if (contains) {
		if (isRoot()) {
			return;
		} else {
			getForceParent()->sumForces(startPosition, startCoord, tolerance, force);
			return;
		}
	}

}

void ForceOctree::changed()
{
	if (average != NULL) {
		delete average;
		average = NULL;
		if (!isRoot())
			getForceParent()->changed();
	} else if (isLeaf() && !isRoot()) {
		getForceParent()->changed();
	}
}

ForceOctree* ForceOctree::create()
{
	return new ForceOctree();
}

} // namespace NetVR
