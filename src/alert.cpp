/*  libsyllable.so - the highlevel API library for Syllable
 *  Copyright (C) 1999 - 2001 Kurt Skauen
 *  Copyright (C) 2003 - 2004 Syllable Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of version 2 of the GNU Library
 *  General Public License as published by the Free Software
 *  Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 *  MA 02111-1307, USA
 */
#include <dialog/alert.h>
#include <gui/layoutview.h>
#include <gui/stringview.h>
#include <gui/imageview.h>
#include <storage/memfile.h>

#include "images/alert.h"

using namespace dialog;
using namespace os;

class AlertView::Privates
{
	public:
	Privates() {
		m_pcImage = NULL;
	}
	
	~Privates() {
	}
	
	public:
		os::LayoutView* m_pcView;
		std::vector<Button*> m_cButtons;
		os::BitmapImage* m_pcImage;
		os::ImageView* m_pcImageView;
};

class Alert::Private
{
	public:
	Private() {
		m_pcBitmap = NULL;
		m_hMsgPort = -1;
		m_pcInvoker = NULL;
		m_pcImage = NULL;
	}

	~Private() {
		if( m_hMsgPort != -1 )
		{
			delete_port( m_hMsgPort );
		}
		if( m_pcInvoker )
		{
			delete m_pcInvoker;
		}	
		if( m_pcBitmap != NULL )
		{
			delete m_pcBitmap;
		}
	}
	
	public:
    AlertView* m_pcView;
    Invoker*   m_pcInvoker;
    port_id    m_hMsgPort;
	Bitmap* m_pcBitmap;
	BitmapImage* m_pcImage;
};


/** Initialize the Alert.
 * \par Description:
 *	The Alert is like a Messagebox in windows.  It provides a way to show errors,
 *  About boxes or even tips without having to create your own Window. 
 * \param cTitle - The title of the window.
 * \param cText  - The text that will be shown in the Alert.
   \param nFlags - The flags that will be passed to the appserver to tell the appserver what options
   				   when creating this Alert.
 * \param ...    - The name of buttons to create(IE: would be like "ok" or "cancel").  You can create 
 					more than one button(IE: new Alert("Title","Text",WND_NOT_RESIZABLE,"OK", "CANCEL",NULL);).  Caveat: Make sure to add "NULL" to the end of the of the constructor.
 * \sa Go() 
 * \author	Kurt Skauen (kurt@atheos.cx) with modifications by Rick Caudill
 *****************************************************************************/
Alert::Alert( const String & cTitle, const String & cText, int nFlags, ... ):Window( Rect( 100, 50, 100, 50 ), "alert_window", cTitle.c_str(), WND_NOT_RESIZABLE | WND_NO_ZOOM_BUT | WND_NO_DEPTH_BUT | WND_MODAL)
{
	m = new Private;

	va_list pArgs;

	va_start( pArgs, nFlags );

	m->m_pcView = new AlertView( cText, pArgs );
	m->m_pcView->SetFgColor( 0, 0, 0 );
	m->m_pcView->SetBgColor( get_default_color( COL_NORMAL ) );
	Point cSize = m->m_pcView->GetPreferredSize( false );

	m->m_pcView->ResizeTo( cSize );
	ResizeTo( cSize );

	if( nFlags > 0 )
	{
		SetFlags( nFlags );
	}
	Desktop cDesktop;

	MoveTo( cDesktop.GetResolution().x / 2 - cSize.x / 2, cDesktop.GetResolution(  ).y / 2 - cSize.y / 2 );

	AddChild( m->m_pcView );
	va_end( pArgs );

	Flush();
	Sync();
}
	
/** Initialize the Alert.
 * \par Description:
 *	The Alert is like a Messagebox in windows.  It provides a way to show errors,
 *  About boxes or even tips without having to create your own Window. 
 * \param cTitle   - The title of the window.
 * \param cText    - The text that will be shown in the Alert.
 * \param nFlags   - The flags that will be passed to the appserver to tell the appserver what options
   				     when creating this.
 * \param pcBitmap - Bitmap that will show up in the Alert Window. 
 * \param ...      - The name of buttons to create(IE: would be like "ok" or "cancel").  You can create 
 					more than one button(IE: new Alert("Title","Text",WND_NOT_RESIZABLE,"OK", "CANCEL");).    Caveat: Make sure to add "NULL" to the end of the of the constructor.
 * \sa Go() 
 * \author	Kurt Skauen (kurt@atheos.cx) with modifications by Rick Caudill
 *****************************************************************************/
