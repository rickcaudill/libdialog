#include <dialog/actionlist.h>
#include <algorithm>
#include <functional>   // for unary_function
#include <iterator>
#include <gui/exceptions.h>

using namespace dialog;

ActionList::ActionList()
{
	m_cActionList.clear();
	m_cCategoryDisplayList.clear();
	m_cCategoryDisplayOrder.clear();
	m_cSeparatorBeforeList.clear();
	m_cSeparatorAfterList.clear();
}

ActionList::ActionList( ActionList& other ) : m_cActionList(other.m_cActionList), m_cCategoryDisplayList(other.m_cCategoryDisplayList)
{
	m_cActionList.clear();
	m_cCategoryDisplayList.clear();
	m_cCategoryDisplayOrder.clear();
	m_cSeparatorBeforeList.clear();
	m_cSeparatorAfterList.clear();
	
	m_cActionList = other.m_cActionList;
	m_cCategoryDisplayList = other.m_cCategoryDisplayList;
	m_cCategoryDisplayOrder = other.m_cCategoryDisplayOrder;
	m_cSeparatorBeforeList = other.m_cSeparatorBeforeList;
	m_cSeparatorAfterList = other.m_cSeparatorAfterList;
}

ActionList::ActionList( const ActionList& other ) : m_cActionList(other.m_cActionList), m_cCategoryDisplayList(other.m_cCategoryDisplayList)
{
	m_cActionList.clear();
	m_cCategoryDisplayList.clear();
	m_cCategoryDisplayOrder.clear();
	m_cSeparatorBeforeList.clear();
	m_cSeparatorAfterList.clear();
		
	m_cActionList = other.m_cActionList;
	m_cCategoryDisplayList = other.m_cCategoryDisplayList;
	m_cCategoryDisplayOrder = other.m_cCategoryDisplayOrder;
	m_cSeparatorBeforeList = other.m_cSeparatorBeforeList;
	m_cSeparatorAfterList = other.m_cSeparatorAfterList;
}

ActionList& ActionList::operator=(const ActionList& other)
{
	m_cActionList.clear();
	m_cCategoryDisplayList.clear();
	m_cCategoryDisplayOrder.clear();
	m_cSeparatorBeforeList.clear();
	m_cSeparatorAfterList.clear();
		
	m_cActionList = other.m_cActionList;
	m_cCategoryDisplayList = other.m_cCategoryDisplayList;
	m_cCategoryDisplayOrder = other.m_cCategoryDisplayOrder;
	m_cSeparatorBeforeList = other.m_cSeparatorBeforeList;
	m_cSeparatorAfterList = other.m_cSeparatorAfterList;	
}

ActionList::~ActionList()
{

}

ActionList& ActionList::operator=(ActionList& other)
{
	m_cActionList.clear();
	m_cCategoryDisplayList.clear();
	m_cCategoryDisplayOrder.clear();
	m_cSeparatorBeforeList.clear();
	m_cSeparatorAfterList.clear();
		
	m_cActionList = other.m_cActionList;
	m_cCategoryDisplayList = other.m_cCategoryDisplayList;
	m_cCategoryDisplayOrder = other.m_cCategoryDisplayOrder;
	m_cSeparatorBeforeList = other.m_cSeparatorBeforeList;
	m_cSeparatorAfterList = other.m_cSeparatorAfterList;
}

ActionList& ActionList::operator=(ActionList other)
{
	m_cActionList.clear();
	m_cCategoryDisplayList.clear();
	m_cCategoryDisplayOrder.clear();
	m_cSeparatorBeforeList.clear();
	m_cSeparatorAfterList.clear();
		
	m_cActionList = other.m_cActionList;
	m_cCategoryDisplayList = other.m_cCategoryDisplayList;
	m_cCategoryDisplayOrder = other.m_cCategoryDisplayOrder;
	m_cSeparatorBeforeList = other.m_cSeparatorBeforeList;
	m_cSeparatorAfterList = other.m_cSeparatorAfterList;
}

ActionList& ActionList::AddAction(dialog::Action& a)
{
	m_cActionList.insert(std::pair<os::String,dialog::Action>(a.GetCategory(),a));
	return (*this);
}

ActionList& ActionList::AddAction(const os::String& category,const os::String& name,const os::String& label,int code,const os::String& tip,const os::String& shortcut, const os::String& shortLabel,os::BitmapImage* image)
{
	m_cActionList.insert(std::pair<os::String,dialog::Action>(category,Action(category,name,label,code,tip,shortcut,shortLabel,image)));
	return (*this);
}

