#pragma once

#include <iostream>
#include <gtk/gtk.h>
#include "core/time_componet.h"
#include "listener.h"
#include "gtk_componets.h"


namespace core {
class Scene {
private:
	std::string name = "";
	core::TimeComponet* time_componet;
	space::Point custom_dimensions;
	bool resizable_win = true;

public:
	core::Listener listener;
	Scene(const std::string& name, int widget_x_spacing, int widget_y_spacing);
	~Scene();
	void set_time_componet(core::TimeComponet* time_componet);
	void set_resizability(bool state);
	bool get_resizability();
	core::TimeComponet* get_time_componet();
	gtkc::Container* container = nullptr;
	void set_custom_dimensions(const space::Point& dimensions);
	const space::Point& get_custom_dimensions();
};
}
