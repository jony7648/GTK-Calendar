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

void signal_subwin_close(void* app_addr, void* subwin_addr, void*) {
	core::App* app = static_cast<core::App*>(app_addr);
	core::Window* subwin = static_cast<core::Window*>(subwin_addr);
	core::Scene* main_scene = nullptr;
	core::Scene* sub_scene = nullptr;

	if (subwin == nullptr) {
		std::cout << "ERROR: subwin is nullptr\n";
		return;
	}


	sub_scene = subwin->get_scene();
	main_scene = app->get_main_scene();

	if (sub_scene == nullptr || main_scene == nullptr) {
		std::cout << "ERROR: the subscene or mainscene is nullptr\n";
		return;
	}

	//do some stuff in calendar to do something with window close signal
	
	main_scene->sig_handler.emit(core::Scene::S_WINDOW_CLOSED, sub_scene);
	return;
}

void signal_end_app(void* app_ptr, void* ignore_ptr, void*) {
	core::App* app = static_cast<core::App*>(app_ptr);

	std::cout << "Time to close the app\n";

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
	
	sig_handler.set_parent_object(this);
	sig_handler.add_signal(S_SCENE_REQUEST_SUBWIN);
	sig_handler.add_signal(S_WINDOW_END_PROGRAM);
	sig_handler.add_signal(S_SUBWIN_CLOSE);
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
	//S_window_end_program.pickup_signal(&window->S_end_program);
	std::cout << "ID before being bassed " << core::Window::S_END_PROGRAM << "\n";
	window->sig_handler.add_emit_func(core::Window::S_END_PROGRAM, signal_end_app, this);
	return Error::CLEAR;
}

Scene* App::get_main_scene() {
	return main_scene;	
}

void App::create_subwins(unsigned int count = 0) {
	core::Window* subwin = nullptr;
	std::string subwin_name = "";

	if (count == 0) {
		count = DEFAULT_SUBWIN_COUNT;
	}

	for (int i=0; i<count; i++) {
		subwin_name = "Subwin " + std::to_string(i);
		subwin = new core::Window(gtk_app, "Subwin ");
		//S_subwin_close.pickup_signal(&subwin->S_window_close);
		subwin->add_emit_func(core::Window::S_WINDOW_CLOSE, signal_subwin_close, this);
		subwin_vect.push_back(subwin);
	}
}

int App::attach_subwin(Window* subwin) {
	if (subwin == nullptr) {
		return 1;
	}

	subwin_vect.push_back(subwin);
	//S_subwin_close.pickup_signal(&subwin->S_window_close);
	subwin->add_emit_func(core::Window::S_WINDOW_CLOSE, signal_subwin_close, this);
	return 0;
}

bool App::request_subwin(void* emitter_obj, SigData* sig_data) {
	const std::string& scene_name = sig_data->str;

	Scene* sub_scene = nullptr;
	Scene* win_scene = nullptr;
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

	for (auto& win : subwin_vect) {
		win_scene = win->get_scene();
		
		if (win_scene == sub_scene) {
			subwin = win;
			break;
		}

		//if the win_scene is a nullptr set the subwin to that window
		//but keep searching, just in case
		if (win_scene == nullptr) {
			subwin = win;
		}
	}


	if (!subwin) {
		std::cout << "Error: a subwin couldn't be found for the sub scene " << sub_scene->get_name() << "   (requested through signal)\n";
	}


	//display window
	subwin->display(sub_scene);

	//emit scene displayed signal
	//sub_scene->S_window_displayed.emit_signal(emitter_obj, sig_data);
	
	sub_scene->sig_handler.emit_data(Scene::S_WINDOW_DISPLAYED, sig_data, emitter_obj);
	

	return true;
}

bool App::request_subwin(core::Scene* sub_scene) {
	bool found_scene = false;
	core::Window* subwin = nullptr;
	core::Scene* win_scene = nullptr;

	if (!sub_scene) {
		std::cout << "Error: Sub scene is a nullptr!\n";
		return false;
	}

	for (Scene* scene : sub_scene_vect) {
		if (scene == sub_scene) {
			found_scene = true;
			break;
		}
	}

	if (!found_scene) {
		std::cout << "ERROR: passed in sub scene could not be found in the sub scene vector!\n";
	}
	

	for (auto& win : subwin_vect) {
		win_scene = win->get_scene();
		
		if (win_scene == sub_scene) {
			subwin = win;
			break;
		}

		//if the win_scene is a nullptr set the subwin to that window
		//but keep searching, just in case
		if (win_scene == nullptr) {
			subwin = win;
		}
	}

	if (!subwin) {
		std::cout << "Error: a subwin couldn't be found for the sub scene " << sub_scene->get_name() << "   (requested thorugh scene pointer)\n";
	}


	subwin->display(sub_scene);

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
	scene->sig_handler.add_emit_func(core::Scene::S_REQUEST_SUBWIN, signal_request_subwin, this);
	scene->set_time_componet(&time_componet);

	scene->sig_handler.emit_data(Scene::S_READY, signal_data);

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

core::TimeComponet* App::get_time_componet() {
	return &time_componet;
}

const space::Point& App::get_win_dimensions() {
	return win_dimensions;
}
}
