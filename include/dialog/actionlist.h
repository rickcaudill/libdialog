#ifndef __F_DIALOG__ACTIONLIST_H__
#define __F_DIALOG__ACTIONLIST_H__

#include <dialog/action.h>
#include <util/string.h>
#include <gui/dropdownmenu.h>
#include <gui/popupmenu.h>
#include <gui/treeview.h>
#include <gui/listview.h>
#include <gui/iconview.h>
#include <gui/toolbar.h>

#include <set>
#include <map>

namespace dialog
{
	class ActionList
	{
		public:
			ActionList();
			~ActionList();
			
			ActionList( ActionList& other );
			ActionList( const ActionList& other );
			
			ActionList& operator=(const ActionList&);
			ActionList& operator=(ActionList&);
			ActionList& operator=(ActionList);
			
			ActionList& AddAction(dialog::Action&);
			ActionList& AddAction(const os::String& cCategory,const os::String& cName,const os::String& cLabel,int nMessageCode,const os::String& cTip="",const os::String& cShortcut="", const os::String& cShortLabel="",os::BitmapImage* image=NULL);
			
			os::DropdownMenu*	AsDropDownMenu(bool showCategories=true) const;
			os::PopupMenu*		AsPopupMenu(bool showCategories=true ) const;
			os::Menu*			AsMenu(os::Menu* parent=NULL,bool showImages=true);
			os::ListView*		AsListView(bool showCategories=true) const;
			os::TreeView*		AsTreeView(bool showCategories=true) const;
			os::IconView*		AsIconView(bool showCategories=true) const;
			os::ToolBar* 		AsToolBar(os::ToolBar* parent,bool showCategories=true,bool showShortLabel=true,bool showImages=true);

			dialog::ActionList FilterBy(const char* category,...);
			dialog::ActionList FilterTo(const char* category,...);
			dialog::ActionList FilterOut(const char* category,...);
			dialog::ActionList FilterOut(const os::String& category,const char* name,...);
			
			dialog::ActionList& SetCategoryDisplayName(const os::String&,const os::String&);
			dialog::ActionList& SetCategoryDisplayOrder(const char* category,...);
			dialog::ActionList& SetCategoryDisplayOrder(const char* category,va_list);
			dialog::ActionList& SetCategoryDisplayOrder(std::list<os::String> categories);
			
			dialog::ActionList& SetSeparatorBefore(const os::String& category,const char* name,...);
			dialog::ActionList& SetSeparatorBefore(const os::String& category,const char* name,va_list);
			dialog::ActionList& SetSeparatorBefore(const os::String& category,std::list<os::String> names);
			bool HasSeparatorBefore(const os::String& category,const os::String& name);
			
			dialog::ActionList& SetSeparatorAfter(const os::String& category,const char* name,...);
			dialog::ActionList& SetSeparatorAfter(const os::String& category,const char* name,va_list);
			dialog::ActionList& SetSeparatorAfter(const os::String& category,std::list<os::String> names);
			bool HasSeparatorAfter(const os::String& category,const os::String& name);			
			
			dialog::Action& FindAction(const os::String& category,const os::String& name);

			std::list<os::String> GetCategories();
		private:
			std::map<os::String,std::list<os::String> > m_cSeparatorBeforeList;
			std::map<os::String,std::list<os::String> > m_cSeparatorAfterList;
			std::multimap<os::String,dialog::Action> m_cActionList;
			std::map<os::String,os::String> m_cCategoryDisplayList;
			std::list<os::String> m_cCategoryDisplayOrder;
			std::vector<os::LayoutNode*> m_cLayoutCleanup;
	};
}

#endif


