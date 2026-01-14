#include <iostream>
#include <gtk/gtk.h>
#include "widget.h"
#include "core/space.h"

namespace gtkc {

Widget::Widget() {
	
}

Widget::~Widget() {
	if (get_gtk_widget() == nullptr) {
		g_object_unref(get_gtk_widget());
	}
}

void Widget::set_name(const std::string& name) {
	this->name = name;
}

const std::string& Widget::get_name() {
	return name;
}


void Widget::set_gtk_widget(GtkWidget* gtk_widget) {
	this->gtk_widget = gtk_widget;
}

GtkWidget* Widget::get_gtk_widget() {
	return gtk_widget;
}

void Widget::set_scale(int x, int y) {
	//x denotes how many columns widget will take up
	//y denotes how many rows widget will take up
	scale.x = x;
	scale.y = y;
}

const space::Point& Widget::get_scale() {
	return scale;
}

void Widget::set_grid_point(int x, int y) {
	grid_point.x = x;
	grid_point.y = y;
}

const space::Point& Widget::get_grid_point() {
	return grid_point;
}

}
