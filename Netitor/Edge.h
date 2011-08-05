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

#ifndef NETITOR_EDGE_H_INCLUDED
#define NETITOR_EDGE_H_INCLUDED

#include "Netitor/EdgeLike.h"

// Classes used as a value
#include <map>
#include <string>
#include <utility>
#include "Netitor/sets.h"


// Classes used as a pointer, reference, or friend
namespace Netitor {
	class Network;
	class Node;
	class Edge;
	class NetworkMonitor;
	class EdgeMonitor;
}
namespace XML {
	class DocumentBuilder;
	class DocumentDismantler;
}
namespace util {
	class PointerToID;
	class IDToPointer;
}


namespace Netitor {

class Edge : public EdgeLike
{
	friend class Network;
	friend class EdgeMonitor;

	/*
	 * Network theoretic interface
	 */
	public:
	Edge();
	Edge(Node* const node1, Node* const node2);
	virtual ~Edge();

	virtual void attach(Node* const node1, Node* const node2);
	virtual void detach();

	// EdgeLike interface
	const std::pair<Node*, Node*>& getNodes() const;

	// Special for Edges
	void setLabel(const std::string& newLabel);
	const std::string& getLabel() const;

	EdgeMonitor* getMonitor(const NetworkMonitor* networkMonitor) const;

	private:
	std::string label;
	std::pair<Node*,Node*> nodes;
	std::map<NetworkMonitor*,EdgeMonitor*> monitors;

	/*
	 * Class info
	 */
	public:
	virtual const std::string& getTypeName() const = 0;

	/*
	 * XML input / output interface
	 */
	public:
	virtual Edge* create() const = 0;
	protected:
	virtual Edge& initFromXML(XML::DocumentDismantler& xmlDocument, 
							  util::IDToPointer& idToPtr) = 0;
	virtual const Edge& outputXML(XML::DocumentBuilder& xmlDocument,
								  util::PointerToID& ptrToID) const = 0;
	virtual Edge& enterRootElement(XML::DocumentDismantler& xmlDocument,
								   util::IDToPointer& idToPtr) = 0;
	virtual const Edge& startRootElement(XML::DocumentBuilder& xmlDocument,
										 util::PointerToID& ptrToID) const = 0;
};

} // namespace Netitor

#endif
