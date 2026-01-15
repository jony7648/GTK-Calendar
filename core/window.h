#pragma once
#include <iostream>
#include <gtk/gtk.h>
#include "space.h"
#include "scene.h"

namespace core {
class Window {
private:
	std::string title = "";
	space::Point dimensions;
	GtkWidget* gtk_window = nullptr;
	Scene* current_scene = nullptr;



public:
	GtkWidget* get_gtk_window();
	Window(GtkApplication* gtk_app_ptr, const std::string& title, const space::Point& dimensions);
	~Window();
	void display(GtkWidget* gtk_container);


	void set_scene(Scene* scene);
};
}
