#pragma once

#include <iostream>
#include <gtk/gtk.h>
#include "core/time_componet.h"
#include "grid_container.h"
#include "signal.h"


namespace core {
class Scene {
private:
	std::string name = "";
	core::TimeComponet* time_componet;
	space::Point custom_dimensions;
	bool resizable_win = true;
	Signaler signaler;
	gtkc::GridContainer widget_container;

public:
	Scene(const std::string& name, int widget_x_spacing, int widget_y_spacing);
	~Scene();
	Signal GS_cal_button_clicked;
	Signal GS_button_clicked;
	//Signal S_request_subwin;
	Signal S_ready; // a signal that gets executed when being attached to the app object
	Signal S_window_displayed;
	Signal S_window_closed;
	Signal S_request_subwin;


	void get_tagged_widgets(std::vector<gtkc::Widget*>&, std::string tag);

	void set_time_componet(core::TimeComponet* time_componet);
	void set_resizability(bool state);
	bool get_resizability();
	void signal_request_subwin(core::Message* message);


	gtkc::GridContainer& get_widget_container();

	const std::string& get_name();
	Signaler* get_signaler();
	core::TimeComponet* get_time_componet();
	void set_custom_dimensions(const space::Point& dimensions);
	const space::Point& get_custom_dimensions();

};
}
