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

#include "Netitor/Netitor"

#include "class.h"

#include <iostream>

const std::string Student::typeName = "student"; // the name for this class
const std::string Met::typeName = "met";

class MyNetworkMonitor : public Netitor::NetworkMonitor
{
	public:
	MyNetworkMonitor(Netitor::Network* network)
		: Netitor::NetworkMonitor(network)
	{ }

	// Actions
	protected:
	virtual void addedNode(Netitor::Node* node)
		{ std::cout << "making a node monitor...\n"; new Netitor::NodeMonitor(this, node); }
	virtual void addedEdge(Netitor::Edge* edge)
		{ std::cout << "making an edge monitor...\n"; new Netitor::InteractionMonitor(this, edge->getNodes().first, edge->getNodes().second); }
};


int main(void)
{
	Netitor::Network::registerNodePrototype(new Student());
	Netitor::Network::registerEdgePrototype(new Met());

	Netitor::Network classNetwork(std::cin);

	Netitor::Network subnet;
	MyNetworkMonitor myNetMon(&subnet);
	
	std::set<Netitor::Node*>::iterator pos;
	for (pos = classNetwork.getNodes().begin();
			static_cast<Student*>(*pos)->first != "Braden"; ++pos);
	
	subnet.addNode(*pos);
	subnet.close();
	
	std::cout << "number of nodes: " << subnet.getNodes().size() << std::endl;
	std::cout << "number of edges: " << subnet.getInteriorEdges().size() << std::endl;
	std::cout << std::endl;
	
	subnet.outputXMLDocument(std::cout);

	return 0;
}


	/*
	Netitor::Network myNetwork;
	Student* myNode1 = new Student("Braden");
	Netitor::Node* myNode2 = new Netitor::Node();
	Netitor::Node* myNode3 = new Netitor::Node();
	Netitor::Node* myNode4 = new Netitor::Node();

	myNetwork.addNode(myNode1);
	myNetwork.addNode(myNode2);
	myNetwork.addNode(myNode3);

	new Netitor::DirectedEdge(myNode1, myNode2);
	new Met(myNode1, myNode3, 6);
	new Netitor::DirectedEdge(myNode2, myNode4);

	std::cout << myNode1->outDegree() << std::endl;
	std::cout << myNode1->inDegree() << std::endl;
	std::cout << myNode2->outDegree() << std::endl;
	std::cout << myNode2->inDegree() << std::endl;

	myNetwork.outputXMLDocument(std::cout);
	*/
