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

/*
 * Network
 */

#ifndef NETITOR_NETWORK_H_INCLUDED
#define NETITOR_NETWORK_H_INCLUDED

// Classes used as a value
#include "Netitor/Failure.h"
#include <string>
#include <map>
#include "Netitor/sets.h"


// Classes used as a pointer, reference, or friend
namespace Netitor {
	class Node;
	class Edge;
	class DirectedEdge;
	class UndirectedEdge;
	class NetworkMonitor;
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

class Network : public IDObject
{
	friend class Edge; // for receiving updates
	friend class NetworkMonitor;

	/*
	 * Network theoretic interface
	 */
	public:
	Network();
	Network(const Network& network);
	virtual ~Network();

	void addNode(Node* const node);
	void removeNode(Node* const node);
	void subsumeNetwork(const Network* network);
	void subtractNetwork(const Network* network);
	void intersectNetwork(const Network* network);
	void clear();

	void erase();

	void close();
	void includeShortestPathToExteriorNode(Node* node);

	std::pair<Network*,Network*> getComponents(NetworkSet& components) const;
	
	unsigned numberOfNodes() const;
	bool isEmpty() const;

	const NodeSet& getNodes() const;
	const EdgeSet& getInteriorEdges() const;
	const EdgeSet& getIncidentEdges() const;

	private:
	NodeSet nodes;
	EdgeSet interiorEdges;
	EdgeSet incidentEdges;
	NetworkMonitorSet monitors;	

	/*
	 * XML input / output interface
	 */
	public:
	Network(std::istream& xmlSource) throw(Failure);
	virtual std::ostream& outputXMLDocument(std::ostream& destination) const;
	void load(std::istream& xmlSource) throw(Failure);

	protected:
	const Network& startRootElement(XML::DocumentBuilder& xmlDocument) const;
	const Network& outputNodesAndEdges(XML::DocumentBuilder& xmlDocument,
									   util::PointerToID& ptrToID) const;

	/*
	 * Registration of node and edge classes
	 */
	public:
	static void registerNodePrototype(const Node* nodePrototype);
	static void registerEdgePrototype(const Edge* Prototype);
	static std::pair<bool,unsigned> getIDForNodeTypeName(const std::string& typeName);
	static std::pair<bool,unsigned> getIDForEdgeTypeName(const std::string& typeName);

	private:
	static std::map<std::string,std::pair<unsigned,const Node*> > nodePrototypeRegister;
	static std::map<std::string,std::pair<unsigned,const Edge*> > edgePrototypeRegister;
};

} // namespace Netitor

#endif
