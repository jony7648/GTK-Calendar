#include <iostream>
#include "window.h"


Window::Window(GtkApplication* gtk_app_ptr, const std::string& title, const space::Point& dimensions) {
	this->title = title;
	this->dimensions.x = dimensions.x;
	this->dimensions.y = dimensions.y;
	
	gtk_window = gtk_application_window_new(gtk_app_ptr);
}

Window::~Window() {

}

void Window::set_scene(Scene* scene) {
	if (scene != nullptr) {
		gtk_window_set_child(GTK_WINDOW(gtk_window), scene->container->get_gtk_widget());
		current_scene = scene;
	}
}

void Window::display() {
	if (current_scene == nullptr) {
		std::cout << "ERROR: current_scene is nullptr!\n";
		return;
	}

	GtkWidget* gtk_container = current_scene->container->get_gtk_widget();
	//
	//
	//

	gtk_window_set_default_size(GTK_WINDOW(gtk_window), dimensions.x, dimensions.y);
	gtk_window_present(GTK_WINDOW(gtk_window));
	gtk_widget_set_size_request(gtk_container, dimensions.x, dimensions.y);
}

GtkWidget* Window::get_gtk_window() {
	return gtk_window;
}
