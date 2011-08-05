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

#include "caveapp.h"

NetworkApplication::NetworkApplication(int &argc, char** &argv, char** &appDefaults)
 : Vrui::Application(argc, argv, appDefaults),
   mainMenu(0) {

	initializeNetwork();

	/* Create the user interface: */
	mainMenu=createMainMenu();

	char cwd[255];
	getcwd(cwd, 255);
	std::string data_dir(cwd);
	data_dir.append("/data");

	fileDialog = new GLMotif::FileSelectionDialog(mainMenu->getManager(), "Open file...", data_dir.c_str(), ".xml");
	fileDialog->getOKCallbacks().add(this, &NetworkApplication::fileOpenAction);
	fileDialog->getCancelCallbacks().add(this, &NetworkApplication::fileCancelAction);

	nodeInfoDialog = NULL;

	/* Install the main menu: */
	Vrui::setMainMenu(mainMenu);

	/* Set the navigation transformation: */
	resetNavigationCallback(0);
}

NetworkApplication::~NetworkApplication() {

	delete mainMenu;
}

void NetworkApplication::fileCancelAction(GLMotif::FileSelectionDialog::CancelCallbackData* cbData)
{
	Vrui::popdownPrimaryWidget(fileDialog);
}

void NetworkApplication::fileOpenAction(GLMotif::FileSelectionDialog::OKCallbackData* cbData)
{
   while(!components.empty()) {
	delete (*components.begin());
	components.erase(components.begin());
   }
   maxNetwork = NULL;
   std::cout << "start opening file...\n";
   mainNetwork->erase();
   //std::ifstream xmlData(("./data/" + file).c_str());
   std::ifstream xmlData(cbData->selectedFileName.c_str());

   //	srand(1001);

   mainNetwork->load(xmlData);

   std::pair<Netitor::Network*,Netitor::Network*> minmax = mainNetwork->getComponents(components);
   maxNetwork = minmax.second;
   visNetwork->subsumeNetwork(maxNetwork);
   //	visNetwork->subsumeNetwork(mainNetwork);
   phyNet->compile();
   //phyNet->degreeBasedLayout(1.5);
   //phyNet->forceDirectedLayout(10);
   for (int i = 0; i < 100; ++i)
 	  phyNet->forceDirectedLayoutStep(1);

   moving = true;

	bool popup = false;

	if(nodeInfoDialog != NULL)
	{
		if(nodeInfoDialog->state() == Dialog::ACTIVE)
		{
			nodeInfoDialog->hide();
			popup = true;
		}

		delete nodeInfoDialog;
	}

	Netitor::NodeSet s = mainNetwork->getNodes();
	Netitor::GenericNode* n = static_cast<Netitor::GenericNode*>(*(s.begin()));
	nodeInfoDialog = new NodeInfoDialog(mainMenu, n->getAttributeMap());

	if(popup)
	{
		nodeInfoDialog->show();
	}

	Vrui::popdownPrimaryWidget(fileDialog);
 }


