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
#include "NetVR/ForceOctree.h"
#include <cmath>
#include <iostream>
#include <list>


class obj : public NetVR::PositionedObject
{
	public:
	NetVR::Point position;

	obj(NetVR::Octree& root)
		: NetVR::PositionedObject(root)
	{ }
};

int main(void)
{
	NetVR::ForceOctree octree;
	std::list<obj*> objs;
	
	for (int i = 0; i < 200; ++i)
		objs.push_back(new obj(octree));

	octree.storeAverages();
	octree.print();

	for (std::list<obj*>::iterator pos = objs.begin();
			pos != objs.end(); ++pos)
	{
		(*pos)->offsetPosition(NetVR::Vector(-0.1,.4,.2));
	}

/*
	std::cout << "<" << (*objs.begin()) << "> ";
	std::cout << std::endl;
	for (int i = 0; i < 3; i++)
		std::cout << (*objs.begin())->getPosition()[i] << "  ";
	std::cout << std::endl;
	(*objs.begin())->offsetPosition(NetVR::Vector(10,0,0));
	for (int i = 0; i < 3; i++)
		std::cout << (*objs.begin())->getPosition()[i] << "  ";
	std::cout << std::endl;
*/

	std::cout << "**********************\n";
	octree.print();
	octree.storeAverages();
	std::cout << "**********************\n";
	octree.print();

	/*
	for (std::list<obj*>::iterator pos = objs.begin();
			pos != objs.end(); ++pos)
	{
		std::cout << (*pos) << " - " << (*pos)->getOctreeLeaf() << " - " << (*pos)->getOctreeLeaf()->getPositionedObject() << " :: " << (*pos == (*pos)->getOctreeLeaf()->getPositionedObject()) << std::endl;
	}
	*/

	for (std::list<obj*>::iterator pos = objs.begin();
			pos != objs.end(); ++pos)
	{
		delete *pos;
	}

}
