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

#ifndef NETVR_STYLE_SELECTOR_H_INCLUDED
#define NETVR_STYLE_SELECTOR_H_INCLUDED

#include "NetVR/Action.h"

#include "Netitor/sets.h"

#include <string>

namespace NetVR {
	class Node;
	class Edge;
}
namespace Netitor {
	class Network;
}

namespace NetVR {

class NodeStyleSelector
{
	public:
	NodeStyleSelector();
	~NodeStyleSelector();
	
	bool applicableToNode(Node* node) const;

	Netitor::NetworkSet networks;
	std::set<std::string> types;

	const Action* bodyColor;
	const Action* bodyTexture;
	const ShapeAction* bodyShape;
	const Action* label;
	const Action* misc;
	const bool* weightFromDegree;
	const float* baseWeight;
};

class EdgeStyleSelector
{
	public:
	EdgeStyleSelector();
	~EdgeStyleSelector();
	
	bool applicableToEdge(Edge* edge) const;

	Netitor::NetworkSet networkInterior;
	Netitor::NetworkSet networkIncoming;
	Netitor::NetworkSet networkOutgoing;
	std::set<std::string> types;
	bool onlyDirected;

	const Action* bodyColor;
	const Action* bodyTexture;
	const ShapeAction* bodyShape;
	const Action* label;
	const Action* misc;
};

} // namespace NetVR

#endif