Alert::Alert( const os::String & cTitle, const os::String & cText, os::Bitmap * pcBitmap, int nFlags, ... ):Window( Rect( 100, 50, 100, 50 ), "alert_window", cTitle.c_str(), WND_NOT_RESIZABLE | WND_NO_ZOOM_BUT | WND_NO_DEPTH_BUT | WND_MODAL )
{
	m = new Private;
	m->m_pcBitmap = pcBitmap;
	
	va_list pArgs;
	va_start( pArgs, nFlags );

	if( nFlags > 0 )
	{
		SetFlags( nFlags );
	}

	os::BitmapImage* pcIcon = NULL;
	pcIcon = new os::BitmapImage((uint8*)m->m_pcBitmap->LockRaster(), IPoint(pcBitmap->GetBounds().Size()) + IPoint(1,1),pcBitmap->GetColorSpace());
	
	m->m_pcView = new AlertView( cText, pArgs, pcIcon );
	Point cSize = m->m_pcView->GetPreferredSize( false );

	m->m_pcView->ResizeTo( cSize );
	ResizeTo( cSize );

	Desktop cDesktop;
	MoveTo( cDesktop.GetResolution().x / 2 - cSize.x / 2, cDesktop.GetResolution(  ).y / 2 - cSize.y / 2 );

	AddChild( m->m_pcView );
	va_end( pArgs );
}


Alert::Alert( const os::String & cTitle, const os::String & cText, os::BitmapImage* pcBitmap, int nFlags, ... ):Window( Rect( 100, 50, 100, 50 ), "alert_window", cTitle.c_str(), WND_NOT_RESIZABLE | WND_NO_ZOOM_BUT | WND_NO_DEPTH_BUT | WND_MODAL )
{
	m = new Private;
		
	va_list pArgs;
	va_start( pArgs, nFlags );

	if( nFlags > 0 )
	{
		SetFlags( nFlags );
	}

	m->m_pcView = new AlertView( cText, pArgs, pcBitmap );
	Point cSize = m->m_pcView->GetPreferredSize( false );

	m->m_pcView->ResizeTo( cSize );
	ResizeTo( cSize );

	Desktop cDesktop;
	MoveTo( cDesktop.GetResolution().x / 2 - cSize.x / 2, cDesktop.GetResolution(  ).y / 2 - cSize.y / 2 );

	AddChild( m->m_pcView );
	va_end( pArgs );
}

/** Initialize the Alert.
 * \par Description:
 *	The Alert is like a Messagebox in windows.  It provides a way to show errors,
 *  About boxes or even tips without having to create your own Window. 
 * \param cTitle    - The title of the window.
 * \param cText     - The text that will be shown in the Alert.
 * \param nFlags    - The flags that will be passed to the appserver to tell the appserver what options
   				       when creating this.
 * \param nAlertNum - Static icons that will show up when you call this constructor. To call the icons you would do something like this: (new Alert("Title","Text", Alert::ALERT_WARNING,0,"OK",NULL);)
 * \param ...       - The name of buttons to create(IE: would be like "ok" or "cancel").  You can create 
 					more than one button(IE: new Alert("Title","Text",WND_NOT_RESIZABLE,"OK", "CANCEL");).    Caveat: Make sure to add "NULL" to the end of the of the constructor.
 * \sa Go() 
 * \author	Kurt Skauen (kurt@atheos.cx) with modifications by Rick Caudill
 *****************************************************************************/
Alert::Alert( const String & cTitle, const String & cText, alert_icon nAlertNum, int nFlags, ... ):Window( Rect( 100, 50, 100, 50 ), "alert_window", cTitle.c_str(), WND_NOT_RESIZABLE | WND_NO_ZOOM_BUT | WND_NO_DEPTH_BUT | WND_MODAL )
{
	m = new Private;

	va_list pArgs;
	va_start( pArgs, nFlags );

	if( nFlags > 0 )
	{
		SetFlags( nFlags );
	}
	
	switch ( nAlertNum )
	{
		case ALERT_WARNING:
		{
			SetImage( nWarning_Icon.width, nWarning_Icon.height, (uint8*)nWarning_Icon.pixel_data);
			break;
		}
		case ALERT_INFO:
		{
			SetImage( nTip_Icon.width, nTip_Icon.height, ( uint8 * )nTip_Icon.pixel_data );
			break;
		}
		case ALERT_QUESTION:
		{
			SetImage( nQuestion_Icon.width, nQuestion_Icon.height, ( uint8 * )nQuestion_Icon.pixel_data );
			break;
		}
		case ALERT_TIP:
		{
			SetImage( nTip_Icon.width, nTip_Icon.height, ( uint8 * )nTip_Icon.pixel_data );
			break;
		}		
		case ALERT_ERROR:
		{
			SetImage( nError_Icon.width, nError_Icon.height, ( uint8 * )nError_Icon.pixel_data );
			break;
		}
		default:
			break;
	}

	os::BitmapImage* pcIcon = NULL;
	pcIcon = new os::BitmapImage((uint8*)m->m_pcBitmap->LockRaster(), os::IPoint( 48,48 ),CS_RGBA32);

	m->m_pcView = new AlertView( cText, pArgs,pcIcon );
	Point cSize = m->m_pcView->GetPreferredSize( false );
	m->m_pcView->ResizeTo( cSize );
	ResizeTo( cSize );

	Desktop cDesktop;
	MoveTo( cDesktop.GetResolution().x / 2 - cSize.x / 2, cDesktop.GetResolution(  ).y / 2 - cSize.y / 2 );

	AddChild( m->m_pcView );
	va_end( pArgs );

}

