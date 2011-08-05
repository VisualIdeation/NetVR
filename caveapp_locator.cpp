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
#include "Netitor/GenericNode.h"

NetworkApplication::Locator::Locator(Vrui::LocatorTool* sTool,NetworkApplication* sApplication)
	:Vrui::LocatorToolAdapter(sTool),
	 application(sApplication),
	 active(false)
{
}

void NetworkApplication::Locator::motionCallback(Vrui::LocatorTool::MotionCallbackData* cbData) { }

void NetworkApplication::Locator::buttonPressCallback(Vrui::LocatorTool::ButtonPressCallbackData* cbData) {

  Vrui::NavTrackerState coord=cbData->currentTransformation;
  pos=coord.getOrigin();

  std::cout << "(" << pos[0] << ", " << pos[1] << ", " << pos[2] << ")..." << std::endl;

  NetVR::Node *phyNode = application->phyNet->findNodeContainingPosition(pos);

  if(phyNode != NULL) {

    Netitor::Node* node = phyNode->getNode();

    if (application->selectionToggleButtons[0]->getToggle()) {

      if (node->isInNetwork(application->singleSelect))
	application->singleSelect->clear();
      else {
	application->singleSelect->clear();
	application->singleSelect->addNode(node);
      }

    } else if (application->selectionToggleButtons[1]->getToggle()) {

      if (node->isInNetwork(application->multiSelect)) {
	application->multiSelect->removeNode(node);
      } else {
	application->multiSelect->addNode(node);
      }

    } else if (application->selectionToggleButtons[2]->getToggle()) {

      application->endSelect->clear();
      application->pathSelect->clear();
      if (application->startSelect->isEmpty()) {
	application->startSelect->addNode(node);
      } else {
	application->endSelect->addNode(node);
	application->pathSelect->subsumeNetwork(application->startSelect);
	application->pathSelect->includeShortestPathToExteriorNode(node);
      }

    } else if (application->selectionToggleButtons[3]->getToggle()) {

      application->endSelect->clear();
      application->pathSelect->clear();
      if (application->startSelect->isEmpty()) {
	application->startSelect->addNode(node);
      } else {
	application->endSelect->addNode(node);
	application->pathSelect->addNode(node);
	application->pathSelect->includeShortestPathToExteriorNode(*application->startSelect->getNodes().begin());
      }

    }

    application->phyNet->compile();


	if(application->nodeInfoDialog->state() == Dialog::ACTIVE)
	{
		std::cout << "Updating node dialog..." << std::endl;

		Netitor::GenericNode* n = dynamic_cast<Netitor::GenericNode*>(node);

		if(n)
		{
			application->nodeInfoDialog->update(n->getAttributeMap());
		}
		else
		{
			std::cout << "Cannot cast to Student ptr" << std::endl;
		}
	}

  }

}


void NetworkApplication::Locator::buttonReleaseCallback(Vrui::LocatorTool::ButtonReleaseCallbackData* cbData) { }

void NetworkApplication::Locator::glRenderAction(GLContextData &contextData) const { }

void NetworkApplication::toolCreationCallback(Vrui::ToolManager::ToolCreationCallbackData* cbData)
{
  /* Check if the new tool is a locator tool: */
  Vrui::LocatorTool* locatorTool=dynamic_cast<Vrui::LocatorTool*>(cbData->tool);
  if(locatorTool!=0)
    {
      /* Create a new locator and associate it with the new tool and this application: */
      Locator* locator=new Locator(locatorTool,this);

      /* Store the new locator in the list: */
      locators.push_back(locator);
    }
}

void NetworkApplication::toolDestructionCallback(Vrui::ToolManager::ToolDestructionCallbackData* cbData)
{
  /* Check if the destroyed tool is a locator tool: */
  Vrui::LocatorTool* locatorTool=dynamic_cast<Vrui::LocatorTool*>(cbData->tool);
  if(locatorTool!=0)
    {
      /* Find the locator in the list: */
      for(LocatorList::iterator lIt=locators.begin();lIt!=locators.end();++lIt)
	if((*lIt)->getTool()==locatorTool)
	  {
	    /* Remove the locator: */
	    delete *lIt;
	    locators.erase(lIt);
	    break;
	  }
    }
}
