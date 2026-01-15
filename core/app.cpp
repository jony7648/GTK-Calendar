#include <iostream>
#include <gtk/gtk.h>
#include "app.h"

namespace core {
App::App(const std::string& app_title, const space::Point& dimensions, int argc, char* argv[]) {
	this->win_dimensions.x = dimensions.x;
	this->win_dimensions.y = dimensions.y;
	this->gtk_app = gtk_application_new(app_title.c_str(), G_APPLICATION_DEFAULT_FLAGS); 
	this->argc = argc;
	this->argv = argv;
}

App::~App() {
	if (window) {
		delete window;
	}

	g_object_unref(gtk_app);
}

void App::attach_window(Window* window) {
	//attaches a window to the  app
	this->window = window;
}

void App::run(void(*activate_func)(GtkApplication* app, gpointer), core::MessengerData<App>* signal_ptr) {
	//g_signal_connect(this->gtk_app, "activate", G_CALLBACK(activate_func), NULL);
	g_signal_connect(gtk_app, "activate", G_CALLBACK(activate_func), signal_ptr);

	app_status = g_application_run(G_APPLICATION(gtk_app), 0, argv);
}

void App::set_scene(core::Scene* scene) {
	if (window == nullptr) {
		std::cout << "ERROR: the app does not have a window attached!\n";
		return;
	}
	
	if (scene == nullptr) {
		std::cout << "ERROR: the new scene isa nullptr!\n";
		return;
	}

	gtk_window_set_child(GTK_WINDOW(window->get_gtk_window()), scene->container->get_gtk_widget());

	if (current_scene != nullptr) {
		delete current_scene;
	}

	current_scene = scene;
	//current_scene->container->win_dimensions();
}

void App::display_window() {
	window->display(current_scene->container->get_gtk_widget());
}

GtkWidget* App::get_scene_container() {
	return current_scene->container->get_gtk_widget();
}

core::TimeComponet* App::get_time_componet() {
	return &time_componet;
}

const space::Point& App::get_win_dimensions() {
	return win_dimensions;
}
}
