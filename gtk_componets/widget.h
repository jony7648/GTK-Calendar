#pragma once
#include <iostream>
#include <vector>
#include <gtk/gtk.h>
#include "core/space.h"
#include "core/messenger.h"
#include "gtk_componets/listener.h"

namespace gtkc {
class Widget {
private:
	std::string name = "";
	int grid_pos = 0;
	space::Rect transform;
	space::Point grid_point;
	space::Point scale;
	bool hexpand = true;
	bool vexpand = true;
	GtkWidget* gtk_widget;
	std::vector<core::Messenger<Widget*>*> messenger_vector; //vector holds the addresses of used messenger_datas, so they can be deleted when widget is freed

protected:
	std::string _widget_type = "";


public:
	Widget();
	~Widget();

	gtkc::Listener<Widget*> listener;

	void set_name(const std::string& name);
	const std::string& get_name();

	void set_gtk_widget(GtkWidget* gtk_widget);
	GtkWidget* get_gtk_widget();

	void set_scale(int x, int y);
	const space::Point& get_scale();

	void set_grid_point(int x, int y);
	const space::Point& get_grid_point();

	const std::string& get_widget_type();

	bool get_hexpand();
	bool get_vexpand();

	void set_messenger_data(core::Messenger<Widget>* messenger_data);
	core::Messenger<Widget>* get_messenger_data();
	void signal_connect(const std::string& emit_type, void(*activate_func)());
	void signal_connect(const std::string& emit_type, void(*activate_func)(GtkWidget* gtk_widget));
	void signal_connect(const std::string& emit_type, void(*activate_func)(GtkWidget* gtk_widget), core::Messenger<Widget*>* messenger_data);
};
}
