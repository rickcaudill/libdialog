#ifndef __F_DIALOG_ALERT_H__
#define __F_DIALOG_ALERT_H__

#include <stdarg.h>

#include <vector>
#include <gui/image.h>
#include <util/string.h>
#include <gui/window.h>
#include <gui/bitmap.h>
#include <gui/stringview.h>
#include <gui/button.h>
#include <util/message.h>
#include <gui/font.h>
#include <gui/desktop.h>
#include <util/invoker.h>
using namespace os;

namespace dialog
{
#if 0
} // Fool Emacs auto-indent
#endif


class AlertView : public View
{
public:
   AlertView(const os::String& cText,va_list pButtons,os::BitmapImage* pcImage=NULL);
    virtual ~AlertView();
    virtual Point GetPreferredSize( bool bLargest );
    
    virtual void	 AllAttached();
  
private:
    AlertView& operator=( const AlertView& );
    AlertView( const AlertView& );

    friend class Alert;
    class Privates;
    Privates *m; 
};

/** 
 * \ingroup gui
 * \par Description:
 *
 * \sa
 * \author	Kurt Skauen (kurt@atheos.cx), with modifications by Rick Caudill ( cau0730@cup.edu)
 *****************************************************************************/

class Alert : public Window
{
public:
	/** Icons
	 *\par: Description:
	 *    	These values are used to specify different icons in the Alert. */
	enum alert_icon{
			/** Warning Icon:  Use this when you want to flag an warning Alert. */ 
		ALERT_WARNING = 0,
			/** Info Icon:     Use this when you want to flag information to the user. */  
		ALERT_INFO = 1,
			/** Question Icon: Use this when you want to ask the user a question. */ 
		ALERT_QUESTION = 2,
			/** Tip Icon:      Use this when you want to give the user a tip for something. */ 
		ALERT_TIP = 3,
		
		ALERT_ERROR = 4,
	}; 
 
public:
    Alert( const String& cTitle, const String& cText, int nFlags, ... );
    Alert( const String& cTitle, const String& cText, os::Bitmap* pcBitmap, int nFlags, ... ); 
    Alert( const String& cTitle, const String& cText, alert_icon nAlertNum, int nFlags, ...);
    Alert( const String& cTitle, const String& cText, BitmapImage* pcBitmap, int nFlags, ... ); 
    Alert( const String& cTitle,View*);
   ~Alert();
  
   
	virtual void	HandleMessage( Message* pcMessage );
	int  Go();
	void Go( Invoker* pcInvoker );
private:
    Alert& operator=( const Alert& );
    Alert( const Alert& );

	class Private;
	Private *m;

    void SetImage(uint32 nWidth, uint32 nHeight, uint8* pnBuffer);
};
}
#endif






