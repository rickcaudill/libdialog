#include <dialog/action.h>
#include <cassert>
using namespace dialog;

Action::Action(const os::String& cCategory,const os::String& cName,const os::String& cLabel,int nMessageCode,const os::String& cTip,const os::String& cShortcut, const os::String& cShortLabel,os::BitmapImage* pcImage)
{
	assert(nMessageCode >= M_FIRST_EVENT);
	assert(cCategory != "");
	assert(cName != "");
	
	m_bVisible = m_bEnabled = true;
	m_bCheckable = m_bChecked = false;
	
	m_cCategory = cCategory;
	m_cName = cName;
	m_cLabel = cLabel;
	m_nMessageCode = nMessageCode;
	m_cTip = cTip;
	m_cShortcut = cShortcut;
	m_cShortLabel = cShortLabel;
	m_pcImage = pcImage;
}

Action::Action(const os::String& category,const os::String& name, int code)
{
	assert(code >= M_FIRST_EVENT);
	assert(category != "");
	assert(name != "");
	
	m_bVisible = m_bEnabled = true;
	m_bCheckable = m_bChecked = false;
	
	m_cCategory = category;
	m_cName = m_cLabel = name;
	m_nMessageCode = code;
	
	m_cTip = m_cShortcut = m_cShortLabel = m_cTip = "";
	m_pcImage = NULL;
}

os::ImageButton* Action::AsButton(bool showImage) const
{
	os::ImageButton* b = new os::ImageButton(os::Rect(),m_cName,m_cLabel,new os::Message(m_nMessageCode),showImage ? m_pcImage : NULL,os::ImageButton::IB_TEXT_BOTTOM,true,true,true,CF_FOLLOW_LEFT|CF_FOLLOW_RIGHT);
	return b;
}

os::MenuItem* Action::AsMenuItem(bool showImage) const
{
	os::MenuItem* item = NULL;
	
	if (m_bCheckable)
	{
		item = new os::CheckMenu(m_cLabel,new os::Message(m_nMessageCode),m_bChecked);
	}
	else
	{	
		item = new os::MenuItem(m_cLabel,new os::Message(m_nMessageCode),m_cShortcut,showImage ? m_pcImage : NULL);
	}
	
	item->SetEnable(m_bEnabled);
	return item;
}						

os::String Action::GetCategory() const
{
	return m_cCategory;
}

dialog::Action& Action::SetCategory(const os::String& category)
{
	m_cCategory = category;
	return (*this);
}

os::String Action::GetName() const
{
	return m_cName;
}

dialog::Action& Action::SetName(const os::String& name)
{
	m_cName = name;
	return (*this);
}

os::String Action::GetLabel() const
{
	return m_cLabel;
}

dialog::Action& Action::SetLabel(const os::String& label)
{
	m_cLabel = label;
	return (*this);
}

os::String Action::GetShortLabel() const
{
	return m_cShortLabel;
}

dialog::Action& Action::SetShortLabel(const os::String& label)
{
	m_cShortLabel = label;
	return (*this);
}

os::String Action::GetTip() const
{
	return m_cTip;
}

dialog::Action& Action::SetTip(const os::String& tip)
{
	m_cTip = tip;
	return (*this);
}

os::String Action::GetShortcut() const
{
	return m_cShortcut;
}

dialog::Action& Action::SetShortcut(const os::String& shortcut)
{
	m_cShortcut = shortcut;
	return (*this);
}

os::BitmapImage* Action::GetImage() const
{
	return m_pcImage;
}

dialog::Action& Action::SetImage(os::BitmapImage* image)
{
	m_pcImage = image;
	return (*this);
}

int Action::GetCode() const
{
	return m_nMessageCode;
}

dialog::Action& Action::SetCode(int code)
{
	assert(code >= M_FIRST_EVENT);
	m_nMessageCode = code;
	return (*this);
}

dialog::Action& Action::SetEnable(bool enable)
{
	m_bEnabled = enable;
	return (*this);
}

bool Action::IsEnabled() const
{
	return m_bEnabled;
}

dialog::Action& Action::SetVisible(bool visible)
{
	m_bVisible = visible;
	return (*this);
}

bool Action::IsVisible() const
{
	return m_bVisible;
}

dialog::Action& Action::SetCheckable(bool checkable)
{
	m_bCheckable = checkable;
	return (*this);
}

bool Action::IsCheckable() const
{
	return m_bCheckable;
}
			
dialog::Action& Action::SetChecked(bool checked)
{
	m_bChecked = checked;
	return (*this);
}

bool Action::IsChecked() const
{
	return m_bChecked;
}

