#include <iostream>
#include <gtk/gtk.h>
#include "widget.h"
#include "core/space.h"

namespace gtkc {

Widget::Widget() {
	//signaler.connect_to_signal(nullptr, "clicked", nullptr);
}

Widget::~Widget() {
	//clean up messengers

	if (_gtk_widget) {
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
	this->_gtk_widget = gtk_widget;
	_signaler.set_parent_gtk(gtk_widget);
	_signaler.set_parent_widget(this);
	//listener.set_parent_widget(this);
	//listener.set_gtk_parent(gtk_widget);
}

GtkWidget* Widget::get_gtk_widget() {
	return _gtk_widget;
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

bool Widget::get_hexpand() {
	return hexpand;
}

bool Widget::get_vexpand() {
	return vexpand;
}

const std::string& Widget::get_widget_type() {
	return _widget_type;
}

core::Signaler* Widget::get_signaler() {
	return &_signaler;
}


void Widget::signal_connect(const std::string& emit_type, void(*activate_func)()) {
	g_signal_connect(this->get_gtk_widget(), emit_type.c_str(), G_CALLBACK (activate_func), nullptr);
}

void Widget::signal_connect(const std::string& emit_type, void(*activate_func)(GtkWidget* gtk_widget)) {
	g_signal_connect(this->get_gtk_widget(), emit_type.c_str(), G_CALLBACK (activate_func), nullptr);
}

void Widget::signal_connect(const std::string& emit_type, void(*activate_func)(GtkWidget* gtk_widget), core::Messenger<Widget*>* messenger_data) {
	g_signal_connect(this->get_gtk_widget(), emit_type.c_str(), G_CALLBACK (activate_func), nullptr);

//	messenger_vector.push_back(messenger_data);
}
}
