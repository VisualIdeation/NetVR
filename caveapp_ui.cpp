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


GLMotif::Popup* NetworkApplication::createLayoutTogglesMenu() {

  const GLMotif::StyleSheet &ss=*Vrui::getWidgetManager()->getStyleSheet();

  /* Create the submenus's top-level shell */
  GLMotif::Popup* layoutTogglesMenuPopup=new GLMotif::Popup("LayoutTogglesMenuPopup",
							    Vrui::getWidgetManager());
  layoutTogglesMenuPopup->setBorderWidth(0.0f);
  layoutTogglesMenuPopup->setBorderType(GLMotif::Widget::RAISED);
  layoutTogglesMenuPopup->setBorderColor(ss.bgColor);
  layoutTogglesMenuPopup->setBackgroundColor(ss.bgColor);
  layoutTogglesMenuPopup->setForegroundColor(ss.fgColor);
  layoutTogglesMenuPopup->setMarginWidth(ss.size);

  /* Create the aray of togle buttons inside the top-level shell */
  GLMotif::RowColumn* layoutTogglesMenu=new GLMotif::RowColumn("LayoutTogglesMenu",
							       layoutTogglesMenuPopup,
							       false);
  layoutTogglesMenu->setBorderWidth(0.0f);
  layoutTogglesMenu->setOrientation(GLMotif::RowColumn::VERTICAL);
  layoutTogglesMenu->setNumMinorWidgets(1);
  layoutTogglesMenu->setMarginWidth(0.0f);
  layoutTogglesMenu->setSpacing(ss.size);

  /* Add buttons */
  GLMotif::ToggleButton* addGravityButton=new GLMotif::ToggleButton("AddGravityButton",
								    layoutTogglesMenu,
								    "Add Gravity",
								    ss.font);
  addGravityButton->setToggle(false);
  addGravityButton->getValueChangedCallbacks().add(this, &NetworkApplication::layoutTogglesCallback);

  GLMotif::ToggleButton* degreeWeightedNodesButton=new GLMotif::ToggleButton("DegreeWeightedNodesButton",
									     layoutTogglesMenu,
									     "Node Charge By Degree",
									     ss.font);
  degreeWeightedNodesButton->setToggle(false);
  degreeWeightedNodesButton->getValueChangedCallbacks().add(this, &NetworkApplication::layoutTogglesCallback);

  GLMotif::Button* resetLayoutButton=new GLMotif::Button("ResetLayoutButton",
							 layoutTogglesMenu,
							 "Reset Layout",
							 ss.font);
  resetLayoutButton->getSelectCallbacks().add(this,&NetworkApplication::resetLayoutCallback);

  layoutTogglesMenu->manageChild();

  return layoutTogglesMenuPopup;
}

