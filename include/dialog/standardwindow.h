#ifndef __F_DIALOG_STANDARD_WINDOW_H__
#define __F_DIALOG_STANDARD_WINDOW_H__

#include <gui/window.h>
#include <gui/menu.h>
#include <gui/toolbar.h>
#include <gui/statusbar.h>
#include <util/event.h>
#include <dialog/actionlist.h>
#include <dialog/action.h>
#include <vector>

using namespace os;

namespace dialog
{
	class StandardWindow : public os::Window
	{
		public:
			StandardWindow(const os::Rect&,const os::String& cName,const os::String& cTitle);
			~StandardWindow();
			
			void			Show( bool bMakeVisible = true );
			bool			OkToQuit();
			void			HandleMessage(os::Message*);
			virtual bool	Validate();
			virtual void	Started();
					
			void ShowStatusBar(bool status);
			bool WillShowStatusBar() const;
			
			void ShowMenus(bool menu);
			bool WillShowMenus() const;
			
			void ShowToolBar( bool toolbar);
			bool WillShowToolBar() const;

			void AddAction(dialog::Action&);
			void SetCategoryDisplayName(const os::String& c,const os::String& n);

			void Layout();
			
			virtual void SetupActions()=0;
			virtual void SetupMenus()=0;
			virtual void SetupToolBar()=0;
			virtual void SetupStatusBar()=0;
			virtual void SetupMainView()=0;
			
			os::Menu* GetMenu() const;
			void SetMenu(os::Menu* menu);
			
			os::ToolBar* GetToolBar() const;
			void SetToolBar(os::ToolBar*);
			
			os::StatusBar* GetStatusBar() const;
			os::LayoutView* GetMainView() const;
			os::Point GetPreferredSize() const;
			dialog::ActionList& GetActionList();
			
			void SetCategoryDisplayOrder(const char* category,...);		
			void SetSeparatorBefore(const os::String& category,const char* name,...);
			void SetSeparatorAfter(const os::String& category,const char* name,...);

			bool IsEnabled() const;
			
		private:
			void _UpdateWindows(os::Message*);
		private:
			enum
			{
				ID_DIALOG_WINDOW_EV=36193515
			};
			os::Menu* m_pcMenu;
			os::ToolBar* m_pcToolBar;
			os::StatusBar* m_pcStatusBar;
			os::LayoutView* m_pcMainView;
			os::LayoutView* m_pcView;
			
			os::Event* m_pcWindowEv;
			std::vector<os::String> m_cModalDialogs;
			
			bool m_bEnabled;
			
			dialog::ActionList m_cActionList;
	};
	
}	
#endif










