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
#include "NetVR/NetVR"

#include "class.h"

#include <iostream>
#include <string>


class NamePrinter : public NetVR::Action
{
	public:
	NamePrinter() {}
	NamePrinter(Student* student)
		: name(student->first)
	{ }
	NamePrinter* link(const void* thing) const
	{
		return new NamePrinter(static_cast<Student*>(
			static_cast<const NetVR::Node*>(thing)->getNode()));
	}
	void operator()() const
	{
		std::cout << "A student named " << name << std::endl;
	}
	std::string name;
};


int main(void)
{
//	Netitor::Network::registerNodePrototype(new Student());
//	Netitor::Network::registerEdgePrototype(new Met());

	Netitor::Network classNetwork(std::cin);

	Netitor::NetworkSet components;
	std::pair<Netitor::Network*,Netitor::Network*> minmax =
		classNetwork.getComponents(components);

	NetVR::Network phyNet(minmax.second);

//	NetVR::NodeStyleSelector* defaultStyle = new NetVR::NodeStyleSelector();
//	defaultStyle->bodyColor = new NamePrinter();
//	phyNet.insertOrderedNodeStyleSelector(0, defaultStyle);

/*
	std::set<Netitor::Node*>::iterator pos;
	for (pos = classNetwork.getNodes().begin();
			static_cast<Student*>(*pos)->first != "Braden"; ++pos);

	subnet.addNode(*pos);
	subnet.close();
	
	std::cout << "number of nodes: " << subnet.getNodes().size() << std::endl;
	std::cout << "number of edges: " << subnet.getInteriorEdges().size() << std::endl;
	std::cout << std::endl;
*/

	std::cout << "started\n";
//	for (int i = 0; i < 100; ++i) {
		phyNet.forceDirectedLayout();
//	}
	std::cout << "finished\n";

//	phyNet.outputPositions(std::cout);
//	classNetwork.removeNode(*pos);
//	phyNet.outputPositions(std::cout);
	
//	phyNet.compile();
//	phyNet.draw();
	
//	classNetwork.outputXMLDocument(std::cout);

	return 0;
}
