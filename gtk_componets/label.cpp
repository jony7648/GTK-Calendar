#include <iostream>
#include "label.h"

namespace gtkc {

Label::Label(const std::string& name, const std::string& text, int grid_x, int grid_y, int width, int height) {
	GtkWidget* gtk_widget = gtk_label_new(text.c_str());

	set_name(name);
	set_gtk_widget(gtk_widget);
	set_grid_point(grid_x, grid_y);
	set_scale(width, height);
	this->text = text;
}

Label::~Label() {
	g_object_unref(get_gtk_widget());
}
}
