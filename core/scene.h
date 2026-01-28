#pragma once

#include <iostream>
#include <gtk/gtk.h>
#include "core/time_componet.h"
#include "listener.h"
#include "gtk_componets.h"
#include "signal.h"


namespace core {
class Scene {
private:
	std::string name = "";
	core::TimeComponet* time_componet;
	space::Point custom_dimensions;
	bool resizable_win = true;
	Signaler signaler;

public:
	Scene(const std::string& name, int widget_x_spacing, int widget_y_spacing);
	~Scene();
	Signal GS_cal_button_clicked;
	Signal S_request_subwin;

	void set_time_componet(core::TimeComponet* time_componet);
	void set_resizability(bool state);
	bool get_resizability();
	void signal_request_subwin(core::Message* message);

	const std::string& get_name();
	Signaler* get_signaler();
	core::TimeComponet* get_time_componet();
	gtkc::Container* container = nullptr;
	void set_custom_dimensions(const space::Point& dimensions);
	const space::Point& get_custom_dimensions();

};
}