/** Initialize the Alert.
 * \par Description:
 *	The Alert is like a Messagebox in windows.  It provides a way to show errors,
 *  About boxes or even tips without having to create your own Window.    When you 
 *  invoke this constructor, you will be able to add a view to the Alert.  This way you can 
 *  create your own specialized Alert. 
 * \param cTitle    - The title of the window.
 * \param pcView -  The view that will be added to the Alert   
 * \par Example:  If the View that is added to the Alert has another control
 *                on it, you must invoke Control::SetTarget(Messenger*)
 * \code
 *  Window* pcWindow = new Window(Rect(100,100,500,500),"","Alert test")
 *  View* pcView = new View(Rect(),"");
 *  Button* pcButton  = new Button(Rect(),"","Ok", new Message(NULL));
 *  pcButton->SetTarget(pcWindow) //bad example, but it shows how to do it
 *  Alert * pcAlert("Test",pcView);
 * \endcode
 * \sa Go() 
 * \author	Rick Caudill(cau0730@cup.edu)
 *****************************************************************************/
Alert::Alert( const String & cTitle, os::View * pcView ):Window( Rect( 100, 50, pcView->GetBounds().Width(  ) + 100, pcView->GetBounds(  ).Height(  ) + 50 ), "alert_window", cTitle.c_str(  ), WND_NOT_RESIZABLE | WND_NO_ZOOM_BUT | WND_NO_DEPTH_BUT | WND_MODAL)
{
	m = new Private;

	m->m_pcInvoker = NULL;
	m->m_hMsgPort = -1;

	AddChild( pcView );
}

//----------------------------------------------------------------------------
// NAME:
// DESC:
// NOTE:
// SEE ALSO:
//----------------------------------------------------------------------------

Alert::~Alert()
{
	delete m;
}

//----------------------------------------------------------------------------
// NAME:
// DESC:
// NOTE:
// SEE ALSO:
//----------------------------------------------------------------------------

void Alert::HandleMessage( Message * pcMessage )
{
	if( pcMessage->GetCode() < int32 ( m->m_pcView->m->m_cButtons.size(  ) ) )
	{
		if( m->m_hMsgPort >= 0 )
		{
			send_msg( m->m_hMsgPort, pcMessage->GetCode(), NULL, 0 );
		}
		else
		{
			if( m->m_pcInvoker != NULL )
			{
				Message *pcMsg = m->m_pcInvoker->GetMessage();

				if( pcMsg == NULL )
				{
					dbprintf( "Error: Invoker registered with this Alert requester does not have a message!\n" );
				}
				else
				{
					pcMsg->AddInt32( "which", pcMessage->GetCode() );
					m->m_pcInvoker->Invoke();
				}
			}
			PostMessage( M_QUIT );
		}
	}
	else
	{
		Handler::HandleMessage( pcMessage );
	}
}

//----------------------------------------------------------------------------
// NAME:
// DESC:
// NOTE:
// SEE ALSO:
//----------------------------------------------------------------------------

int Alert::Go()
{
	uint32 nCode;
	int nError;

	m->m_hMsgPort = create_port( "alert_port", DEFAULT_PORT_SIZE );

	Show();
	MakeFocus();
	if( m->m_hMsgPort < 0 )
	{
		dbprintf( "Alert::WaitForSelection() failed to create message port\n" );
		PostMessage( M_QUIT );
		return ( -1 );
	}
	nError = get_msg( m->m_hMsgPort, &nCode, NULL, 0 );
	if( nError < 0 )
	{
		PostMessage( M_QUIT );
		return ( -1 );
	}
	else
	{
		PostMessage( M_QUIT );
		return ( nCode );
	}

}



//----------------------------------------------------------------------------
// NAME:
// DESC:
// NOTE:
// SEE ALSO:
//----------------------------------------------------------------------------


void Alert::Go( Invoker * pcInvoker )
{
	m->m_pcInvoker = pcInvoker;
	Show();
	MakeFocus();
}



//----------------------------------------------------------------------------
// NAME: SetImage()
// DESC: SetImage() is a private method for adding alert icons to the alert
// NOTE:
// SEE ALSO:
//----------------------------------------------------------------------------

