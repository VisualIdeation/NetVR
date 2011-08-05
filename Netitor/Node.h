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

#ifndef NETITOR_NODE_H_INCLUDED
#define NETITOR_NODE_H_INCLUDED

// Classes used as a value
#include <string>
#include <map>
#include "Netitor/sets.h"

// Classes used as a pointer, reference, or friend
namespace Netitor {
	class Network;
	class NetworkData;
	class Node;
	class Edge;
	class NetworkMonitor;
	class NodeMonitor;
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

class Node : public IDObject
{
	friend class Network; // for receiving events
	friend class Edge; // for receiving events
	friend class NodeMonitor;
	friend class InteractionMonitor;

	/*
	 * Network theoretic interface
	 */
	public:
	Node();
	virtual ~Node();

	int outDegree() const;
	int inDegree() const;
	int degree() const;

	bool isIsolated() const;

	const std::multiset<Edge*>& getInEdges() const;
	const std::multiset<Edge*>& getOutEdges() const;
	EdgeSet getIncidentEdges() const;
	Edge* findEdgeIntoNetwork(Network* network) const;
	Edge* findEdgeFromNetwork(Network* network) const;

	NodeSet getRelatedNodes() const;
	EdgeSet getEdgesRelatingNode(Node* node) const;

	const NetworkSet& getNetworks() const;
	bool isInNetwork(const Network* network) const;

	void setLabel(const std::string& newLabel);
	const std::string& getLabel() const;

	NodeMonitor* getMonitor(const NetworkMonitor* networkMonitor) const;
	const std::map<Node*,std::map<NetworkMonitor*,InteractionMonitor*> > getAllInteractionMonitors() const;
	const std::map<NetworkMonitor*,InteractionMonitor*>& getInteractionMonitors(const Node* node) const;
	InteractionMonitor* getInteractionMonitor(const Node* node, const NetworkMonitor* networkMonitor) const;

	// Instructions from edges
	private:
	void attachEdge(Edge* edge, Node* node2);
	void detachEdge(Edge* edge, Node* node2);

	// Instructions from this
	protected:
	void notifyMonitorsOfChangedProperty();

	private:
	std::string label;
	std::multiset<Edge*> inEdges;
	std::multiset<Edge*> outEdges;
	int degree_;
	NetworkSet networks;
	std::map<NetworkMonitor*,NodeMonitor*> monitors;
	std::map<Node*,std::map<NetworkMonitor*,InteractionMonitor*> > interactionMonitors;
	static const std::map<NetworkMonitor*,InteractionMonitor*> emptyInteractionMonitors;

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
	virtual Node* create() const;
	protected:
	virtual Node& initFromXML(XML::DocumentDismantler& xmlDocument, 
							  util::IDToPointer& idToPtr);
	virtual const Node& outputXML(XML::DocumentBuilder& xmlDocument,
								  util::PointerToID& ptrToID) const;
	private:
	virtual Node& enterRootElement(XML::DocumentDismantler& xmlDocument,
								   util::IDToPointer& idToPtr);
	virtual const Node& startRootElement(XML::DocumentBuilder& xmlDocument,
										 util::PointerToID& ptrToID) const;
};


} // namespace Netitor

#endif
