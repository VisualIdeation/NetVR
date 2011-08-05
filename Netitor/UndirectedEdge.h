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

#ifndef NETITOR_UNDIRECTED_EDGE_H_INCLUDED
#define NETITOR_UNDIRECTED_EDGE_H_INCLUDED

// Base classes
#include "Netitor/Edge.h"

// Classes used as a value
#include "Netitor/Failure.h"
#include <string>

// Classes used as a pointer, reference, or friend
namespace Netitor {
	class Network;
	class Node;
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

class UndirectedEdge : public Edge
{
	/*
	 * Network theoretic interface
	 */
	public:
	UndirectedEdge();
	UndirectedEdge(Node* const node1, Node* const node2);
	virtual ~UndirectedEdge();

	bool isDirected() const;

	bool isIncomingIntoNetwork(const Network* network) const;
	bool isOutgoingFromNetwork(const Network* network) const;

	/*
	 * Class info
	 */
	public:
	virtual const std::string& getTypeName() const;
	private:
	static const std::string typeName;

	/*
	 * XML input / output interface
	 */
	public:
	virtual Edge* create() const;
	protected:
	virtual Edge& initFromXML(XML::DocumentDismantler& xmlDocument, 
							  util::IDToPointer& idToPtr);
	virtual const Edge& outputXML(XML::DocumentBuilder& xmlDocument,
								  util::PointerToID& ptrToID) const;
	private:
	virtual Edge& enterRootElement(XML::DocumentDismantler& xmlDocument,
								   util::IDToPointer& idToPtr) throw(Failure);
	virtual const Edge& startRootElement(XML::DocumentBuilder& xmlDocument,
										 util::PointerToID& ptrToID) const;
};

} // namespace Netitor

#endif
