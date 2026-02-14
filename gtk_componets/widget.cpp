#include <iostream>
#include <fstream>
#include <gtk/gtk.h>
#include "core/file_util.h"
#include "core/space.h"
#include "widget.h"
#include "container.h"

namespace gtkc {

Widget::Widget() {
	_scale.x = 1;
	_scale.y = 1;
	//signaler.connect_to_signal(nullptr, "clicked", nullptr);
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

void Widget::apply_provider() {
	//this function will style the widget based on the properties
	//set in it's css file

	if (_css_path.empty()) {
		//std::cout << "ERROR: css_path not set!\n";
		return;
	}
	
	gtk_css_provider_load_from_path(_css_provider, _css_path.c_str());
	gtk_style_context_add_provider_for_display(_default_display, GTK_STYLE_PROVIDER(_css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void Widget::load_css() {
	if (!_gtk_widget) {
		std::cout << "ERROR: can't css style gtk_widget is a nullptr\n";
	}

	if (!_css_provider) {
		std::cout << "Error: _css_provider is a nullptr!\n";
	}

	const std::string& css_class = get_type();

	gtk_widget_add_css_class(_gtk_widget, css_class.c_str());
}

void Widget::load_css(const std::string& class_name) {
	if (!_gtk_widget) {
		std::cout << "ERROR: can't css style gtk_widget is a nullptr\n";
	}

	if (!_css_provider) {
		std::cout << "Error: _css_provider is a nullptr!\n";
	}

	gtk_widget_add_css_class(_gtk_widget, class_name.c_str());
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
	std::println("Widget Name: {}, Tag: {}, SigData: {}, HEXPAND: {}, VEXPAND: {}, Address: {}, GTK Address: {}\n",
		_name, _tag, _sig_data, _vexpand, _hexpand, static_cast<void*>(this), static_cast<void*>(_gtk_widget)
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
	_css_provider = gtk_css_provider_new();
	_default_display = gdk_display_get_default();
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
	_scale.x = x;
	_scale.y = y;
}

void Widget::size_request(int width, int height) {
	gtk_widget_set_size_request(_gtk_widget, width, height);
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

bool Widget::is_type(const std::string& type) {
	if (_type == type) {
		return true;
	}

	return false;
}

const std::string& Widget::get_type() {
	return _type;
}

void Widget::set_sig_data(int data) {
	_sig_data = data;
}

int Widget::get_sig_data() {
	return _sig_data;
}

core::Signaler* Widget::get_signaler() {
	return &_signaler;
}
}
