#include <iostream>
#include <fstream>
#include <gtk/gtk.h>
#include "core/file_util.h"
#include "core/space.h"
#include "widget.h"
#include "container.h"
#include "core/signal_handler.h"
#include "core/util.h"

namespace gtkc {

Widget::Widget() {
	_scale.x = 1;
	_scale.y = 1;
}

Widget::Widget(const BaseInitProperties& prop) {
	set_hexpand(prop.shared_properties.hexpand);
	set_vexpand(prop.shared_properties.vexpand);
	set_scale(prop.shared_properties.scale);
	//size_request(prop.shared_properties.size_request);
	set_tag(prop.shared_properties.tag);
	set_name(prop.name);
	set_grid_point(prop.grid_point);
	set_sig_data(prop.sig_data);
}


Widget::~Widget() {
	//clean up messengers

	if (_gtk_widget) {
		g_object_unref(get_gtk_widget());
	}
}

void Widget::attach(Container* container) {
	//std::cout << container->get_name() << "\n";
	gtk_grid_attach(GTK_GRID(container->get_gtk_widget()), _gtk_widget, _grid_point.x, _grid_point.y, _scale.x, _scale.y);
	_holder_gtk_widget = container->get_gtk_widget();
}

void Widget::reattach() {
	//will reatach a widget to it's grid_container
	g_object_ref(_gtk_widget);

	gtk_grid_remove(GTK_GRID(_holder_gtk_widget), _gtk_widget);
	gtk_grid_attach(GTK_GRID(_holder_gtk_widget), _gtk_widget, _grid_point.x, _grid_point.y, _scale.x, _scale.y);

	g_object_unref(_gtk_widget);
}

void Widget::set_css_file_name(const std::string& css_path) {
	_css_path = CSS_STYLES_DIR + css_path;
}

void Widget::show() {
	gtk_widget_set_visible(_gtk_widget, true);
	is_visible = true;
}

void Widget::hide() {
	gtk_widget_set_visible(_gtk_widget, false);
	is_visible = false;
}

bool Widget::get_visibility() {
	return is_visible;
}

void Widget::display_info() {
	g_print("Widget Name: %s, Type: %d, Tag: %s, SigData: %d, HEXPAND: %d, VEXPAND: %d, Address: %p, GTK Address: %p\n",
		_name.c_str(), (int)_type, _tag.c_str(), _sig_data, _vexpand, _hexpand, static_cast<void*>(this), static_cast<void*>(_gtk_widget)
	);
}

void Widget::set_name(const std::string& name) {
	this->_name = name;
}

const std::string& Widget::get_name() {
	return _name;
}

void Widget::set_tag(const std::string& tag) {
	_tag = tag;
}

const std::string& Widget::get_tag() {
	return _tag;
}

bool Widget::has_tag(const std::string& tag) {
	if (_tag == tag) {
		return true;
	}
	return false;
}

void Widget::set_gtk_widget(GtkWidget* gtk_widget) {
	this->_gtk_widget = gtk_widget;
	css_provider.attach_gtk_widget(gtk_widget);
	//listener.set_parent_widget(this);
	//listener.set_gtk_parent(gtk_widget);
}

GtkWidget* Widget::get_gtk_widget() {
	return _gtk_widget;
}

void Widget::set_scale(int x, int y) {
	//x denotes how many columns widget will take up
	//y denotes how many rows widget will take up
	_scale.x = x;
	_scale.y = y;
}

void Widget::set_scale(const space::Point& scale) {
	_scale.x = scale.x;
	_scale.y = scale.y;
}

void Widget::size_request(int width, int height) {
	gtk_widget_set_size_request(_gtk_widget, width, height);
}

void Widget::size_request(const space::Point& size) {
	gtk_widget_set_size_request(_gtk_widget, size.x, size.y);
}

const space::Point& Widget::get_scale() {
	return _scale;
}

void Widget::set_grid_point(int x, int y) {
	_grid_point.x = x;
	_grid_point.y = y;

	if (_presenting) {
		reattach();
	}
}

void Widget::set_grid_point(const space::Point& point) {
	_grid_point.x = point.x;
	_grid_point.y = point.y;

	if (_presenting) {
		reattach();
	}
}

const space::Point& Widget::get_grid_point() {
	return _grid_point;
}

void Widget::set_hexpand(bool hexpand) {
	this->_hexpand = hexpand;
}

void Widget::set_vexpand(bool vexpand) {
	this->_vexpand = vexpand;
}

bool Widget::get_hexpand() {
	return _hexpand;
}

bool Widget::get_vexpand() {
	return _vexpand;
}

void Widget::set_presenting(bool state) {
	_presenting = true;
}

bool Widget::is_type(Type type) {
	if (_type == type) {
		return true;
	}

	return false;
}

void Widget::set_type(Type type) {
	_type = type;
}

Type Widget::get_type() {
	return _type;
}

void Widget::set_sig_data(int data) {
	_sig_data = data;
}

int Widget::get_sig_data() {
	return _sig_data;
}
}