void NetworkApplication::initializeNetwork() {
	std::cout << "NetworkApplication::initializeNetwork()..." << std::endl;

        Netitor::Network::registerNodePrototype(new Student());
//  Netitor::Network::registerEdgePrototype(new Met());

	mainNetwork = new Netitor::Network;
	visNetwork = new Netitor::Network;
	maxNetwork = NULL;

	phyNet	   = new NetVR::Network(visNetwork);
	singleSelect = new Netitor::Network;
	multiSelect  = new Netitor::Network;
	startSelect = new Netitor::Network;
	endSelect = new Netitor::Network;
	pathSelect = new Netitor::Network;

	moving = false;

	defaultNodeWeight = new NetVR::NodeStyleSelector();
	defaultNodeWeight->weightFromDegree = new bool(false);
	defaultNodeWeight->baseWeight = new float(2.0);

	NetVR::NodeStyleSelector *defaultNodeStyle = new NetVR::NodeStyleSelector();
	defaultNodeStyle->bodyShape = new SphereNode(0.25);
	//defaultNodeStyle->bodyShape = new CubeNode(0.5);
	defaultNodeStyle->bodyColor = new Material(GLMaterial::Color(0.0f,0.75f,1.0f),
						   GLMaterial::Color(1.0f,1.0f,1.0f), 80.0f);

	NetVR::EdgeStyleSelector *defaultEdgeStyle = new NetVR::EdgeStyleSelector();
	defaultEdgeStyle->bodyShape = new CylinderEdge();
	defaultEdgeStyle->bodyColor = new Material(GLMaterial::Color(0.65f,0.65f,0.65f),
		     				   GLMaterial::Color(0.65f,0.65f,0.65f), 80.0f);

	NetVR::EdgeStyleSelector *defaultDirectedEdgeStyle = new NetVR::EdgeStyleSelector();
	defaultDirectedEdgeStyle->bodyShape = new ArrowCylinderEdge();
	defaultDirectedEdgeStyle->bodyColor = new Material(GLMaterial::Color(0.65f,0.65f,0.65f),
							   GLMaterial::Color(0.65f,0.65f,0.65f), 80.0f);
	defaultDirectedEdgeStyle->onlyDirected = true;

	NetVR::NodeStyleSelector *selectedNodeStyle = new NetVR::NodeStyleSelector();
	selectedNodeStyle->bodyColor = new Material(GLMaterial::Color(1.0f,0.0f,0.0f),
						    GLMaterial::Color(1.0f,0.0f,0.0f), 120.0f);
	selectedNodeStyle->networks.insert(singleSelect);
	selectedNodeStyle->networks.insert(multiSelect);
	selectedNodeStyle->networks.insert(startSelect);
	selectedNodeStyle->networks.insert(endSelect);
	selectedNodeStyle->networks.insert(pathSelect);

	NetVR::EdgeStyleSelector *singleSelectedEdgeStyle = new NetVR::EdgeStyleSelector();
	singleSelectedEdgeStyle->bodyShape = new ArrowCylinderEdge();
	singleSelectedEdgeStyle->bodyColor = new Material(GLMaterial::Color(1.0f,0.6f,0.0f),
							  GLMaterial::Color(1.0f,0.6f,0.0f), 120.0f);
	singleSelectedEdgeStyle->networkInterior.insert(singleSelect);
	singleSelectedEdgeStyle->networkIncoming.insert(singleSelect);
	singleSelectedEdgeStyle->networkOutgoing.insert(singleSelect);
	singleSelectedEdgeStyle->networkInterior.insert(multiSelect);
	singleSelectedEdgeStyle->networkIncoming.insert(multiSelect);
	singleSelectedEdgeStyle->networkOutgoing.insert(multiSelect);

	NetVR::EdgeStyleSelector *pathSelectedEdgeStyle = new NetVR::EdgeStyleSelector();
	pathSelectedEdgeStyle->bodyShape = singleSelectedEdgeStyle->bodyShape;
	pathSelectedEdgeStyle->bodyColor = singleSelectedEdgeStyle->bodyColor;
	pathSelectedEdgeStyle->networkInterior.insert(pathSelect);

	// for political data
	NetVR::NodeStyleSelector *liberalStyle = new NetVR::NodeStyleSelector();
	liberalStyle->bodyShape = new SphereNode(0.25);
	liberalStyle->bodyColor = new Material(GLMaterial::Color(0.0f,0.0f,1.0f),
					     GLMaterial::Color(1.0f,1.0f,1.0f), 80.0f);
	liberalStyle->types.insert("l");

	NetVR::NodeStyleSelector *neutralStyle = new NetVR::NodeStyleSelector();
	neutralStyle->bodyShape = new SphereNode(0.25);
	neutralStyle->bodyColor = new Material(GLMaterial::Color(1.0f,1.0f,1.0f),
					     GLMaterial::Color(1.0f,1.0f,1.0f), 80.0f);
	neutralStyle->types.insert("n");

	NetVR::NodeStyleSelector *conservativeStyle = new NetVR::NodeStyleSelector();
	conservativeStyle->bodyShape = new SphereNode(0.25);
	conservativeStyle->bodyColor = new Material(GLMaterial::Color(1.0f,0.0f,0.0f),
					     GLMaterial::Color(1.0f,1.0f,1.0f), 80.0f);
	conservativeStyle->types.insert("c");

	// for bipartite actor/movie data
	NetVR::NodeStyleSelector *movieStyle = new NetVR::NodeStyleSelector();
	movieStyle->bodyShape = new CubeNode(1.0);
	movieStyle->bodyColor = new Material(GLMaterial::Color(0.0f,1.0f,0.0f),
					     GLMaterial::Color(1.0f,1.0f,1.0f), 80.0f);
	movieStyle->types.insert("movie");


	/*
	 * Setup Select Lists
	 */

	phyNet->nodeStyleSelectors.push_back(selectedNodeStyle);
	phyNet->nodeStyleSelectors.push_back(movieStyle);
	phyNet->nodeStyleSelectors.push_back(conservativeStyle);
	phyNet->nodeStyleSelectors.push_back(liberalStyle);
	phyNet->nodeStyleSelectors.push_back(neutralStyle);
	phyNet->nodeStyleSelectors.push_back(movieStyle);
	phyNet->nodeStyleSelectors.push_back(defaultNodeStyle);
	phyNet->nodeStyleSelectors.push_back(defaultNodeWeight);

	phyNet->edgeStyleSelectors.push_back(singleSelectedEdgeStyle);
	phyNet->edgeStyleSelectors.push_back(pathSelectedEdgeStyle);
	phyNet->edgeStyleSelectors.push_back(defaultDirectedEdgeStyle);
	phyNet->edgeStyleSelectors.push_back(defaultEdgeStyle);

	phyNet->compile();

	phyNet->outputPositions(std::cout);
}

void NetworkApplication::frame(void) {
  if (moving) {
	for (int i = 0; i < 10; ++i)
	  moving = moving && phyNet->forceDirectedLayoutStep(0.8);
	Vrui::requestUpdate();
  }
}

void NetworkApplication::display(GLContextData& contextData) const {

	phyNet->draw();
}

void NetworkApplication::resetNavigationCallback(Misc::CallbackData* cbData) {
	/* Reset the Vrui navigation transformation: */
	Vrui::Point modelCenter=Vrui::Point::origin;
	Vrui::setNavigationTransformation(modelCenter, 25.0);
}
