#pragma once
#include <gtk/gtk.h>
#include <iostream>
#include "util.h"
#include "messenger.h"
#include "space.h"
#include "window.h"
#include "time_componet.h"


namespace core {
class App {
private:
	Window* window = nullptr;
	GtkApplication* gtk_app = nullptr;
	core::TimeComponet time_componet;
	std::string title;
	space::Point win_dimensions;

	int argc = 0;
	char** argv = nullptr;
	int app_status = 0;
public:
	App(const std::string& title, const space::Point& dimensions,  int argc, char* argv[]);
	~App();
	void run(void(*activate_func)(GtkApplication*, gpointer), core::MessengerData<App>* signal_ptr);
	void attach_window(Window* window);
	core::TimeComponet* get_time_componet();
	const space::Point& get_win_dimensions();

};
}
