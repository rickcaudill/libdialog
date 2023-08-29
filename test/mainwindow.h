#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <util/application.h>
#include <gui/window.h>
#include <gui/image.h>
#include <util/message.h>
#include <util/resources.h>
#include <dialog/alert.h>
#include <dialog/standardwindow.h>
#include <gui/textview.h>
#include <gui/layoutview.h>

class MainWindow : public dialog::StandardWindow
{
public:
	MainWindow();
	void HandleMessage( os::Message* );

	virtual void SetupActions();
	virtual void SetupMenus();
	virtual void SetupToolBar();
	virtual void SetupStatusBar();
	virtual void SetupMainView();
private:
	bool OkToQuit(); // Obsolete?
	
	dialog::Alert* alert;
};

#endif





