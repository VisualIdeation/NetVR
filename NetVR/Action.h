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

#ifndef NETVR_ACTION_H_INCLUDED
#define NETVR_ACTION_H_INCLUDED

#include "NetVR/Geometry.h"

namespace NetVR {

class Action
{
	public:
	Action() {}
	virtual ~Action() {}
	virtual Action* link(const void* thing) const = 0;
	virtual void operator()() const = 0;
};

class ShapeAction : public Action
{
	public:
	virtual ShapeAction* link(const void* thing) const = 0;
	virtual Point getCenter() const = 0;
	virtual bool contains(const Point& point) const = 0;
	virtual HitResult intersectionsWithRay(const Ray& ray) const = 0;
};

} // namespace NetVR

#endif
