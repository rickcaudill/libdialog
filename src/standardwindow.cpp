
#include <dialog/standardwindow.h>
#include <util/application.h>

using namespace dialog;
using namespace os;

StandardWindow::StandardWindow(const os::Rect& cRect,const os::String& cName,const os::String& cTitle) : Window(cRect,cName,cTitle)
{
	m_pcView = new os::LayoutView(GetBounds(),"standard_layoutview",NULL,CF_FOLLOW_ALL);
	
	/*initialize default components*/
	m_pcMenu = new Menu(Rect(),"main_menu",ITEMS_IN_ROW,CF_FOLLOW_LEFT | CF_FOLLOW_RIGHT);
	m_pcToolBar = new os::ToolBar(os::Rect(),"toolbar",CF_FOLLOW_LEFT | CF_FOLLOW_RIGHT);
	m_pcMainView = new os::LayoutView(os::Rect(),"view",NULL,CF_FOLLOW_ALL);
	m_pcMainView->SetRoot(new os::VLayoutNode("mainview_node"));
	m_pcStatusBar = new os::StatusBar(os::Rect(),"statusbar",CF_FOLLOW_BOTTOM);

	AddChild(m_pcView);

	m_pcWindowEv = new os::Event();
	m_pcWindowEv->SetToRemote( "os/Window/", -1 );
	m_pcWindowEv->SetMonitorEnabled( true, this, ID_DIALOG_WINDOW_EV );
	
	m_cModalDialogs.clear();	
	m_bEnabled = true;
}

StandardWindow::~StandardWindow()
{
}

void StandardWindow::Started()
{
}

bool StandardWindow::OkToQuit()
{
	return Validate();
}

void StandardWindow::Show(bool bVisible)
{
	Layout();
	Window::Show(bVisible);
}

void StandardWindow::Layout()
{
	os::VLayoutNode* root = new os::VLayoutNode("root");
	root->SetBorders(os::Rect(0,0,0,0));
	
	if (m_pcMenu->IsVisible())
	{
		root->AddChild(m_pcMenu,0.05);
		root->AddChild(new os::VLayoutSpacer("",1,1));
	}
	
	if (m_pcToolBar->IsVisible())
	{
		root->AddChild(m_pcToolBar,0.1);
		root->AddChild(new os::VLayoutSpacer("",1,1));
	}
	root->AddChild(m_pcMainView,0.84);
	root->AddChild(new os::VLayoutSpacer("",1,1));		
	
	
	if (m_pcStatusBar->IsVisible())
	{
		root->AddChild(m_pcStatusBar,0.01);
		root->AddChild(new os::VLayoutSpacer("",1,1));
	}
	m_pcView->SetRoot(root);
	m_pcView->InvalidateLayout();
}

bool StandardWindow::Validate()
{
	return true;
}
			
void StandardWindow::ShowStatusBar(bool status)
{
	m_pcStatusBar->Show(status);
	Layout();
}

bool StandardWindow::WillShowStatusBar() const
{
	return m_pcStatusBar->IsVisible();
}
			
void StandardWindow::ShowMenus(bool menu)
{
	m_pcMenu->Show(menu);
	Layout();
}

bool StandardWindow::WillShowMenus() const
{
	return m_pcMenu->IsVisible();
}
			
void StandardWindow::ShowToolBar( bool toolbar)
{
	m_pcToolBar->Show(toolbar);
	Layout();
}

bool StandardWindow::WillShowToolBar() const
{
	return m_pcToolBar->IsVisible();
}

os::Menu* StandardWindow::GetMenu() const
{
	return m_pcMenu;
}

void StandardWindow::SetMenu(os::Menu* menu)
{
	m_pcMenu = menu;
	Layout();
}

os::ToolBar* StandardWindow::GetToolBar() const
{
	return m_pcToolBar;
}

void StandardWindow::SetToolBar(os::ToolBar* bar)
{
	m_pcToolBar = bar;
	Layout();
}

os::StatusBar* StandardWindow::GetStatusBar() const
{
	return m_pcStatusBar;
}

os::LayoutView* StandardWindow::GetMainView() const
{
	return m_pcMainView;
}

dialog::ActionList& StandardWindow::GetActionList()
{
	return m_cActionList;
}

os::Point StandardWindow::GetPreferredSize() const
{
	return m_pcView->GetPreferredSize(false);
}


void StandardWindow::HandleMessage(os::Message* pcMessage)
{
	switch (pcMessage->GetCode())
	{
		case ID_DIALOG_WINDOW_EV:
		{
			_UpdateWindows(pcMessage);
			
			m_bEnabled = m_cModalDialogs.size() == 0 ? true : false;
			break;
		}
		default:
			Window::HandleMessage(pcMessage);
			break;
	}
}

bool StandardWindow::IsEnabled() const
{
	return m_bEnabled;
}

void StandardWindow::AddAction(dialog::Action& a)
{
	m_cActionList.AddAction(a);
}

void StandardWindow::SetCategoryDisplayName(const os::String& c,const os::String& n)
{
	m_cActionList.SetCategoryDisplayName(c,n);
}

void StandardWindow::SetCategoryDisplayOrder(const char* category,...)
{
	va_list args;
	va_start(args,category);
	
	std::list<os::String> categories;
	for (const char* c = category; c!=NULL; c = va_arg(args,const char *) )
	{
		categories.push_back(c);
	}
	categories.unique();
	
	m_cActionList.SetCategoryDisplayOrder(categories);
	va_end(args);
}

void StandardWindow::SetSeparatorBefore(const os::String& category,const char* name,...)
{
	va_list args;
	va_start(args,name);
	
	std::list<os::String> names;
	for (const char* c = name; c!=NULL; c = va_arg(args,const char *) )
	{
		names.push_back(c);
	}
	names.unique();
	
	m_cActionList.SetSeparatorBefore(category,names);
	va_end(args);
}

void StandardWindow::SetSeparatorAfter(const os::String& category,const char* name,...)
{
	va_list args;
	va_start(args,name);
	
	std::list<os::String> names;
	for (const char* c = name; c!=NULL; c = va_arg(args,const char *) )
	{
		names.push_back(c);
	}
	names.unique();
	
	m_cActionList.SetSeparatorAfter(category,names);
	va_end(args);
}

void StandardWindow::_UpdateWindows( os::Message* pcMessage )
{
	bool bDummy;
	int64 nFlags;
	bool bVisible;
	os::String zID;
	
	os::String thisApp = os::Application::GetInstance()->GetName();
	os::String zApp;
	
	pcMessage->FindString( "application", &zApp );
	pcMessage->FindString("id",&zID);

	/*Note: This will only work if there is one application named of this application*/
	/*Note: This now will only work after the first window is shown*/
	if( pcMessage->FindBool( "event_registered", &bDummy ) != 0 )
	{
		if (zApp == thisApp)
		{
			if (pcMessage->FindInt64("flags",&nFlags) == 0)
			{
				if (nFlags & WND_MODAL)
				{
					if( pcMessage->FindBool( "visible", &bVisible ) == 0 )
					{
						if (bVisible)
						{
							m_cModalDialogs.push_back(zID);
						}
					}
				}
			}
		}
		else
		{
			m_cModalDialogs.erase(std::remove(m_cModalDialogs.begin(),m_cModalDialogs.end(),zID),m_cModalDialogs.end());
		}
	}
}






