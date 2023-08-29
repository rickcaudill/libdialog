#ifndef APPLICATION_H
#define APPLICATION_H

#include <util/application.h>
#include <gui/window.h>
#include <dialog/standardwindow.h>

class App : public os::Application
{
public:
	App();
private:
	dialog::StandardWindow* m_pcMainWindow;
};

#endif