GLMotif::Popup* NetworkApplication::createViewTogglesMenu() {

  const GLMotif::StyleSheet &ss=*Vrui::getWidgetManager()->getStyleSheet();

  /* Create the submenus's top-level shell */
  GLMotif::Popup* viewTogglesMenuPopup=new GLMotif::Popup("ViewTogglesMenuPopup",
							  Vrui::getWidgetManager());
  viewTogglesMenuPopup->setBorderWidth(0.0f);
  viewTogglesMenuPopup->setBorderType(GLMotif::Widget::RAISED);
  viewTogglesMenuPopup->setBorderColor(ss.bgColor);
  viewTogglesMenuPopup->setBackgroundColor(ss.bgColor);
  viewTogglesMenuPopup->setForegroundColor(ss.fgColor);
  viewTogglesMenuPopup->setMarginWidth(ss.size);

  /* Create the aray of togle buttons inside the top-level shell */
  GLMotif::RowColumn* viewTogglesMenu=new GLMotif::RowColumn("ViewTogglesMenu",
							     viewTogglesMenuPopup,
							     false);
  viewTogglesMenu->setBorderWidth(0.0f);
  viewTogglesMenu->setOrientation(GLMotif::RowColumn::VERTICAL);
  viewTogglesMenu->setNumMinorWidgets(1);
  viewTogglesMenu->setMarginWidth(0.0f);
  viewTogglesMenu->setSpacing(ss.size);

  GLMotif::ToggleButton* showAllToggleButton=new GLMotif::ToggleButton("ShowAllToggleButton",
								       viewTogglesMenu,
								       "All Components",
								       ss.font);
  showAllToggleButton->getValueChangedCallbacks().add(this, &NetworkApplication::viewTogglesCallback);

  GLMotif::ToggleButton* showLargestToggleButton=new GLMotif::ToggleButton("ShowLargestToggleButton",
									   viewTogglesMenu,
									   "Largest Component",
									   ss.font);
  showLargestToggleButton->getValueChangedCallbacks().add(this, &NetworkApplication::viewTogglesCallback);
  showLargestToggleButton->setToggle(true);

  GLMotif::ToggleButton* showSelectedToggleButton=new GLMotif::ToggleButton("ShowSelectedToggleButton",
									    viewTogglesMenu,
									    "Selected Component(s)",
									    ss.font);
  showSelectedToggleButton->getValueChangedCallbacks().add(this, &NetworkApplication::viewTogglesCallback);

  viewToggleButtons.push_back(showAllToggleButton);
  viewToggleButtons.push_back(showLargestToggleButton);
  viewToggleButtons.push_back(showSelectedToggleButton);

  viewTogglesMenu->manageChild();

  return viewTogglesMenuPopup;
}

GLMotif::Popup* NetworkApplication::createSelectionTogglesMenu() {

  const GLMotif::StyleSheet &ss=*Vrui::getWidgetManager()->getStyleSheet();

  /* Create the submenus's top-level shell */
  GLMotif::Popup* selectionTogglesMenuPopup=new GLMotif::Popup("SelectionTogglesMenuPopup",
							       Vrui::getWidgetManager());
  selectionTogglesMenuPopup->setBorderWidth(0.0f);
  selectionTogglesMenuPopup->setBorderType(GLMotif::Widget::RAISED);
  selectionTogglesMenuPopup->setBorderColor(ss.bgColor);
  selectionTogglesMenuPopup->setBackgroundColor(ss.bgColor);
  selectionTogglesMenuPopup->setForegroundColor(ss.fgColor);
  selectionTogglesMenuPopup->setMarginWidth(ss.size);

  /* Create the aray of togle buttons inside the top-level shell */
  GLMotif::RowColumn* selectionTogglesMenu=new GLMotif::RowColumn("SelectionTogglesMenu",
								  selectionTogglesMenuPopup,
								  false);
  selectionTogglesMenu->setBorderWidth(0.0f);
  selectionTogglesMenu->setOrientation(GLMotif::RowColumn::VERTICAL);
  selectionTogglesMenu->setNumMinorWidgets(1);
  selectionTogglesMenu->setMarginWidth(0.0f);
  selectionTogglesMenu->setSpacing(ss.size);

  GLMotif::ToggleButton* singleSelectionToggleButton=new GLMotif::ToggleButton("SingleSelectionToggleButton",
									       selectionTogglesMenu,
									       "Single Highlight",
									       ss.font);
  singleSelectionToggleButton->getValueChangedCallbacks().add(this, &NetworkApplication::selectionTogglesCallback);
  singleSelectionToggleButton->setToggle(true);

  GLMotif::ToggleButton* multiSelectionToggleButton=new GLMotif::ToggleButton("MultiSelctionToggleButton",
									      selectionTogglesMenu,
									      "Multi Highlight",
									      ss.font);
  multiSelectionToggleButton->getValueChangedCallbacks().add(this, &NetworkApplication::selectionTogglesCallback);


  GLMotif::ToggleButton* pathFromNodeToggleButton=new GLMotif::ToggleButton("PathFromNodeToggleButton",
									    selectionTogglesMenu,
									    "Path From Node",
									    ss.font);
  pathFromNodeToggleButton->getValueChangedCallbacks().add(this, &NetworkApplication::selectionTogglesCallback);

  GLMotif::ToggleButton* pathToNodeToggleButton=new GLMotif::ToggleButton("PathToNodeToggleButton",
									  selectionTogglesMenu,
									  "Path To Node",
									  ss.font);
  pathToNodeToggleButton->getValueChangedCallbacks().add(this, &NetworkApplication::selectionTogglesCallback);



  selectionToggleButtons.push_back(singleSelectionToggleButton);
  selectionToggleButtons.push_back(multiSelectionToggleButton);
  selectionToggleButtons.push_back(pathFromNodeToggleButton);
  selectionToggleButtons.push_back(pathToNodeToggleButton);
  selectionTogglesMenu->manageChild();

  return selectionTogglesMenuPopup;
}

