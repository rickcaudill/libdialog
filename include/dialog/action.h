#ifndef __F_DIALOG__ACTION_H__
#define __F_DIALOG__ACTION_H__

#include <util/string.h>
#include <gui/imagebutton.h>
#include <gui/image.h>
#include <gui/menu.h>
#include <gui/checkmenu.h>

using namespace os;

namespace dialog
{
	class Action
	{
		public:
			Action(const os::String& cCategory,const os::String& cName,const os::String& cLabel,int nMessageCode,const os::String& cTip="",const os::String& cShortcut="", const os::String& cShortLabel="",os::BitmapImage* image=NULL);
			Action(const os::String& category,const os::String& name, int code);
								
			os::ImageButton*	AsButton(bool showImage=true) const;
			os::MenuItem*		AsMenuItem(bool showImage=true) const;
			
			os::String GetCategory() const;
			dialog::Action& SetCategory(const os::String&);
				
			os::String GetName() const;
			dialog::Action& SetName(const os::String&);
			
			os::String GetLabel() const;
			dialog::Action& SetLabel(const os::String&);

			os::String GetShortLabel() const;
			dialog::Action& SetShortLabel(const os::String&);
						
			os::String GetTip() const;
			dialog::Action& SetTip(const os::String&);

			os::String GetShortcut() const;
			dialog::Action& SetShortcut(const os::String&);			
			
			os::BitmapImage* GetImage() const;
			dialog::Action& SetImage(os::BitmapImage*);
			
			int GetCode() const;
			dialog::Action& SetCode(int);
			
			dialog::Action& SetVisible(bool);
			bool IsVisible() const;
			
			dialog::Action& SetEnable(bool);
			bool IsEnabled() const;
			
			dialog::Action& SetCheckable(bool);
			bool IsCheckable() const;
			
			dialog::Action& SetChecked(bool);
			bool IsChecked() const;
			
			dialog::Action& SetMutuallyExclusive(bool);
			bool IsMutuallyExclusive() const;
		private:
			os::String m_cCategory;
			os::String m_cName;
			os::String m_cLabel;
			os::String m_cTip;
			os::String m_cShortcut;
			os::String m_cShortLabel;
			
			bool m_bEnabled;
			bool m_bVisible;
			bool m_bCheckable;
			bool m_bChecked;
			bool m_bMutuallyExclusive;
			
			os::BitmapImage* m_pcImage;
			int m_nMessageCode;
	};
};
#endif







