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

#ifndef NETITOR_IDOBJECT_H_INCLUDED
#define NETITOR_IDOBJECT_H_INCLUDED

namespace Netitor {

class IDObject
{
	public:
	IDObject() : idobject_id(IDObject::nextID++) { }
	virtual ~IDObject() { }

	struct less
	{
		bool operator()(const IDObject* idobj1, const IDObject* idobj2) const
		{
			return (idobj1->idobject_id < idobj2->idobject_id);
		}
	};

	private:
	static unsigned nextID;
	const unsigned idobject_id;
};

}

#endif