void Alert::SetImage( uint32 nWidth, uint32 nHeight, uint8 *pnBuffer )
{
	if( m->m_pcBitmap != 0 )
		delete m->m_pcBitmap;
	
	uint32 nBufferSize = nWidth * nHeight * 4;

	m->m_pcBitmap = new Bitmap( nWidth, nHeight, CS_RGBA32 );
	uint8 *pnRaster = m->m_pcBitmap->LockRaster();
	
	for( uint32 nByte = 0; nByte < nBufferSize; nByte += 4 )
	{
		( *pnRaster++ ) = pnBuffer[nByte + 2];
		( *pnRaster++ ) = pnBuffer[nByte + 1];
		( *pnRaster++ ) = pnBuffer[nByte];
		( *pnRaster++ ) = pnBuffer[nByte + 3];
	}
	m->m_pcBitmap->UnlockRaster();
}


AlertView::AlertView(const os::String& cText, va_list pButtons,os::BitmapImage* image) : View(Rect(0,0,1,1),"alertview",CF_FOLLOW_ALL)
{
	m = new Privates;
	m->m_pcImage = image;
	m->m_pcView = new os::LayoutView(os::Rect(0,0,1,1),"alert_private_layout",NULL);
	
	/*create our main layout node, and pad each side by 5 pixels*/
	os::HLayoutNode* root = new os::HLayoutNode("root");
	root->SetBorders(os::Rect(5,5,5,5));
	
	/*we will only set this if there is an image*/
	if (m->m_pcImage)
	{
		os::VLayoutNode* imageNode = new os::VLayoutNode("image_node");
		imageNode->SetBorders(os::Rect(0,0,10,0));
		imageNode->AddChild(m->m_pcImageView = new os::ImageView(Rect(0,0,0,0),"alert_imageview",m->m_pcImage,os::ImageView::DEFAULT,CF_FOLLOW_NONE),1.0f);
		imageNode->AddChild(new os::VLayoutSpacer("",10.0f));
		root->AddChild(imageNode);
		root->AddChild(new os::HLayoutSpacer(""));
	}
	
	os::VLayoutNode* text_to_button_node = new os::VLayoutNode("text_to_button_node");
	os::VLayoutNode* textNode = new os::VLayoutNode("text_node");
	textNode->AddChild(new os::StringView(os::Rect(),"alert_stringview",cText,DTF_ALIGN_LEFT));
	text_to_button_node->AddChild(textNode);
	
	//for the button message, i = the button number count, we use this for the message
	uint i=0;
	
	/*create a new node, set its alignment to right*/
	os::VLayoutNode* buttonNode = new VLayoutNode("button_node");
	buttonNode->SetHAlignment(os::ALIGN_RIGHT);
	buttonNode->SetVAlignment(os::ALIGN_RIGHT);
	
	/*for args iteration, and previous but name so we can use 'SameWidth'*/
	const char* pzButName;
	const char* pzPrevButName = NULL;
	
	//our first button node
	os::HLayoutNode* node = new os::HLayoutNode("button_node_1");
	
	/*iterate through args*/
	while( ( pzButName = va_arg( pButtons, const char * ) ) != NULL )
	{
		/*let us split by 4, because it's a great number*/
		/*we add the previous node, then create another one (lets not forget to set prev to null*/
		if (i%4 == 0)
		{
			buttonNode->AddChild(node);
			node = new os::HLayoutNode(os::String().Format("button_node_%d",i));
			pzPrevButName = NULL;
		}
		
		//create our button, add it to the list of buttons, then add it to our node
		Button *pcButton = new Button( Rect( 0, 0, 1, 1 ), pzButName, pzButName, new Message( i++ ) );
		m->m_cButtons.push_back( pcButton );
		node->AddChild( pcButton );
		
		if (pzPrevButName != NULL)
		{
			node->SameWidth(pzPrevButName,pzButName,NULL);
			node->SameHeight(pzPrevButName,pzButName,NULL);
		}
		pzPrevButName = pzButName;
		pcButton->SetTabOrder();
	}
	
	if (i > 1)
	{
	}
	else
	{
		/*this will pad with 5 extra pixels wide, and 2 pixels vertical*/
		os::Point p = m->m_cButtons[0]->GetPreferredSize(false) + Point(5,2);
		node->LimitMaxSize(p);
	}
	buttonNode->AddChild(node);

	text_to_button_node->AddChild(buttonNode);
	root->AddChild(text_to_button_node);
	m->m_pcView->SetRoot(root);
	AddChild(m->m_pcView);
}

AlertView::~AlertView()
{
	delete m;
}

void AlertView::AllAttached()
{
	m->m_pcImage->Sync();
	m->m_pcImageView->Refresh();
	m->m_pcImageView->Sync();
	m->m_cButtons[0]->MakeFocus();
}

Point AlertView::GetPreferredSize( bool bLargest )
{
	return ( m->m_pcView->GetPreferredSize(bLargest));
}