os::ToolBar* ActionList::AsToolBar(os::ToolBar* parent,bool showCategories,bool showShortLabel,bool showImages)
{
	os::ToolBar* bar = NULL;
	
	if (parent)
	{
		bar = parent;
	}
	else
	{
		bar = new os::ToolBar(os::Rect(),"toolbar",CF_FOLLOW_LEFT | CF_FOLLOW_RIGHT);
	}
	
	std::list<os::String> categories = GetCategories();
	for (std::list<os::String>::iterator i=categories.begin(); i!=categories.end(); i++)
	{
		if (showCategories)
		{
			if (showCategories)
			{
				bar->AddSeparator(*i);
			}
			
			for (std::multimap<os::String,dialog::Action>::iterator j=m_cActionList.begin(); j!=m_cActionList.end(); j++)
			{
				if (j->first == *i)
				{
					os::ImageButton* b = j->second.AsButton(showImages);
					
					if (j->second.GetShortLabel() != "" && showShortLabel)
					{
						b->SetLabel(j->second.GetShortLabel());
					}
					
					if (b)
					{
						m_cLayoutCleanup.push_back(bar->AddChild(b,os::ToolBar::TB_FIXED_WIDTH));
					}
				}
			}
			
		}
	}
	return bar;
}


os::DropdownMenu* ActionList::AsDropDownMenu(bool showCategories) const
{
}

os::PopupMenu* ActionList::AsPopupMenu(bool showCategories) const
{
}


os::Menu* ActionList::AsMenu(os::Menu* parent,bool showImage)
{
	std::list<os::String> categories = GetCategories();

	os::Menu* menu = NULL;
	if (categories.size() == 1)
	{
		std::list<os::String>::iterator i = categories.begin();
		os::String name = *i;
	
		std::map<os::String,os::String>::iterator it = m_cCategoryDisplayList.find(*i);
		
		if (it != m_cCategoryDisplayList.end())
		{
			name = it->second;
		}		

		menu = new Menu(Rect(),name,ITEMS_IN_COLUMN,CF_FOLLOW_LEFT | CF_FOLLOW_RIGHT);	
		for (std::multimap<os::String,dialog::Action>::iterator j=m_cActionList.begin(); j!=m_cActionList.end(); j++)
		{
			if (j->first == *i)
			{
				if (j->second.IsVisible())
				{
					os::MenuItem* item = j->second.AsMenuItem(showImage);
					item->SetEnable(j->second.IsEnabled());
					
					if (HasSeparatorBefore(j->first,j->second.GetName()))
					{
						menu->AddItem(new os::MenuSeparator());
					}
					menu->AddItem(item);
					
					if (HasSeparatorAfter(j->first,j->second.GetName()))
					{
						menu->AddItem(new os::MenuSeparator());
					}
				}	
			}
		}
	}
	
	else if (categories.size() > 1)
	{
		if (parent)
		{
			menu = parent;
		}
		else
		{
			menu = new Menu(Rect(),"app_list_menu",ITEMS_IN_ROW,CF_FOLLOW_LEFT | CF_FOLLOW_RIGHT);	
		}
		for (std::list<os::String>::iterator i=categories.begin(); i!=categories.end(); i++)
		{
			os::String name = *i;
			std::map<os::String,os::String>::iterator it = m_cCategoryDisplayList.find(*i);
			if (it != m_cCategoryDisplayList.end())
			{
				name = it->second;
			}
			
			os::Menu* m = new Menu(Rect(),name,ITEMS_IN_COLUMN,CF_FOLLOW_LEFT | CF_FOLLOW_RIGHT);
			for (std::multimap<os::String,dialog::Action>::iterator j=m_cActionList.begin(); j!=m_cActionList.end(); j++)
			{
				if (j->first == *i)
				{
					if (j->second.IsVisible())
					{
						os::MenuItem* item = j->second.AsMenuItem(showImage);
						item->SetEnable(j->second.IsEnabled());
						
						if (HasSeparatorBefore(j->first,j->second.GetName()))
						{
							m->AddItem(new os::MenuSeparator());
						}
						
						m->AddItem(item);
						
						if (HasSeparatorAfter(j->first,j->second.GetName()))
						{
							m->AddItem(new os::MenuSeparator());
						}
					}
				}
			}
			menu->AddItem(m);
		}
	}
	return menu;	
}


dialog::ActionList ActionList::FilterBy(const char* category,...)
{
	ActionList a(*this);
	va_list args;
	va_start(args,category);
	va_end(args);

	return a;
}

dialog::ActionList ActionList::FilterTo(const char* category,...)
{
	ActionList a(*this);
	
	va_list args;
	va_start(args,category);

	//Let's filter out categories that we don't want to remove
	//I know this is slow and we should have a better method, but for now it works
	std::list<os::String> categories = this->GetCategories();
	for (const char* c = category; c!=NULL; c = va_arg(args,const char *) )
	{
		printf("FilterTo: %s\n",c);
		if (c != NULL)
		{
			categories.remove(c);
		}
	}
	
	for (std::list<os::String>::iterator i = categories.begin(); i != categories.end(); i++)
	{
		printf("Filter: %s\n",(*i).c_str());
		a.m_cActionList.erase((*i));
	}
	va_end(args);
	return a;
}

dialog::ActionList ActionList::FilterOut(const char* category,...)
{
	ActionList a(*this);

	va_list args;
	va_start(args,category);

	while (category)
	{
		category = va_arg(args,const char *);
		a.m_cActionList.erase(category);
	}
	
	va_end(args);
	return a;
}

