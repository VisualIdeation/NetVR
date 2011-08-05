#ifndef __NODEINFODIALOG_H
#define __NODEINFODIALOG_H

#include <map>
#include <vector>

#include <GLMotif/TextField.h>

#include "CaveDialog.h"

class NodeInfoDialog : public Dialog
{
public:
	// storing AttributeMap as a workaround for createDialog not taking any arguments
	NodeInfoDialog(GLMotif::PopupMenu *parentMenu, const XML::AttributeMap& attrs) : Dialog(parentMenu), attrs(attrs)
    {
    	dialogWindow = createDialog();
    }

    virtual ~NodeInfoDialog() {}

    void update(const XML::AttributeMap& attrs)
    {
		for(XML::AttributeMap::const_iterator attr = attrs.begin(); attr != attrs.end(); attr++)
		{
			std::string key = (*attr).first;
			std::string value = (*attr).second;

			GLMotif::TextField* tf = textFieldMap[key];
			tf->setLabel(value.c_str());
		}
    }

protected:
	GLMotif::PopupWindow* createDialog()
	{
		const GLMotif::StyleSheet& ss = *Vrui::getWidgetManager()->getStyleSheet();

		GLMotif::PopupWindow* nodeInfoDialogPopup = new GLMotif::PopupWindow("NodeInfoDialogPopup",Vrui::getWidgetManager(),"Node Data",ss.font);

		GLMotif::RowColumn* nodeInfoDialog = new GLMotif::RowColumn("NodeInfoDialog",nodeInfoDialogPopup,false);
		nodeInfoDialog->setOrientation(GLMotif::RowColumn::VERTICAL);
		nodeInfoDialog->setNumMinorWidgets(2);
		nodeInfoDialog->setMarginWidth(0.0f);
		nodeInfoDialog->setSpacing(ss.size);
		
		std::cout << "# of attributes: " << attrs.size() << std::endl;

		for(XML::AttributeMap::const_iterator attr = attrs.begin(); attr != attrs.end(); attr++)
		{
			std::string key = (*attr).first;
			std::string value = (*attr).second;

			new GLMotif::Label(key.c_str(), nodeInfoDialog, key.c_str(), ss.font);
			GLMotif::TextField* tf = new GLMotif::TextField(value.c_str(), nodeInfoDialog, ss.font, 20);

			textFieldVector.push_back(tf);
			textFieldMap.insert(std::make_pair(key, tf));
		}

		nodeInfoDialog->manageChild();
		return nodeInfoDialogPopup;
	}

private:
	std::vector<GLMotif::TextField*> textFieldVector;
	std::map<std::string, GLMotif::TextField*> textFieldMap;
	XML::AttributeMap attrs;
};

#endif
