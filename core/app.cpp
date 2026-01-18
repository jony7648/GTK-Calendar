#include <iostream>
#include <gtk/gtk.h>
#include "app.h"

bool is_custom_point(const space::Point& point) {
	if (point.x == 0 && point.y == 0) {
		return true;	
	}

	return false;	
}

bool process_close_request(GtkWidget* gtk_app, gpointer user_data) {
	auto* sig_data = (core::DoubleMessenger<core::App*, core::Window*>*)user_data;
	bool is_main_window;

	core::App* app = sig_data->object1;
	core::Window* window = sig_data->object2;

	is_main_window = window->get_is_main_window();

	//delete window;

	return false;	
}

namespace core {
App::App(const std::string& app_title, const space::Point& dimensions, int argc, char* argv[]) {
	this->win_dimensions.x = dimensions.x;
	this->win_dimensions.y = dimensions.y;
	this->gtk_app = gtk_application_new(app_title.c_str(), G_APPLICATION_DEFAULT_FLAGS); 
	this->argc = argc;
	this->argv = argv;
}

App::~App() {
	g_object_unref(gtk_app);
}

Error App::attach_main_window(Window* window) {
	if (window == nullptr) {
		return Error::NULLPTR;
	}

	//attaches a window to the  app
	this->main_window = window;

	window->signal_set_close(this, process_close_request);
	return Error::CLEAR;
}

Scene* App::get_main_scene() {
	return main_scene;	
}

int App::attach_subwin(Window* subwin) {
	if (subwin == nullptr) {
		return 1;
	}

	subwin_vect.push_back(subwin);

	return 0;
}

Error App::attach_main_scene(core::Scene* scene) {
	bool set_dimensions = false;

	if (scene == nullptr) {
		std::cout << "ERROR: the new scene is a nullptr!\n";
		return Error::NULLPTR;
	}

	if (main_window == nullptr) {
		std::cout << "ERROR: app does not have a main window. Set main window before main scene\n";
		return Error::NULLPTR;
	}

	gtk_window_set_child(GTK_WINDOW(main_window->get_gtk_window()), scene->container->get_gtk_widget());

	if (main_scene != nullptr) {
		delete main_scene;
	}

	main_scene = scene;

	return Error::CLEAR;
}

void App::display_main_window() {
	main_window->display(main_scene);
}

GtkWidget* App::get_scene_container() {
	return main_scene->container->get_gtk_widget();
}

void App::run(void(*activate_func)(GtkApplication* app, gpointer), core::Messenger<App*>* signal_ptr) {
	g_signal_connect(gtk_app, "activate", G_CALLBACK(activate_func), signal_ptr);

	app_status = g_application_run(G_APPLICATION(gtk_app), 0, argv);
}

void App::set_subwin_cap(int cap) {
	subwin_cap = cap;
}

core::TimeComponet* App::get_time_componet() {
	return &time_componet;
}

const space::Point& App::get_win_dimensions() {
	return win_dimensions;
}
}
