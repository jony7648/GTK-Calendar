#include <iostream>
#include <gtk/gtk.h>
#include "grid_container.h"

namespace gtkc {

GridContainer::GridContainer() {
	GtkWidget* gtk_grid = gtk_grid_new();
	set_gtk_widget(gtk_grid);
	set_type(Type::GridContainer);
}

GridContainer::GridContainer(const std::string& name, int size_x, int size_y) {
	GtkWidget* gtk_grid = gtk_grid_new();

	grid_size.x = size_x;
	grid_size.y = size_y;

	set_name(name);
	set_gtk_widget(gtk_grid);
	set_type(Type::GridContainer);
}

GridContainer* GridContainer::get_child_container(const std::string& name) {
	GridContainer* ret_widget = nullptr;

	for (Widget* widget : *this) {
		if (widget != nullptr && widget->get_name() == name) {
			ret_widget = static_cast<GridContainer*>(widget);
		}
		
	}

	return ret_widget;
}

void GridContainer::set_column_homogeneous(bool state) {
	gtk_grid_set_column_homogeneous(GTK_GRID(_gtk_widget), state);
}

const space::Point& GridContainer::get_grid_size() {
	return grid_size;	
}


}
