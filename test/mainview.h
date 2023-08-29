#ifndef __MAINVIEW_H__
#define __MAINVIEW_H__

#include <gui/layoutview.h>
#include <gui/dropdownmenu.h>
#include <gui/imagebutton.h>
#include <gui/view.h>

#include <dialog/actionlist.h>
#include <dialog/action.h>

class MainView : public os::LayoutView
{
public:
	MainView();
private:
	os::VLayoutNode* root;
	os::ImageButton* addButton;
	os::DropdownMenu* menu;
	
	dialog::ActionList list;
};

#endif

