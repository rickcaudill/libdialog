#include "mainwindow.h"
#include "messages.h"

#include <dialog/alert.h>
#include <dialog/standardwindow.h>
#include <dialog/inputbox.h>
#include <dialog/splash.h>
#include <dialog/fontrequester.h>
#include <dialog/colorrequester.h>
#include <dialog/action.h>

#include <gui/image.h>
#include <gui/view.h>
#include <gui/splitter.h>

#include <util/resources.h>
#include <util/resource.h>

#include "mainview.h"
#include "common.h"

MainWindow::MainWindow() : dialog::StandardWindow( os::Rect( 0, 0, 700, 300 ), "main_wnd", "Dialog Test" )
{
	/* Set Icon */
	os::Resources cCol( get_image_id() );
	os::ResStream *pcStream = cCol.GetResourceStream( "icon48x48.png" );
	os::BitmapImage *pcIcon = new os::BitmapImage( pcStream );
	SetIcon( pcIcon->LockBitmap() );
	delete( pcIcon );
	
	SetupActions();
	SetupMenus();
	SetupToolBar();
	SetupMainView();
	SetupStatusBar();
}

void MainWindow::HandleMessage( os::Message* pcMessage )
{
	switch( pcMessage->GetCode() )
	{
		case M_APP_QUIT:
		{
			if (!IsEnabled())
			{
				return;
			}			
			PostMessage( os::M_QUIT );
			break;
		}

		case M_TOGGLE_MENU:
		{
			ShowMenus(!WillShowMenus());
			break;
		}
		case M_TOGGLE_TOOLBAR:
		{
			try
			{
				bool toggle = !WillShowToolBar();
				ShowToolBar(toggle);
				GetActionList().FindAction("Views","toggle_toolbar").SetVisible(toggle);
			}
			catch(...)
			{
			}
			break;
		}
		
		case M_TOGGLE_STATUSBAR:
		{
			try
			{
				bool toggle = !WillShowStatusBar();
				ShowStatusBar(toggle);
				printf("%d\n",GetActionList().FindAction("Views","toggle_statusbar").SetVisible(toggle).IsVisible());
			}
			catch(...)
			{
			}
			
			break;
		}

	
		case M_FONT_REQUESTER:
		{
			if (!IsEnabled())
			{
				return;
			}			
			dialog::FontRequester* f = new dialog::FontRequester(new os::Messenger(this));
			f->Show();
			break;
		}
		case M_LOAD_REQUESTER:
		{
			break;
		}
		case M_SAVE_REQUESTER:
		{
			break;
		}

		case M_INPUT_REQUESTER:
		{
			if (!IsEnabled())
			{
				return;
			}			
			dialog::InputBox* b = new dialog::InputBox();
			b->Show();
			b->MakeFocus();
			break;
		}
		
		case M_SPLASH_REQUESTER:
		{
			if (!IsEnabled())
			{
				return;
			}			
			dialog::Splash* s = new dialog::Splash();
			s->Go();
			break;
		}
		
		case M_COLOR_REQUESTER:
		{
			if (!IsEnabled())
			{
				return;
			}			
			dialog::ColorRequester* c = new dialog::ColorRequester(new os::Messenger(this));
			c->Show();
			c->MakeFocus();
			break;
		}
		case M_INFO_ALERT:
		{
			if (!IsEnabled())
			{
				return;
			}			
			alert = new dialog::Alert("Info...","This is an info alert...",dialog::Alert::ALERT_INFO,0,"OK",NULL);
			alert->Go();
			break;
		}
		case M_QUESTION_ALERT:
		{
			if (!IsEnabled())
			{
				return;
			}			
			alert = new dialog::Alert("Question...","This is a question alert...\nWould you like to close?",dialog::Alert::ALERT_QUESTION,0,"YES","NO","Really?","Okay?","Huh?","Why Not?","What?","Why?","Who?",NULL);
			alert->Go();			
			break;
		}
		case M_TIP_ALERT:
		{
			if (!IsEnabled())
			{
				return;
			}
			alert = new dialog::Alert("Info...","This is a tip alert...",dialog::Alert::ALERT_TIP,0,"OK",NULL);
			alert->Go();			
			break;
		}
		case M_WARNING_ALERT:
		{
			if (!IsEnabled())
			{
				return;
			}
			alert = new dialog::Alert("Warning...","This is a warning alert...",dialog::Alert::ALERT_WARNING,0,"OK",NULL);
			alert->Go();			
			break;
		}
		
		case M_ERROR_ALERT:
		{
			if (!IsEnabled())
			{
				return;
			}
			alert = new dialog::Alert("Error...","This is an error alert...",dialog::Alert::ALERT_ERROR,0,"OK",NULL);
			alert->Go();			
			break;			
		}
		
		case M_CUSTOM_ALERT:
		{
			os::BitmapImage* image = GetImage("dialog-custom.png");
			alert = new dialog::Alert("Custom...","This is a custom alert...",image,0,"OK",NULL);
			alert->Go(new os::Invoker());	
			break;
		}
		default:
		{
			StandardWindow::HandleMessage(pcMessage);
			break;
		}
	}
}

