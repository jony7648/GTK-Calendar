#include <iostream>

#include "file_util.h"
#include "app.h"

#include <gtk/gtk.h>

bool is_custom_point(const space::Point& point) {
	if (point.x == 0 && point.y == 0) {
		return true;	
	}

	return false;	
}

void signal_request_subwin(core::EmitData<core::Scene>& emit_data) {
	core::App* app = static_cast<core::App*>(emit_data.receiver);
	core::Window& emit_window = emit_data.holder->get_parent_win();
	gtkc::Widget* widget = static_cast<gtkc::Widget*>(emit_data.emitter);
	auto* sig_data = static_cast<core::Scene::SigRequestSubwin*>(emit_data.sig_data);
	core::Scene* p_sub_scene = nullptr;


	if (!widget || !sig_data) {
		return;
	}

	for (core::Scene* scene : app->get_sub_scene_vect()) {
		if (scene->get_name() == sig_data->scene_name) {
			p_sub_scene = scene;
		}
	}

	if (!p_sub_scene) {
		return;
	}

	app->request_subwin(*p_sub_scene, &emit_window, widget, sig_data);
}

void signal_subwin_close(core::EmitData<core::Window> emit_data) {
	core::App* app = static_cast<core::App*>(emit_data.receiver);
	core::Window* subwin = emit_data.holder;
	core::Scene* main_scene = nullptr;
	core::Scene* sub_scene = nullptr;

	if (subwin == nullptr ) {
		std::cout << "ERROR: closed subwin is nullptr\n";
		return;
	}


	sub_scene = subwin->get_scene();
	main_scene = app->get_main_scene();

	if (sub_scene == nullptr || main_scene == nullptr) {
		std::cout << "ERROR: the subscene or mainscene is nullptr\n";
		return;
	}

	//do some stuff in calendar to do something with window close signal

	main_scene->sig_handler.emit_data(core::Scene::S_WINDOW_CLOSED, sub_scene);
	
	return;
}

void signal_end_app(core::EmitData<core::Window> emit_data) {
	core::App* app = static_cast<core::App*>(emit_data.receiver);

	std::cout << "Time to close the app\n";

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
		return Error(ErrorType::Nullptr);
	}

	//attaches a window to the  app
	this->main_window = window;
	window->set_attached_state();
	window->set_as_main_window();
	//S_window_end_program.pickup_signal(&window->S_end_program);
	std::cout << "ID before being bassed " << core::Window::S_END_PROGRAM << "\n";
	window->sig_handler.add_emit_func(core::Window::S_END_PROGRAM, signal_end_app, this);
	return Error(ErrorType::Clear);
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
		subwin->sig_handler.add_emit_func(core::Window::S_WINDOW_CLOSE, signal_subwin_close, this);
		subwin_vect.push_back(subwin);
	}
}

int App::attach_subwin(Window* subwin) {
	if (subwin == nullptr) {
		return 1;
	}

	subwin_vect.push_back(subwin);
	//S_subwin_close.pickup_signal(&subwin->S_window_close);
	subwin->sig_handler.add_emit_func(core::Window::S_WINDOW_CLOSE, signal_subwin_close, this);
	return 0;
}

Error App::request_subwin(core::Scene& sub_scene, core::Window* emit_window, void* emitter_obj, core::Scene::SigRequestSubwin* sig_data) {
	bool found_scene = false;
	core::Window* p_subwin = nullptr;
	core::Scene* p_win_scene = nullptr;

	for (Scene* p_scene : sub_scene_vect) {
		if (p_scene == &sub_scene) {
			found_scene = true;
			break;
		}
	}

	if (!found_scene) {
		//std::cout << "ERROR: passed in sub scene could not be found in the sub scene vector!\n";
		return Error(ErrorType::SceneNotFound);
	}
	

	for (Window* p_win : subwin_vect) {
		p_win_scene = p_win->get_scene();
		
		if (p_win_scene == &sub_scene) {
			p_subwin = p_win;
			break;
		}

		//if the win_scene is a nullptr set the subwin to that window
		//but keep searching, just in case
		if (p_win_scene == nullptr) {
			p_subwin = p_win;
		}
	}

	if (!p_subwin) {
		std::cout << "Error: a subwin couldn't be found for the sub scene " << sub_scene.get_name() << "   (requested thorugh scene pointer)\n";
		return Error(ErrorType::SubwinNotFound);
	}

	//if there is an emmiter object emit the window requested signal
	if (emitter_obj) {
		sub_scene.sig_handler.emit_data(Scene::S_WINDOW_DISPLAYED, sig_data->sig_ptr, emitter_obj);
	}


	p_subwin->display(sub_scene);

	return Error(ErrorType::Clear);
}

Error App::attach_main_scene(core::Scene* scene, void* signal_data) {
	if (scene == nullptr) {
		std::cout << "ERROR: the new scene is a nullptr!\n";
		return Error(ErrorType::Nullptr);
	}

	if (main_window == nullptr) {
		std::cout << "ERROR: app does not have a main window. Set main window before main scene\n";
		return Error(ErrorType::Nullptr);
	}

	//gtk_window_set_child(GTK_WINDOW(main_window->get_gtk_window()), scene->widget_container.get_gtk_widget());

	if (main_scene != nullptr) {
		delete main_scene;
	}

	main_scene = scene;
	//scene->S_request_subwin.set_emit_func(signal_request_subwin);

	//rewrite the signals so you can add signals to them and chain them
	scene->sig_handler.add_emit_func(core::Scene::S_REQUEST_SUBWIN, signal_request_subwin, this);
	scene->set_time_componet(&time_componet);

	scene->sig_handler.emit_data(Scene::S_READY, signal_data);

	//S_scene_request_subwin.listen(scene->get_signaler(), "clicked", &signal_request_subwin);

	return Error(ErrorType::Clear);
}

Error App::attach_sub_scene(Scene* scene) {
	scene->set_time_componet(&time_componet);
	sub_scene_vect.push_back(scene);

	return Error(ErrorType::Clear);	
}

void App::display_main_window() {
	if (main_scene) {
		main_window->display(*main_scene);
	}

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
std::vector<core::Scene*>& App::get_sub_scene_vect() {
	return sub_scene_vect;
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