GLMotif::PopupMenu* NetworkApplication::createMainMenu() {

  /* Create a popup shell to hold the main menu */
  GLMotif::PopupMenu* mainMenuPopup=new GLMotif::PopupMenu("MainMenuPopup",Vrui::getWidgetManager());
  mainMenuPopup->setBorderWidth(0.0f);
  mainMenuPopup->setBorderType(GLMotif::Widget::RAISED);
  mainMenuPopup->setBorderColor(Vrui::getUiBgColor());
  mainMenuPopup->setBackgroundColor(Vrui::getUiBgColor());
  mainMenuPopup->setForegroundColor(Vrui::getUiFgColor());
  mainMenuPopup->setMarginWidth(Vrui::getUiSize());
  mainMenuPopup->setTitleSpacing(Vrui::getUiSize());
  mainMenuPopup->setTitle("NetVR",Vrui::getUiFont());

  /* Create the main menu itself */
  GLMotif::Menu* mainMenu=new GLMotif::Menu("MainMenu",mainMenuPopup,false);
  mainMenu->setBorderWidth(0.0f);
  mainMenu->setOrientation(GLMotif::Menu::VERTICAL);
  mainMenu->setNumMinorWidgets(1);
  mainMenu->setMarginWidth(0.0f);
  mainMenu->setSpacing(Vrui::getUiSize());

  /* Create buttons and assign callback methods */
  GLMotif::Button* openFileButton=new GLMotif::Button("OpenFileButton",mainMenu,"Open File",Vrui::getUiFont());
  openFileButton->getSelectCallbacks().add(this,&NetworkApplication::openFileCallback);


  GLMotif::CascadeButton* layoutTogglesCascade=new GLMotif::CascadeButton("LayoutTogglesCascade",
									  mainMenu,
									  "Layout Options",
									  Vrui::getUiFont());
  layoutTogglesCascade->setPopup(createLayoutTogglesMenu());

  GLMotif::CascadeButton* viewTogglesCascade=new GLMotif::CascadeButton("ViewTogglesCascade",
									mainMenu,
									"View",
									Vrui::getUiFont());
  viewTogglesCascade->setPopup(createViewTogglesMenu());


  GLMotif::CascadeButton* selectionTogglesCascade=new GLMotif::CascadeButton("SelectionTogglesCascade",
									     mainMenu,
									     "Node Selection",
									     Vrui::getUiFont());
  selectionTogglesCascade->setPopup(createSelectionTogglesMenu());


  GLMotif::ToggleButton* showStudentInfoToggleButton=new GLMotif::ToggleButton("ShowStudentInfoToggleButton",
									       mainMenu,
									       "Show Student Info Dialog",
									       Vrui::getUiFont());

  showStudentInfoToggleButton->getValueChangedCallbacks().add(this, &NetworkApplication::mainMenuTogglesCallback);

  GLMotif::Button* resetNavigationButton=new GLMotif::Button("ResetNavigationButton",mainMenu,"Reset Navigation",Vrui::getUiFont());
  resetNavigationButton->getSelectCallbacks().add(this,&NetworkApplication::resetNavigationCallback);

  GLMotif::Button* quitApplicationButton=new GLMotif::Button("QuitApplicationButton",mainMenu,"Quit",Vrui::getUiFont());
  quitApplicationButton->getSelectCallbacks().add(this,&NetworkApplication::quitCallback);
  mainMenu->manageChild();
  return mainMenuPopup;
}


/*
 *  User-interface related callbacks
 */

