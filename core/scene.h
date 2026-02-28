#pragma once

#include <iostream>
#include <gtk/gtk.h>
#include "core/time_componet.h"
#include "core/signal_handler.h"
#include "grid_container.h"


namespace core {
class Scene {

public:
	enum SIGNALS {
		S_READY,
		S_WINDOW_DISPLAYED,
		S_WINDOW_CLOSED,
		S_REQUEST_SUBWIN,
	};

	Scene(const std::string& name, int widget_x_spacing, int widget_y_spacing);
	~Scene();
		//Signal S_ready; // a signal that gets executed when being attached to the app object
	//Signal S_window_displayed;
	//Signal S_window_closed;
	//Signal S_request_subwin;
	//Signal GS_cal_button_clicked;
	//Signal GS_button_clicked;


	void get_tagged_widgets(std::vector<gtkc::Widget*>&, std::string tag);

	void set_time_componet(core::TimeComponet* time_componet);
	void set_resizability(bool state);
	bool get_resizability();
	void signal_request_subwin(core::Message* message);

	void emit_signal(int id);
	void add_emit_func(int id, void(*emit_func)(void*, void*, void*), void* receiver_obj);


	gtkc::GridContainer& get_widget_container();

	const std::string& get_name();
	core::TimeComponet* get_time_componet();
	void set_custom_dimensions(const space::Point& dimensions);
	const space::Point& get_custom_dimensions();

	SigHandler sig_handler;

private:

	std::string name = "";
	core::TimeComponet* time_componet;
	space::Point custom_dimensions;
	bool resizable_win = true;
	gtkc::GridContainer widget_container;
};
}