std::list<os::String> ActionList::GetCategories()
{
	std::list<os::String> categories;
	
	if (m_cCategoryDisplayOrder.size() > 0)
	{
		for (std::list<os::String>::iterator i=m_cCategoryDisplayOrder.begin(); i!=m_cCategoryDisplayOrder.end(); i++)
		{
			if (m_cActionList.find((*i)) != m_cActionList.end())
			{
				categories.push_back((*i));
			}
			
		}
	
		for (std::multimap<os::String,dialog::Action>::iterator i = m_cActionList.begin();i!=m_cActionList.end();i++)
		{
			std::list<String>::iterator  f = std::find(categories.begin(),categories.end(),i->first);
			
			if (f == categories.end())
				categories.push_back(i->first);
		}
	}
	else
	{
		for (std::multimap<os::String,dialog::Action>::iterator i = m_cActionList.begin();i!=m_cActionList.end();i++)
		{
			std::list<String>::iterator  f = std::find(categories.begin(),categories.end(),i->first);
			
			if (f == categories.end())
				categories.push_back(i->first);
		}		
	}
	categories.unique();
		
	return categories;
}

dialog::ActionList& ActionList::SetCategoryDisplayName(const os::String& category,const os::String& name)
{
	m_cCategoryDisplayList.insert(std::make_pair(category,name));
	return (*this);
}


dialog::ActionList& ActionList::SetCategoryDisplayOrder(const char* category,...)
{
	va_list args;
	va_start(args,category);
	SetCategoryDisplayOrder(args);
	va_end(args);
	return (*this);
}

dialog::ActionList& ActionList::SetCategoryDisplayOrder(const char* category, va_list args)
{
	std::list<os::String> categories;
	for (const char* c = category; c!=NULL; c = va_arg(args,const char *) )
	{
		categories.push_back(c);
	}
	categories.unique();
	SetCategoryDisplayOrder(categories);
	return (*this);
}

dialog::ActionList& ActionList::SetCategoryDisplayOrder(std::list<os::String> categories)
{
	m_cCategoryDisplayOrder = categories;
	return (*this);
}


dialog::ActionList&  ActionList::SetSeparatorBefore(const os::String& category,const char* name,...)
{
	va_list args;
	va_start(args,name);
	SetSeparatorBefore(category,name,args);
	va_end(args);
	return (*this);
}

dialog::ActionList&  ActionList::SetSeparatorBefore(const os::String& category,const char* name,va_list args)
{
	std::list<os::String> names;
	for (const char* c = name; c!=NULL; c = va_arg(args,const char *) )
	{
		names.push_back(c);
	}
	names.unique();
	SetSeparatorBefore(category,names);
	return (*this);
}

dialog::ActionList& ActionList::SetSeparatorBefore(const os::String& category,std::list<os::String> names)
{
	m_cSeparatorBeforeList[category] = names;
	return (*this);
}

bool ActionList::HasSeparatorBefore(const os::String& category,const os::String& name)
{
	if (m_cSeparatorBeforeList.count(category))
	{
		std::list<String>::iterator f = std::find(m_cSeparatorBeforeList[category].begin(),m_cSeparatorBeforeList[category].end(),name);
			
		if (f != m_cSeparatorBeforeList[category].end())
		{
			return true;
		}
	}
	return false;
}
		
dialog::ActionList&  ActionList::SetSeparatorAfter(const os::String& category,const char* name,...)
{
	va_list args;
	va_start(args,name);
	SetSeparatorAfter(category,name,args);
	va_end(args);
	return (*this);
}

dialog::ActionList&  ActionList::SetSeparatorAfter(const os::String& category,const char* name,va_list args)
{
	std::list<os::String> names;
	for (const char* c = name; c!=NULL; c = va_arg(args,const char *) )
	{
		names.push_back(c);
	}
	names.unique();
	SetSeparatorAfter(category,names);
	return (*this);
}

dialog::ActionList&  ActionList::SetSeparatorAfter(const os::String& category,std::list<os::String> names)
{
	m_cSeparatorAfterList[category] = names;
	return (*this);
}

bool ActionList::HasSeparatorAfter(const os::String& category,const os::String& name)
{
	if (m_cSeparatorAfterList.count(category))
	{
		std::list<String>::iterator f = std::find(m_cSeparatorAfterList[category].begin(),m_cSeparatorAfterList[category].end(),name);
			
		if (f != m_cSeparatorAfterList[category].end())
		{
			return true;
		}
	}
	return false;
}

dialog::Action& ActionList::FindAction(const os::String& category,const os::String& name)
{
	for (std::multimap<os::String,dialog::Action>::iterator i=m_cActionList.begin(); i!= m_cActionList.end(); i++)
	{
		if (name == i->second.GetName())
		{
			return i->second;
		}
	}
	throw( GeneralFailure( "Couldn't find action", -1 ) );
	
}


