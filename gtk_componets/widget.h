#pragma once
#include <iostream>
#include <gtk/gtk.h>
#include "core/space.h"

namespace gtkc {
class Widget {
private:
	std::string name = "";
	int grid_pos = 0;
	space::Rect transform;
	space::Point grid_point;
	space::Point scale;
	GtkWidget* gtk_widget;
	std::string gtk_widget_type = "";

public:
	Widget();
	~Widget();

	void set_name(const std::string& name);
	const std::string& get_name();

	void set_gtk_widget(GtkWidget* gtk_widget);
	GtkWidget* get_gtk_widget();

	void set_scale(int x, int y);
	const space::Point& get_scale();

	void set_grid_point(int x, int y);
	const space::Point& get_grid_point();



	const std::string& get_gtk_widget_type();
};
}
