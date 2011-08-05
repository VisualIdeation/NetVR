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

#ifndef DIRECTORY_DIALOG_H
#define DIRECTORY_DIALOG_H

#include "CaveDialog.h"
#include "Directory.h"
#include <GLMotif/StyleSheet.h>
#include <GLMotif/PopupWindow.h>
#include <GLMotif/RowColumn.h>
#include <GLMotif/TextField.h>
#include <GLMotif/Button.h>
#include <Misc/CallbackData.h>

#include <iostream>
#include <vector>
#include <string>

class DirectoryDialog;

class FileHandler { 
    friend class DirectoryDialog;
  public:
    virtual ~FileHandler() { }
  protected:
    virtual void fileOpenAction(std::string) = 0; 
};

class DirectoryDialog:public Dialog {    
  public:
    DirectoryDialog( GLMotif::PopupMenu *parentMenu,
                     FileHandler *handler,
                     const char *directory=".",
                     std::string ext = std::string() )       
        : Dialog(parentMenu), handler(handler)
        { 
            dialogWindow = createDialog();

            if (!ext.empty())
	            dir.addExtensionFilter(ext);
            dir.read(directory);
            dir.sort();
            files = dir.contents();
            currentFile = files.begin();
            selectedFileTextField->setLabel((*currentFile).c_str());

            dir.printDirectory();
        }
        virtual ~DirectoryDialog() { }
        
  protected:
        GLMotif::PopupWindow* createDialog(void) 
            {
                const GLMotif::StyleSheet& ss=*Vrui::getWidgetManager()->getStyleSheet();
                
                GLMotif::PopupWindow* directoryDialogPopup=new GLMotif::PopupWindow("DirectoryDialogPopup",Vrui::getWidgetManager(),"Open File",ss.font);
                
                GLMotif::RowColumn* directoryDialog=new GLMotif::RowColumn("DirectoryDialog",directoryDialogPopup,false);
                directoryDialog->setOrientation(GLMotif::RowColumn::VERTICAL);
                directoryDialog->setNumMinorWidgets(3);
                directoryDialog->setMarginWidth(0.0f);
                directoryDialog->setSpacing(ss.size);

                GLMotif::Button *previousFileButton=new GLMotif::Button("PreviousFileButton",directoryDialog,"PREV",ss.font);
                previousFileButton->getSelectCallbacks().add(this, &DirectoryDialog::prevToggleCallback);
                
                
                //selectedFileTextField = new GLMotif::TextField("SelectedFileTextField",directoryDialog,ss.font,ss.fontHeight*6.0f);
                selectedFileTextField = new GLMotif::TextField("SelectedFileTextField",directoryDialog,ss.font,20);
            
                GLMotif::Button *nextFileButton=new GLMotif::Button("NextFileButton",directoryDialog,"NEXT",ss.font);
                nextFileButton->getSelectCallbacks().add(this, &DirectoryDialog::nextToggleCallback);
                
                GLMotif::Button *quitButton=new GLMotif::Button("QuitButton",directoryDialog,"Cancel",ss.font);
                quitButton->getSelectCallbacks().add(this, &DirectoryDialog::rejectToggleCallback);
                
                new GLMotif::Label("Spacer1",directoryDialog,"",ss.font);
                GLMotif::Button *acceptButton=new GLMotif::Button("AcceptButton",directoryDialog,"Open",ss.font);
                acceptButton->getSelectCallbacks().add(this, &DirectoryDialog::acceptToggleCallback);
                
                directoryDialog->manageChild();
                return directoryDialogPopup;
            }
        
        void nextToggleCallback(Misc::CallbackData *cbData) 
        {              
            if(currentFile != (files.end()-1))
                currentFile++;
            selectedFileTextField->setLabel((*currentFile).c_str());
        }
        void prevToggleCallback(Misc::CallbackData *cbData) 
        { 
            if(currentFile != files.begin())
                currentFile--;
            selectedFileTextField->setLabel((*currentFile).c_str());
        }        
        void acceptToggleCallback(Misc::CallbackData *cbData)
        {
            //(*openAction)(*currentFile);
            handler->fileOpenAction(*currentFile);
            currentFile = files.begin(); 
            selectedFileTextField->setLabel((*currentFile).c_str());
            hide();
        }
        void rejectToggleCallback(Misc::CallbackData *cbData)
        {
            currentFile = files.begin(); 
            selectedFileTextField->setLabel((*currentFile).c_str());
            hide();
        }        
       
  private:        
        Directory dir;
        
        GLMotif::TextField *selectedFileTextField;        
        std::vector<std::string> files;
        std::vector<std::string>::iterator currentFile;

        FileHandler *handler;
};


#endif
