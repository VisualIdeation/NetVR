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

#include "NetVR/StyleSelector.h"

#include "Netitor/Node.h"
#include "Netitor/Edge.h"
#include "NetVR/Node.h"
#include "NetVR/Edge.h"


namespace NetVR {

/*
 * NodeStyleSelector
 */

NodeStyleSelector::NodeStyleSelector()
	: bodyColor(NULL), bodyTexture(NULL), bodyShape(NULL), label(NULL), 
     misc(NULL), weightFromDegree(NULL), baseWeight(NULL)
{ }

NodeStyleSelector::~NodeStyleSelector() {
	if (bodyColor != NULL) delete bodyColor;
	if (bodyTexture != NULL) delete bodyTexture;
	if (bodyShape != NULL) delete bodyShape;
	if (label != NULL) delete label;
	if (misc != NULL) delete misc;
	if (weightFromDegree != NULL) delete weightFromDegree;
	if (baseWeight != NULL) delete baseWeight;
}

bool NodeStyleSelector::applicableToNode(Node* node) const
{
	bool networkTest = false;
	bool typeTest = false;

	if (networks.empty()) {
		networkTest = true;
	} else {
		for (Netitor::NetworkSet::iterator npos = networks.begin();
				npos != networks.end(); ++npos)
		{
			if (node->getNode()->isInNetwork(*npos)) {
				networkTest = true;
				break;
			}
		}
	}

	if (types.empty()) {
		typeTest = true;
	} else if (types.find(node->getNode()->getTypeName()) != types.end()) {
		typeTest = true;
	}

	return networkTest && typeTest;
}

/*
 * EdgeStyleSelector
 */

EdgeStyleSelector::EdgeStyleSelector()
	: onlyDirected(false), bodyColor(NULL), bodyTexture(NULL),
	  bodyShape(NULL), label(NULL), misc(NULL)
{ }

EdgeStyleSelector::~EdgeStyleSelector() {
	if (bodyColor != NULL) delete bodyColor;
	if (bodyTexture != NULL) delete bodyTexture;
	if (bodyShape != NULL) delete bodyShape;
	if (label != NULL) delete label;
	if (misc != NULL) delete misc;
}

bool EdgeStyleSelector::applicableToEdge(Edge* edge) const
{
	bool networkTest = false;
	bool typeTest = false;

	if (onlyDirected && !edge->isDirected())
		return false;

	for (Netitor::NetworkSet::iterator npos = networkInterior.begin();
			npos != networkInterior.end(); ++npos)
	{
		if (edge->isInteriorInNetwork(*npos)) {
			networkTest = true;
			break;
		}
	}

	for (Netitor::NetworkSet::iterator npos = networkIncoming.begin();
			npos != networkIncoming.end(); ++npos)
	{
		if (edge->isIncomingIntoNetwork(*npos)) {
			networkTest = true;
			break;
		}
	}

	for (Netitor::NetworkSet::iterator npos = networkOutgoing.begin();
			npos != networkOutgoing.end(); ++npos)
	{
		if (edge->isOutgoingFromNetwork(*npos)) {
			networkTest = true;
			break;
		}
	}

	if (networkInterior.empty() && networkIncoming.empty() && networkOutgoing.empty()) {
		networkTest = true;
	}

	if (types.empty()) {
		typeTest = true;
	}
//	else if (types.find(edge->getTypeName()) != types.end()) {
//		typeTest = true;
//	}

	return networkTest && typeTest;
}

} // namespace NetVR
