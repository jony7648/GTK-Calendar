#include <iostream>
#include "window.h"
#include "app.h"

bool process_close_request(GtkWidget* gtk_window, gpointer user_data) {
	core::Window* window = static_cast<core::Window*>(user_data);
	
	if (!window->is_main()) {
		core::Scene* win_scene = window->get_scene();

		if (!win_scene) {
			return true;
		}


		//alert app and scene of the sub_window's closing
		win_scene->sig_handler.emit(core::Window::S_WINDOW_CLOSE);
		window->sig_handler.emit(core::Window::S_WINDOW_CLOSE);

		window->hide();
		
		return true;
	}

	window->sig_handler.emit(core::Window::S_END_PROGRAM);
	//gtk_window_destroy(GTK_WINDOW(gtk_window));
	return true;	
}


namespace core {
Window::Window(GtkApplication* gtk_app_ptr, const std::string& title) {
	this->_title = title;
	this->_dimensions.x = 30;
	this->_dimensions.y = 30;
	
	_gtk_window = gtk_application_window_new(gtk_app_ptr);

	sig_handler.set_parent_object(this);
	sig_handler.add_signal(S_REQUEST);
	sig_handler.add_signal(S_END_PROGRAM);
	sig_handler.add_signal(S_WINDOW_CLOSE);

	g_signal_connect(this->_gtk_window, "close-request", G_CALLBACK(process_close_request), this);
	//std::cout << "This is the window address: " << this << "\n";
	

	_css_provider.load_file("window.css", _gtk_window);
}

Window::~Window() {
	gtk_window_destroy(GTK_WINDOW(_gtk_window));
}


void Window::set_scene(Scene* scene) {
	if (scene != nullptr) {
		gtk_window_set_child(GTK_WINDOW(_gtk_window), scene->get_widget_container().get_gtk_widget());
		_p_current_scene = scene;
	}
}

core::Scene* Window::get_scene() {
	return _p_current_scene;
}

void Window::show() {
	gtk_widget_set_visible(_gtk_window, true);
	_is_visible = true;
}

void Window::hide() {
	gtk_widget_set_visible(_gtk_window, false);
	_is_visible = false;
}

Error Window::display(Scene& scene) {
	/*
	if (p_current_scene == nullptr) {
		std::cout << "ERROR: p_current_scene is nullptr!\n";
		return;
	}
	*/

	if (_is_displaying) {
		show();
		return Error(ErrorType::Clear);
	}

	/*
	if (scene == nullptr) {
		std::cout << "ERROR: Can't display a scene that is a nullptr\n";
		return Error(ErrorType::Nullptr);
	}

	if (scene->get_widget_container().get_gtk_widget() == nullptr) {
		std::cout << "ERROR: The gtk widget of the scene container is a nullptr";
		return Error(ErrorType::Nullptr);
	}
	*/

	const space::Point& scene_dimensions = scene.get_custom_dimensions();

	_p_current_scene = &scene;
	gtk_window_set_resizable(GTK_WINDOW(_gtk_window), scene.get_resizability());
	gtk_window_set_default_size(GTK_WINDOW(_gtk_window), scene_dimensions.x, scene_dimensions.y);
	gtk_window_present(GTK_WINDOW(_gtk_window));
	gtk_window_set_child(GTK_WINDOW(get_gtk_window()), scene.get_widget_container().get_gtk_widget());
	//gtk_widget_set_size_request(scene->widget_container.get_gtk_widget(), dimensions.x, dimensions.y);

	_is_displaying = true;

	return Error(ErrorType::Clear);
}

void Window::set_sensitivity(bool state) {
	gtk_widget_set_sensitive(GTK_WIDGET(_gtk_window), state);
}

void Window::set_attached_state() {
	_is_attached = true;
}

bool Window::get_attached_state() {
	return _is_attached;
}

bool Window::get_display_state() {
	return _is_displaying;
}

bool Window::get_visibility() {
	return _is_visible;
}

void Window::set_type(Type type) {
	_type = type;
}

bool Window::is_type(Type type) {
	if (_type == type) {
		return true;
	}
	return false;
}

bool Window::is_main() {
	if (_type == Type::Main) {
		return true;
	}
	return false;
}

Window::Type Window::get_type() {
	return _type;
}

void Window::set_dimensions(const space::Point& dimensions) {
	this->_dimensions = dimensions;
}

GtkWidget* Window::get_gtk_window() {
	return _gtk_window;
}
}
