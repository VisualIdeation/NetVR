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

#ifndef NETWORK_APPLICATION_H
#define NETWORK_APPLICATION_H


#include <Vrui/Vrui.h>
#include <Vrui/Application.h>
#include <Vrui/Tools/LocatorTool.h>
#include <Vrui/LocatorToolAdapter.h>
#include <Vrui/ToolManager.h>

#include <GLMotif/Button.h>
#include <GLMotif/ToggleButton.h>
#include <GLMotif/Menu.h>
#include <GLMotif/PopupMenu.h>
#include <GLMotif/CascadeButton.h>
#include <GLMotif/FileSelectionDialog.h>
#include <GLMotif/WidgetManager.h>
#include <GLMotif/StyleSheet.h>

#include <Misc/ThrowStdErr.h>

#include "Netitor/GenericNode.h"
#include "NetVR/NetVR"

#include "class.h"
#include "styles.h"
#include "NodeInfoDialog.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

class NetworkApplication : public Vrui::Application { //, public FileHandler {
 private:
  class Locator:public Vrui::LocatorToolAdapter
    {
    private:
      NetworkApplication* application;
      bool active;
      Vrui::Point pos;

    public:
      Locator(Vrui::LocatorTool* sTool,NetworkApplication* sApplication);

      virtual void motionCallback(Vrui::LocatorTool::MotionCallbackData* cbData);
      virtual void buttonPressCallback(Vrui::LocatorTool::ButtonPressCallbackData* cbData);
      virtual void buttonReleaseCallback(Vrui::LocatorTool::ButtonReleaseCallbackData* cbData);
      virtual void glRenderAction(GLContextData &contextData) const;
    };

  typedef std::vector<Locator*> LocatorList;

 public:
  NetworkApplication(int &argc, char** &argv, char** &appDefaults);
  ~NetworkApplication();

  virtual void display(GLContextData& contextData) const;
  virtual void frame();

  void resetNavigationCallback(Misc::CallbackData* cbData);
  void quitCallback(Misc::CallbackData *cbData)
    {
#ifndef _OLD_VRUI
      Vrui::shutdown();
#endif
    }

  void openFileCallback(Misc::CallbackData* cbData);
  void layoutTogglesCallback(GLMotif::ToggleButton::ValueChangedCallbackData* cbData);
  void resetLayoutCallback(Misc::CallbackData* cbData);
  void viewTogglesCallback(GLMotif::ToggleButton::ValueChangedCallbackData* cbData);
  void selectionTogglesCallback(GLMotif::ToggleButton::ValueChangedCallbackData* cbData);
  void mainMenuTogglesCallback(GLMotif::ToggleButton::ValueChangedCallbackData* cbData);


  virtual void toolCreationCallback(Vrui::ToolManager::ToolCreationCallbackData* cbData);
  virtual void toolDestructionCallback(Vrui::ToolManager::ToolDestructionCallbackData* cbData);

 protected:
  virtual void fileOpenAction(GLMotif::FileSelectionDialog::OKCallbackData*);
  virtual void fileCancelAction(GLMotif::FileSelectionDialog::CancelCallbackData*);

 private:
  LocatorList locators;

  GLMotif::PopupMenu* mainMenu;
  GLMotif::PopupMenu* createMainMenu();

  GLMotif::ToggleButton* degreeWeightedNodesButton;

  NetVR::NodeStyleSelector *defaultNodeWeight;

  Netitor::Network *mainNetwork;
  Netitor::Network *visNetwork;
  Netitor::NetworkSet components;
  Netitor::Network *maxNetwork;
  NetVR::Network   *phyNet;

  Netitor::Network* singleSelect;
  Netitor::Network* multiSelect;
  Netitor::Network* startSelect;
  Netitor::Network* endSelect;
  Netitor::Network* pathSelect;

  bool moving;

  void initializeNetwork(void);

  GLMotif::FileSelectionDialog* fileDialog;
  NodeInfoDialog* nodeInfoDialog;

  GLMotif::Popup* createLayoutTogglesMenu();
  GLMotif::Popup* createViewTogglesMenu();
  GLMotif::Popup* createSelectionTogglesMenu();

  std::vector<GLMotif::ToggleButton*> viewToggleButtons;
  std::vector<GLMotif::ToggleButton*> selectionToggleButtons;
};


#endif
