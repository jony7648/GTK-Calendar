#include <iostream>
#include "window.h"
#include "app.h"


namespace core {
Window::Window(GtkApplication* gtk_app_ptr, const std::string& title, const space::Point& dimensions) {
	this->title = title;
	this->dimensions.x = dimensions.x;
	this->dimensions.y = dimensions.y;
	
	gtk_window = gtk_application_window_new(gtk_app_ptr);
}

Window::~Window() {
	gtk_window_close(GTK_WINDOW(gtk_window));
}

void Window::set_scene(Scene* scene) {
	if (scene != nullptr) {
		gtk_window_set_child(GTK_WINDOW(gtk_window), scene->container->get_gtk_widget());
		current_scene = scene;
	}
}

Error Window::display(Scene* scene) {
	/*
	if (current_scene == nullptr) {
		std::cout << "ERROR: current_scene is nullptr!\n";
		return;
	}
	*/

	if (scene == nullptr) {
		std::cout << "ERROR: Can't display a scene that is a nullptr\n";
		return Error::NULLPTR;
	}
	if (scene->container == nullptr) {
		std::cout << "ERROR: Scene container is a nullptr!";
		return Error::NULLPTR;
	}

	if (scene->container->get_gtk_widget() == nullptr) {
		std::cout << "ERROR: The gtk widget of the scene container is a nullptr";
		return Error::NULLPTR;
	}



	const space::Point& scene_dimensions = scene->get_custom_dimensions();

	gtk_window_set_resizable(GTK_WINDOW(gtk_window), scene->get_resizability());
	gtk_window_set_default_size(GTK_WINDOW(gtk_window), scene_dimensions.x, scene_dimensions.y);
	gtk_window_present(GTK_WINDOW(gtk_window));
	gtk_widget_set_size_request(scene->container->get_gtk_widget(), dimensions.x, dimensions.y);



	return Error::CLEAR;
}



void Window::signal_set_close(core::App* app, bool(*func)(GtkWidget* widget, gpointer user_data)) {
	if (func == nullptr) {
		return;
	}

	auto* messenger = new DoubleMessenger<App*, Window*>;
	messenger->object1 = app;
	messenger->object2 = this;

	g_signal_connect(gtk_window, "close-request", G_CALLBACK (func), messenger);
	
	//_signal_close = func;
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
