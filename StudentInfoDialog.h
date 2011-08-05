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

#ifndef STUDENT_INFO_DIALOG_H
#define STUDENT_INFO_DIALOG_H

#include "CaveDialog.h"

#include "class.h"

#include <string>

#include <iostream>
using std::cout;
using std::endl;

class StudentInfoDialog:public Dialog {
  public:
    StudentInfoDialog(GLMotif::PopupMenu *parentMenu) 
        : Dialog(parentMenu)
        {
            dialogWindow = createDialog();
        }
    virtual ~StudentInfoDialog() { }

    void update(const Student* student) 
        { 
            cout << "StudentInfoDialog::update()..." << endl;
            std::cout << "first: " << student->first << endl;
            std::cout << "last : " << student->last << endl;
            std::cout << "dept : "  << student->dept << endl;
            
            std::string name = student->first;
            name += " " + student->last;
            std::cout << "name: " << name << endl;
            
            
            nameTextField->setLabel(name.c_str());
            deptTextField->setLabel(student->dept.c_str());
        }

  protected:
    GLMotif::PopupWindow* createDialog() 
        {
            const GLMotif::StyleSheet& ss=*Vrui::getWidgetManager()->getStyleSheet();
            
            GLMotif::PopupWindow* nodeInfoDialogPopup=new GLMotif::PopupWindow("NodeInfoDialogPopup",Vrui::getWidgetManager(),"Node Data",ss.font);
            
            GLMotif::RowColumn* nodeInfoDialog=new GLMotif::RowColumn("NodeInfoDialog",nodeInfoDialogPopup,false);
            nodeInfoDialog->setOrientation(GLMotif::RowColumn::VERTICAL);
            nodeInfoDialog->setNumMinorWidgets(2);
            nodeInfoDialog->setMarginWidth(0.0f);
            nodeInfoDialog->setSpacing(ss.size);
    	    
	    //GLMotif::Label *nameLabel=
            new GLMotif::Label("NameLabel",nodeInfoDialog,"Name",ss.font);
            
            //nameTextField=new GLMotif::TextField("NameTextField",nodeInfoDialog,ss.font,ss.fontHeight*10.0f);
            nameTextField=new GLMotif::TextField("NameTextField",nodeInfoDialog,ss.font,20);
            
	    //GLMotif::Label *deptLabel=
            new GLMotif::Label("DeptLabel",nodeInfoDialog,"Dept",ss.font);
            	   
            //deptTextField=new GLMotif::TextField("DeptTextField",nodeInfoDialog,ss.font,ss.fontHeight*10.0f);
            deptTextField=new GLMotif::TextField("DeptTextField",nodeInfoDialog,ss.font,20);
            
            nodeInfoDialog->manageChild();
            return nodeInfoDialogPopup;
        }

  private:
    GLMotif::TextField *nameTextField;
    GLMotif::TextField *deptTextField;
};


#endif
