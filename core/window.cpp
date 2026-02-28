#include <iostream>
#include "window.h"
#include "app.h"

bool process_close_request(GtkWidget* gtk_window, gpointer user_data) {
	bool is_main_window;
	core::Window* window = static_cast<core::Window*>(user_data);

	is_main_window = window->get_is_main_window();

	
	if (is_main_window == false) {
		//alert app and scene of the sub_window's closing
		window->get_scene()->sig_handler.emit(core::Window::S_WINDOW_CLOSE);
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
	this->title = title;
	this->dimensions.x = 30;
	this->dimensions.y = 30;
	
	gtk_window = gtk_application_window_new(gtk_app_ptr);

	sig_handler.set_parent_object(this);
	sig_handler.add_signal(S_REQUEST);
	sig_handler.add_signal(S_END_PROGRAM);
	sig_handler.add_signal(S_WINDOW_CLOSE);

	g_signal_connect(this->gtk_window, "close-request", G_CALLBACK(process_close_request), this);
	//std::cout << "This is the window address: " << this << "\n";
}

Window::~Window() {
	gtk_window_destroy(GTK_WINDOW(gtk_window));
}

void Window::emit_signal(int id, void* parent) {
	sig_handler.emit(id, parent);
}

void Window::add_emit_func(int id, void(*emit_func)(void*, void*, void*), void* receiver_obj) {
	sig_handler.add_emit_func(id, emit_func, receiver_obj);
}

void Window::set_scene(Scene* scene) {
	if (scene != nullptr) {
		gtk_window_set_child(GTK_WINDOW(gtk_window), scene->get_widget_container().get_gtk_widget());
		current_scene = scene;
	}
}

core::Scene* Window::get_scene() {
	return current_scene;
}

void Window::show() {
	gtk_widget_set_visible(gtk_window, true);
	is_visible = true;
}

void Window::hide() {
	gtk_widget_set_visible(gtk_window, false);
	is_visible = false;
}




Error Window::display(Scene* scene) {
	/*
	if (current_scene == nullptr) {
		std::cout << "ERROR: current_scene is nullptr!\n";
		return;
	}
	*/

	if (is_displaying) {
		show();
		return Error::CLEAR;
	}

	if (scene == nullptr) {
		std::cout << "ERROR: Can't display a scene that is a nullptr\n";
		return Error::NULLPTR;
	}

	if (scene->get_widget_container().get_gtk_widget() == nullptr) {
		std::cout << "ERROR: The gtk widget of the scene container is a nullptr";
		return Error::NULLPTR;
	}



	const space::Point& scene_dimensions = scene->get_custom_dimensions();

	current_scene = scene;
	gtk_window_set_resizable(GTK_WINDOW(gtk_window), scene->get_resizability());
	gtk_window_set_default_size(GTK_WINDOW(gtk_window), scene_dimensions.x, scene_dimensions.y);
	gtk_window_present(GTK_WINDOW(gtk_window));
	gtk_window_set_child(GTK_WINDOW(get_gtk_window()), scene->get_widget_container().get_gtk_widget());
	//gtk_widget_set_size_request(scene->widget_container.get_gtk_widget(), dimensions.x, dimensions.y);


	is_displaying = true;

	return Error::CLEAR;
}



void Window::signal_set_close(core::App* app, bool(*func)(GtkWidget* widget, gpointer user_data)) {
	if (func == nullptr) {
		return;
	}

	//YOU NEED TO REWRITE THIS AS THIS IS A MEMORY LEAK

	auto* messenger = new DoubleMessenger<App*, Window*>;
	messenger->object1 = app;
	messenger->object2 = this;

	g_signal_connect(gtk_window, "close-request", G_CALLBACK (func), messenger);
	
	//_signal_close = func;
}

void Window::set_attached_state() {
	is_attached = true;
}

bool Window::get_attached_state() {
	return is_attached;
}

bool Window::get_display_state() {
	return is_displaying;
}

bool Window::get_visibility() {
	return is_visible;
}

void Window::set_as_main_window() {
	is_main_window = true;
}

bool Window::get_is_main_window() {
	return is_main_window;
}

void Window::set_dimensions(const space::Point& dimensions) {
	this->dimensions = dimensions;
}

GtkWidget* Window::get_gtk_window() {
	return gtk_window;
}
}
