#pragma once

#include <iostream>
#include <gtk/gtk.h>
#include "core/time_componet.h"
#include "gtk_componets.h"


class Scene {
private:
	std::string name = "";
	core::TimeComponet* time_componet;

public:
	Scene(const std::string& name, int widget_x_spacing, int widget_y_spacing);
	~Scene();
	void set_time_componet(core::TimeComponet* time_componet);
	core::TimeComponet* get_time_componet();
	gtkc::Container* container = nullptr;
};