void NetworkApplication::openFileCallback(Misc::CallbackData* cbData)
{
    Vrui::getWidgetManager()->popupPrimaryWidget(fileDialog, Vrui::getWidgetManager()->calcWidgetTransformation(mainMenu));
}

void NetworkApplication::layoutTogglesCallback(GLMotif::ToggleButton::ValueChangedCallbackData* cbData)
{
  std::string name=cbData->toggle->getName();

  if(name=="DegreeWeightedNodesButton")
    {
      if (cbData->set) {
	delete defaultNodeWeight->weightFromDegree;
	defaultNodeWeight->weightFromDegree = new bool(true);
	phyNet->compile();
	moving = true;
      } else {
	delete defaultNodeWeight->weightFromDegree;
	defaultNodeWeight->weightFromDegree = new bool(false);
	phyNet->compile();
	moving = true;
      }
    }
  else if(name=="AddGravityButton")
    {
      phyNet->gravity = cbData->set;
      phyNet->compile();
      moving = true;
    }

}

void NetworkApplication::resetLayoutCallback(Misc::CallbackData* cbData)
{
  phyNet->resetLayout();
  moving = true;
}

void NetworkApplication::viewTogglesCallback(GLMotif::ToggleButton::ValueChangedCallbackData* cbData)
{
  std::string name=cbData->toggle->getName();

  if(name=="ShowAllToggleButton")
    {
      visNetwork->subsumeNetwork(mainNetwork);
      phyNet->compile();
      moving = true;
    }
  else if(name=="ShowLargestToggleButton")
    {
      if (maxNetwork) {
	visNetwork->intersectNetwork(maxNetwork);
	visNetwork->subsumeNetwork(maxNetwork);
	phyNet->compile();
	moving = true;
      }
    }
  else if(name=="ShowSelectedToggleButton")
    {
      Netitor::Network newNetwork;
      newNetwork.subsumeNetwork(singleSelect);
      newNetwork.subsumeNetwork(multiSelect);
      newNetwork.subsumeNetwork(endSelect);
      newNetwork.subsumeNetwork(startSelect);
      newNetwork.subsumeNetwork(pathSelect);
      newNetwork.close();
      visNetwork->intersectNetwork(&newNetwork);
      visNetwork->subsumeNetwork(&newNetwork);
      phyNet->compile();
      moving = true;
    }

  /* Fake radio button behavior */
  std::vector<GLMotif::ToggleButton*>::iterator button;
  for(button=viewToggleButtons.begin(); button!=viewToggleButtons.end(); ++button)
    if(name != (*button)->getName())
      (*button)->setToggle(false);
    else
      (*button)->setToggle(true);
}

void NetworkApplication::selectionTogglesCallback(GLMotif::ToggleButton::ValueChangedCallbackData* cbData)
{
  std::string name = cbData->toggle->getName();

  std::cout << "Time to Toggle!\n";

  singleSelect->clear();
  multiSelect->clear();
  startSelect->clear();
  endSelect->clear();
  pathSelect->clear();
  phyNet->compile();
  std::cout << "Time to Stop Toggle!\n";

  if(name=="SingleSelectionToggleButton") {

  } else if(name=="MultiSelctionToggleButton") {

  } else if(name=="PathFromNodeToggleButton") {

  } else if(name=="PathToNodeToggleButton") {

  }

  /* Fake radio button behavior */
  std::vector<GLMotif::ToggleButton*>::iterator button;
  for(button=selectionToggleButtons.begin(); button!=selectionToggleButtons.end(); ++button)
    if(name != (*button)->getName())
      (*button)->setToggle(false);
    else
      (*button)->setToggle(true);

  std::cout << "Time to Stop Toggle!\n";

}

void NetworkApplication::mainMenuTogglesCallback(GLMotif::ToggleButton::ValueChangedCallbackData* cbData)
{
	std::string name = cbData->toggle->getName();

	if(name == "ShowStudentInfoToggleButton")
	{
		if(cbData->set)
		{
			nodeInfoDialog->show();
		}
		else
		{
			nodeInfoDialog->hide();
		}
	}
}
