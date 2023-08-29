#include "mainview.h"
#include "common.h"

#include <gui/image.h>
#include <util/resource.h>
#include <util/resources.h>


MainView::MainView() : os::LayoutView(os::Rect(),"mainview",NULL)
{
	root = new os::VLayoutNode("root");
	
	os::HLayoutNode* actionlist_node = new os::HLayoutNode("button");
	
	actionlist_node->AddChild(menu = new os::DropdownMenu(os::Rect(),"drop"),10);
	menu->SetReadOnly(true);
	menu->SetMaxPreferredSize(1650);

	os::HLayoutNode* button_node = new os::HLayoutNode("button_node");
	os::BitmapImage* b = GetImage("add.png");
	b->SetSize(os::Point(16,16));
	button_node->AddChild(addButton = new os::ImageButton(os::Rect(),"add","Add",NULL,b,false,false,false),1);
	button_node->AddChild(new os::ImageButton(os::Rect(),"remove","Remove",NULL,b,false,false,false),1);
	button_node->SameWidth("add","remove",NULL);
	
	actionlist_node->AddChild(button_node);
	actionlist_node->SameHeight("drop","add","remove",NULL);	
	root->AddChild(actionlist_node);
	
	//Add FilterView
	
	
	
	//Add ListView/DisplayView
	root->AddChild(new os::VLayoutSpacer("",100.0f,100.f));
	SetRoot(root);
}



