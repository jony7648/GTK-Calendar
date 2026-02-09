#include <iostream>
#include <gtk/gtk.h>
#include "grid_container.h"

namespace gtkc {

GridContainer::GridContainer() {
	GtkWidget* gtk_grid = gtk_grid_new();
	set_gtk_widget(gtk_grid);
	_type = "Grid Container";
}

GridContainer::GridContainer(const std::string& name, int size_x, int size_y) {
	GtkWidget* gtk_grid = gtk_grid_new();

	grid_size.x = size_x;
	grid_size.y = size_y;

	set_name(name);
	set_gtk_widget(gtk_grid);
	_type = "Grid Container";
}

const space::Point& GridContainer::get_grid_size() {
	return grid_size;	
}
}
