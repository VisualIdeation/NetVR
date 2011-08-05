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

#ifndef NETITOR_DIRECTED_EDGE_H_INCLUDED
#define NETITOR_DIRECTED_EDGE_H_INCLUDED

// Classes used as bases
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

class DirectedEdge : public Edge
{
	/*
	 * Network theoretic interface
	 */
	public:
	DirectedEdge();
	DirectedEdge(Node* const theFromNode, Node* const theToNode);
	virtual ~DirectedEdge();

	Node* getFromNode() const;
	Node* getToNode() const;

	bool isDirected() const;

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
	virtual DirectedEdge* create() const;
	protected:
	virtual DirectedEdge& initFromXML(XML::DocumentDismantler& xmlDocument, 
									  util::IDToPointer& idToPtr);
	virtual const DirectedEdge& outputXML(XML::DocumentBuilder& xmlDocument,
										  util::PointerToID& ptrToID) const;
	private:
	virtual DirectedEdge& enterRootElement(XML::DocumentDismantler& xmlDocument,
										   util::IDToPointer& idToPtr) throw(Failure);
	virtual const DirectedEdge& startRootElement(XML::DocumentBuilder& xmlDocument,
												 util::PointerToID& ptrToID) const;
};

} // namespace Netitor

#endif
