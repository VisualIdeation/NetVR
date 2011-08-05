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

#ifndef NETVR_EDGE_H_INCLUDED
#define NETVR_EDGE_H_INCLUDED

#include "NetVR/Geometry.h"

#include "Netitor/InteractionMonitor.h"
#include "NetVR/Action.h"

namespace Netitor {
	class NetworkMonitor;
}


namespace NetVR {

class Edge : public Netitor::InteractionMonitor
{
	public:
	Edge(Netitor::NetworkMonitor* networkMonitor, Netitor::Node* node1, Netitor::Node* node2);
	~Edge();

	void applySpringForceToNodes();

	void compile();
	void draw() const;

	bool contains(const Point& point) const;

	void reset();

	// Netitor::EdgeMonitor interface
	void detachedThis();

	private:
	const Action* bodyColor;
	const Action* bodyTexture;
	const ShapeAction* bodyShape;
	const Action* label;
	const Action* misc;
};


} // namespace NetVR

#endif
