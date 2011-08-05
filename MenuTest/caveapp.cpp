#include <Vrui/Vrui.h>
#include <Vrui/Application.h>
#include <Geometry/OrthogonalTransformation.h>
#include <GL/gl.h>
#include <GLMotif/Button.h>
#include <GLMotif/ToggleButton.h>
#include <GLMotif/CascadeButton.h>
#include <GLMotif/Menu.h>
#include <GLMotif/PopupMenu.h>
#include <GLMotif/Popup.h>
#include <GL/GLContextData.h>
#include <GLMotif/StyleSheet.h>
#include <GLMotif/RowColumn.h>
#include <GLMotif/WidgetManager.h>

#include <vector>
#include <string>
#include <iostream>

class CaveApp:public Vrui::Application {
public:
    CaveApp(int &argc, char** &argv, char** &appDefaults);
    ~CaveApp();

    virtual void display(GLContextData& contextData) const;
    virtual void frame();

    void resetNavigationCallback(Misc::CallbackData* cbData);

private:
    GLMotif::PopupMenu* mainMenu;
    GLMotif::PopupMenu* createMainMenu();

    GLMotif::Popup* createLayoutTogglesMenu();
    GLMotif::Popup* createViewTogglesMenu();
    GLMotif::Popup* createSelectionTogglesMenu();

    void viewTogglesCallback(GLMotif::ToggleButton::ValueChangedCallbackData *cbData) 
        {
            std::string name = cbData->toggle->getName();
            //std::cout << name << std::endl;

            std::vector<GLMotif::ToggleButton*>::iterator button;
            for(button=viewToggleButtons.begin(); button!=viewToggleButtons.end(); ++button)
                if(name != (*button)->getName())
                    (*button)->setToggle(false);
        }
    void selectionTogglesCallback(GLMotif::ToggleButton::ValueChangedCallbackData *cbData) 
        {
            std::string name = cbData->toggle->getName();
            //std::cout << name << std::endl;

            std::vector<GLMotif::ToggleButton*>::iterator button;
            for(button=selectionToggleButtons.begin(); button!=selectionToggleButtons.end(); ++button)
                if(name != (*button)->getName())
                    (*button)->setToggle(false);
                else
                    (*button)->setToggle(true);
        }


    std::vector<GLMotif::ToggleButton*> viewToggleButtons;
    std::vector<GLMotif::ToggleButton*> selectionToggleButtons;
};


GLMotif::Popup* CaveApp::createLayoutTogglesMenu() { 

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
    GLMotif::Button* forceDirectedButton=new GLMotif::Button("ForceDirectedButton",
                                                             layoutTogglesMenu,
                                                             "Forced Directed Layout",
                                                             ss.font);
    GLMotif::Button* heirarchalButton=new GLMotif::Button("HeirarchalButton",
                                                          layoutTogglesMenu,
                                                          "Heirarchal Layout",
                                                          ss.font);
    GLMotif::ToggleButton* degreeWeightedNodesButton=new GLMotif::ToggleButton("DegreeWeightedNodesButton",
                                                                   layoutTogglesMenu,
                                                                   "Node Charge By Degree",
                                                                   ss.font);
    degreeWeightedNodesButton->setToggle(false);

    GLMotif::Button* resetLayoutButton=new GLMotif::Button("ResetLayoutButton",
                                                           layoutTogglesMenu,
                                                           "Reset Layout",
                                                           ss.font);

    layoutTogglesMenu->manageChild();

    return layoutTogglesMenuPopup;
}

GLMotif::Popup* CaveApp::createViewTogglesMenu() { 

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
    showAllToggleButton->getValueChangedCallbacks().add(this, &CaveApp::viewTogglesCallback);

    GLMotif::ToggleButton* showLargestToggleButton=new GLMotif::ToggleButton("ShowLargestToggleButton",
                                                                             viewTogglesMenu,
                                                                             "Largest Component",
                                                                             ss.font);
    showLargestToggleButton->getValueChangedCallbacks().add(this, &CaveApp::viewTogglesCallback);

    GLMotif::ToggleButton* showSelectedToggleButton=new GLMotif::ToggleButton("ShowSelectedToggleButton",
                                                                              viewTogglesMenu,
                                                                              "Selected Component(s)",
                                                                              ss.font);
    showSelectedToggleButton->getValueChangedCallbacks().add(this, &CaveApp::viewTogglesCallback);
    
    viewToggleButtons.push_back(showAllToggleButton);
    viewToggleButtons.push_back(showLargestToggleButton);
    viewToggleButtons.push_back(showSelectedToggleButton);

    viewTogglesMenu->manageChild();

    return viewTogglesMenuPopup;
}

GLMotif::Popup* CaveApp::createSelectionTogglesMenu() { 

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
                                                                                 "Single Select",
                                                                                 ss.font);
    singleSelectionToggleButton->getValueChangedCallbacks().add(this, &CaveApp::selectionTogglesCallback);

    GLMotif::ToggleButton* multiSelectionToggleButton=new GLMotif::ToggleButton("MultiSelctionToggleButton",
                                                                                selectionTogglesMenu,
                                                                                "Multi Select",
                                                                                ss.font);
    multiSelectionToggleButton->getValueChangedCallbacks().add(this, &CaveApp::selectionTogglesCallback);

    selectionToggleButtons.push_back(singleSelectionToggleButton);
    selectionToggleButtons.push_back(multiSelectionToggleButton);

    selectionTogglesMenu->manageChild();

    return selectionTogglesMenuPopup;
}

GLMotif::PopupMenu* CaveApp::createMainMenu() {

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
    GLMotif::CascadeButton* layoutTogglesCascade=new GLMotif::CascadeButton("LayoutTogglesCascade",
                                                                            mainMenu,
                                                                            "Layouts",
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

    GLMotif::Button* resetNavigationButton=new GLMotif::Button("ResetNavigationButton",mainMenu,"Reset Navigation",Vrui::getUiFont());
    resetNavigationButton->getSelectCallbacks().add(this,&CaveApp::resetNavigationCallback);

    GLMotif::Button* quitApplicationButton=new GLMotif::Button("QuitApplicationButton",mainMenu,"Quit",Vrui::getUiFont());
    
    mainMenu->manageChild();
    return mainMenuPopup;
}

CaveApp::CaveApp(int &argc, char** &argv, char** &appDefaults) 
 : Vrui::Application(argc, argv, appDefaults),
   mainMenu(0) {

    /* Create the user interface: */
    mainMenu=createMainMenu();
    
    /* Install the main menu: */
    Vrui::setMainMenu(mainMenu);
    
    /* Set the navigation transformation: */
    resetNavigationCallback(0); 
}

CaveApp::~CaveApp() { 
    
    delete mainMenu;
}

void CaveApp::frame(void) { }

void CaveApp::display(GLContextData& contextData) const { }

void CaveApp::resetNavigationCallback(Misc::CallbackData* cbData) {
    /* Reset the Vrui navigation transformation: */
    Vrui::Point modelCenter=Vrui::Point::origin; 
    Vrui::setNavigationTransformation(modelCenter, 25.0);
}

int main(int argc, char *argv[]) {

    char **appDefaults = 0;
    CaveApp app(argc, argv, appDefaults);
    app.run();
    
    return 0;
}
