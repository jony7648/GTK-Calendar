#include <iostream>
#include <gtk/gtk.h>
#include "app.h"


bool is_custom_point(const space::Point& point) {
	if (point.x == 0 && point.y == 0) {
		return true;	
	}

	return false;	
}



void signal_request_subwin(void* app_addr, void* win_addr) {
	core::App* app = static_cast<core::App*>(app_addr);
	core::Window* win = static_cast<core::Window*>(app_addr);

	app->request_subwin("Note Scene");
}

namespace core {
App::App(const std::string& app_title, const space::Point& dimensions, int argc, char* argv[]) {
	this->win_dimensions.x = dimensions.x;
	this->win_dimensions.y = dimensions.y;
	this->gtk_app = gtk_application_new(app_title.c_str(), G_APPLICATION_DEFAULT_FLAGS); 
	this->argc = argc;
	this->argv = argv;
	
	signaler.set_parent_widget(this);
	S_scene_request_subwin.set_parent_widget(this);
	S_scene_request_subwin.set_emit_func(&signal_request_subwin);
}

App::~App() {
	for (auto& win : subwin_vect) {
		delete win;
	}


	delete main_window;
	g_object_unref(gtk_app);
}


Error App::attach_main_window(Window* window) {
	if (window == nullptr) {
		return Error::NULLPTR;
	}

	//attaches a window to the  app
	this->main_window = window;
	window->set_attached_state();
	window->set_as_main_window();
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

bool App::request_subwin(const std::string& scene_name) {
	Scene* sub_scene = nullptr;
	Window* subwin = nullptr;

	for (auto& scene : sub_scene_vect) {
		if (scene->get_name() == scene_name) {
			sub_scene = scene;
			break;
		}
	}


	if (!sub_scene) {
		std::cout << "Subwin can't display sub scene cause it's a nullptr\n";
		return false;
	}

	for (auto& win : subwin_vect) {
		subwin = win;
	}


	//std::cout << "This is the window we are going to show: " << subwin;
	if (subwin->get_display_state() == true) {
		subwin->display(sub_scene);
	}
	else {
		subwin->show();
	}

	return true;
}

Error App::attach_main_scene(core::Scene* scene) {
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
	//scene->S_request_subwin.set_emit_func(signal_request_subwin);

	//rewrite the signals so you can add signals to them and chain them
	std::cout << "Signal Picked Up!" << "\n";
	S_scene_request_subwin.pickup_signal(&scene->GS_cal_button_clicked);	
	//S_scene_request_subwin.listen(scene->get_signaler(), "clicked", &signal_request_subwin);

	return Error::CLEAR;
}

Error App::attach_sub_scene(Scene* scene) {
	sub_scene_vect.push_back(scene);

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
