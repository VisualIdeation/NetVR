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

#ifndef CAVE_DIALOG_H
#define CAVE_DIALOG_H

#include <Vrui/Vrui.h>
#include <GLMotif/WidgetManager.h>
#include <GLMotif/PopupMenu.h>
#include <GLMotif/PopupWindow.h>

class Dialog {
public:
    enum State { ACTIVE, HIDDEN };

    Dialog(GLMotif::PopupMenu *parentMenu) 
        : parent(parentMenu), dialogWindow(0), CurrentState(HIDDEN) 
        { 
            //dialogWindow=createDialog();
        }
    virtual ~Dialog()
        {
            if(dialogWindow)
                delete dialogWindow;
        }
    State state() const 
        {
            return CurrentState;
        }    
    void show() 
        {           
            if(CurrentState!=ACTIVE) {
                Vrui::getWidgetManager()->popupPrimaryWidget(dialogWindow,Vrui::getWidgetManager()->calcWidgetTransformation(parent));
                CurrentState=ACTIVE;
            }
        }
    void hide() 
        {    
            if(CurrentState!=HIDDEN) {
                Vrui::popdownPrimaryWidget(dialogWindow);       
                CurrentState=HIDDEN;
            }
        }
    
protected:
    virtual GLMotif::PopupWindow* createDialog() = 0;
    
    GLMotif::PopupMenu   *parent;
    GLMotif::PopupWindow *dialogWindow;
    State CurrentState;
};

#endif
