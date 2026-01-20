#pragma once

#include <iostream>
#include <gtk/gtk.h>
#include "core/time_componet.h"
#include "gtk_componets.h"


namespace core {
class Scene {
private:
	std::string name = "";
	core::TimeComponet* time_componet;
	space::Point custom_dimensions;

public:
	gtkc::Listener<gtkc::Widget*> listener;
	Scene(const std::string& name, int widget_x_spacing, int widget_y_spacing);
	~Scene();
	void set_time_componet(core::TimeComponet* time_componet);
	core::TimeComponet* get_time_componet();
	gtkc::Container* container = nullptr;
	void set_custom_dimensions(const space::Point& dimensions);
	const space::Point& get_custom_dimensions();
};
}
