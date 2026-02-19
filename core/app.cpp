#include <iostream>
#include <gtk/gtk.h>
#include "file_util.h"
#include "app.h"

bool is_custom_point(const space::Point& point) {
	if (point.x == 0 && point.y == 0) {
		return true;	
	}

	return false;	
}

void signal_request_subwin(void* app_addr, void* emitter_obj, void* sig_data_addr) {
	core::App* app = static_cast<core::App*>(app_addr);
	gtkc::Widget* widget = static_cast<gtkc::Widget*>(emitter_obj);
	core::SigData* sig_data = static_cast<core::SigData*>(sig_data_addr);


	app->request_subwin(emitter_obj, sig_data);
}

void signal_end_app(void* app_ptr, void* ignore_ptr) {
	core::App* app = static_cast<core::App*>(app_ptr);

	//gtk_window_destroy(GTK_WINDOW(app->main_window->get_gtk_window()));

	//std::cout << "App: " << app << "\n";
	app->close();
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

	S_window_end_program.set_parent_widget(this);
	S_window_end_program.set_emit_func(&signal_end_app);
}

App::~App() {
	for (auto& win : subwin_vect) {
		//delete win;
	}


	//delete main_window;
	//g_object_unref(gtk_app);
}


Error App::attach_main_window(Window* window) {
	if (window == nullptr) {
		return Error::NULLPTR;
	}

	//attaches a window to the  app
	this->main_window = window;
	window->set_attached_state();
	window->set_as_main_window();
	S_window_end_program.pickup_signal(&window->S_end_program);
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

bool App::request_subwin(void* emitter_obj, SigData* sig_data) {
	const std::string& scene_name = sig_data->str;

	Scene* sub_scene = nullptr;
	Window* subwin = nullptr;

	//check for the subscenes existance
	for (auto& scene : sub_scene_vect) {
		std::cout << "Scene Name" << scene_name << "\n";
		if (scene && scene->get_name() == scene_name) {
			sub_scene = scene;
			break;
		}
	}


	//if the sub scene couldn't be found
	if (!sub_scene) {
		std::cout << "Subwin can't display sub scene cause it's a nullptr\n";
		return false;
	}


	//prepare a win for the specified scene
	for (auto& win : subwin_vect) {
		subwin = win;
	}


	//display window
	if (subwin->get_display_state() == false) {
		subwin->display(sub_scene);
	}
	else if (subwin->get_visibility() == false) {
		subwin->show();
	}

	sub_scene->S_window_displayed.emit_signal(emitter_obj, sig_data);

	//subwin->get_scene

	return true;
}

Error App::attach_main_scene(core::Scene* scene, void* signal_data) {
	if (scene == nullptr) {
		std::cout << "ERROR: the new scene is a nullptr!\n";
		return Error::NULLPTR;
	}

	if (main_window == nullptr) {
		std::cout << "ERROR: app does not have a main window. Set main window before main scene\n";
		return Error::NULLPTR;
	}

	//gtk_window_set_child(GTK_WINDOW(main_window->get_gtk_window()), scene->widget_container.get_gtk_widget());

	if (main_scene != nullptr) {
		delete main_scene;
	}

	main_scene = scene;
	//scene->S_request_subwin.set_emit_func(signal_request_subwin);

	//rewrite the signals so you can add signals to them and chain them
	std::cout << "Signal Picked Up!" << "\n";
	S_scene_request_subwin.pickup_signal(&scene->S_request_subwin);	
	scene->set_time_componet(&time_componet);
	scene->S_ready.emit_signal(signal_data);

	//S_scene_request_subwin.listen(scene->get_signaler(), "clicked", &signal_request_subwin);

	return Error::CLEAR;
}

Error App::attach_sub_scene(Scene* scene) {
	scene->set_time_componet(&time_componet);
	sub_scene_vect.push_back(scene);

	return Error::CLEAR;	
}

void App::display_main_window() {
	main_window->display(main_scene);
}

void App::apply_provider(const std::string& css_dir_path) {
	//use file_util to loop thorugh and grab content from all css files

	_css_provider = gtk_css_provider_new();
	_default_display = gdk_display_get_default();

	std::vector<std::string> file_vector(10);

	file_util::get_files_in_dir(file_vector, css_dir_path);

	for (std::string& path : file_vector) {
		if (path == "") {
			continue;
		}

		std::cout << "Path: " << path << "\n";	
	}

	gtk_css_provider_load_from_path(_css_provider, "test.css");
	gtk_style_context_add_provider_for_display(_default_display, GTK_STYLE_PROVIDER(_css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

GtkWidget* App::get_scene_container() {
	return main_scene->get_widget_container().get_gtk_widget();
}

void App::run(void(*activate_func)(GtkApplication*, gpointer), void* activate_signal) {
	g_signal_connect(gtk_app, "activate", G_CALLBACK(activate_func), activate_signal);

	app_status = g_application_run(G_APPLICATION(gtk_app), 0, argv);
}

void App::close() {
	//gtk_window_destroy(GTK_WINDOW(main_window->get_gtk_window()));
	
	delete main_window;

	for (Window* window : subwin_vect) {
		delete window;
	}
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