bool MainWindow::OkToQuit()  // Obsolete?
{
	os::Application::GetInstance()->PostMessage( os::M_QUIT );
	return( true );
}

void MainWindow::SetupActions()
{
	
	AddAction(dialog::Action("Application","about",M_APP_QUIT).SetLabel("_About").SetTip("About the application").SetShortcut("CTRL+A").SetShortLabel("").SetImage(NULL));
	AddAction(dialog::Action("Application","exit",M_APP_QUIT).SetLabel("_Exit").SetTip("Exit the application").SetShortcut("CTRL+Q").SetShortLabel("").SetImage(NULL));

	/*Views Menu/Shortcuts */
	AddAction(dialog::Action("Views","toggle_menu",M_TOGGLE_MENU).SetLabel("Show _Menu").SetShortcut("CTRL+M").SetTip("").SetShortLabel("").SetImage(NULL).SetEnable(false).SetCheckable(true).SetChecked(true));
	AddAction(dialog::Action("Views","toggle_toolbar",M_TOGGLE_TOOLBAR).SetLabel("Show _Toolbar").SetShortcut("CTRL+T").SetTip("").SetShortLabel("").SetImage(NULL).SetEnable(true).SetCheckable(true).SetChecked(true));
	AddAction(dialog::Action("Views","toggle_statusbar",M_TOGGLE_STATUSBAR).SetLabel("Show _Statusbar").SetShortcut("CTRL+S").SetTip("").SetShortLabel("").SetImage(NULL).SetEnable(true).SetCheckable(true).SetChecked(true));


	/*Dialogs Menu/ToolBar*/
	AddAction(dialog::Action("Dialogs","fontrequester",M_FONT_REQUESTER).SetLabel("Font Requester...").SetTip("").SetShortcut("").SetShortLabel("Font...").SetImage(GetImage("font.png")));
	AddAction(dialog::Action("Dialogs","filerequester-load",M_LOAD_REQUESTER).SetLabel("File Requester - Load...").SetTip("").SetShortcut("").SetShortLabel("Load...").SetImage(GetImage("load.png")));
	AddAction(dialog::Action("Dialogs","filerequester-save",M_SAVE_REQUESTER).SetLabel("File Requester - Save...").SetTip("").SetShortcut("").SetShortLabel("Save...").SetImage(GetImage("save.png")));
	AddAction(dialog::Action("Dialogs","inputbox",M_INPUT_REQUESTER).SetLabel("Input box...").SetTip("").SetShortcut("").SetShortLabel("Input...").SetImage(GetImage("input.png")));
	AddAction(dialog::Action("Dialogs","splashscreen",M_SPLASH_REQUESTER).SetLabel("Splash Screen...").SetTip("").SetShortcut("").SetShortLabel("Splash...").SetImage(GetImage("colors.png")));
	AddAction(dialog::Action("Dialogs","colorrequester",M_COLOR_REQUESTER).SetLabel("Color Requester...").SetTip("").SetShortcut("").SetShortLabel("Color...").SetImage(GetImage("colors.png")));
	
	/*Alerts Menu/ToolBar*/
	AddAction(dialog::Action("Alerts","info",M_INFO_ALERT).SetLabel("Info..").SetTip("").SetShortcut("").SetShortLabel("Info...").SetImage(GetImage("dialog-information.png")));
	AddAction(dialog::Action("Alerts","question",M_QUESTION_ALERT).SetLabel("Question...").SetTip("").SetShortcut("").SetShortLabel("Ques...").SetImage(GetImage("dialog-question.png")));
	AddAction(dialog::Action("Alerts","tip",M_TIP_ALERT).SetLabel("Tip...").SetTip("").SetShortcut("").SetShortLabel("Tip...").SetImage(GetImage("dialog-information.png")));
	AddAction(dialog::Action("Alerts","warning",M_WARNING_ALERT).SetLabel("Warning...").SetTip("").SetShortcut("").SetShortLabel("Warn...").SetImage(GetImage("dialog-warning.png")));
	AddAction(dialog::Action("Alerts","error",M_ERROR_ALERT).SetLabel("Error...").SetTip("").SetShortcut("").SetShortLabel("Err").SetImage(GetImage("dialog-warning.png")));
	AddAction(dialog::Action("Alerts","custom",M_CUSTOM_ALERT).SetLabel("Custom - Multiple Buttons...").SetTip("").SetShortcut("").SetShortLabel("Custom...").SetImage(GetImage("dialog-custom.png")));
	

	AddAction(dialog::Action("Actions","as_menu",M_ACTION_ASMENU).SetLabel("As Menu...").SetTip("").SetShortcut("").SetShortLabel("").SetImage(NULL));
	AddAction(dialog::Action("Actions","as_iconview",M_ACTION_ASMENU).SetLabel("As IconView...").SetTip("").SetShortcut("").SetShortLabel("").SetImage(NULL));
	AddAction(dialog::Action("Actions","as_dropdown",M_ACTION_ASMENU).SetLabel("As Dropdown...").SetTip("").SetShortcut("").SetShortLabel("").SetImage(NULL));
	AddAction(dialog::Action("Actions","as_listview",M_ACTION_ASMENU).SetLabel("As ListView...").SetTip("").SetShortcut("").SetShortLabel("").SetImage(NULL));

	SetCategoryDisplayName("Application","_Application");
	SetCategoryDisplayName("Views","_Views");
	SetCategoryDisplayName("Dialogs","_Dialogs");
	SetCategoryDisplayName("Alerts","_Alerts");
	SetCategoryDisplayName("Actions","A_ctions");
	SetCategoryDisplayOrder("Application","Views","Dialogs","Alerts",NULL);
	
	//SetActionGroup("Actions","as_menu","as_iconview","as_dropdown","as_listview","as_popupmenu",NULL);
	
	
	SetSeparatorAfter("Application","about",NULL);
	SetSeparatorBefore("Dialogs","splashscreen",NULL);
	SetSeparatorAfter("Dialogs","splashscreen",NULL);
}

void MainWindow::SetupMenus()
{
	os::Menu* m = GetMenu();
	GetActionList().AsMenu(m,true);
	m->SetTargetForItems(this);
}

void MainWindow::SetupToolBar()
{
	os::ToolBar* bar = GetToolBar();
	dialog::ActionList l = GetActionList().FilterTo("Dialogs","Alerts",NULL);
	l.AsToolBar(bar,true);
}

void MainWindow::SetupStatusBar()
{
	os::StatusBar* bar = GetStatusBar();
	bar->AddPanel("info","");
}

void MainWindow::SetupMainView()
{
	os::LayoutView* main = GetMainView();
	os::VLayoutNode* n = (os::VLayoutNode*) main->GetRoot();
	
	os::TextView* t = new os::TextView(os::Rect(),"dialog_text","");
	t->SetValue(ReadFileFromResource("description.txt"));
	
	t->SetMultiLine(true);
	t->SetReadOnly(true);
	t->SetEnable(false);


	MainView* v = new MainView();
	os::Splitter* s = new os::Splitter(os::Rect(),"splitter",t,v);

	n->AddChild(s);
	
}



