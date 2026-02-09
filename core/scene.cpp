#include <iostream>
#include <gtk/gtk.h>
#include <core/time_componet.h>
#include "scene.h"

namespace core {
Scene::Scene(const std::string& name, int widget_x_spacing, int widget_y_spacing) {
	this->name = name;
	widget_container.set_name(name);
	widget_container.set_widget_spacing(widget_x_spacing, widget_y_spacing);
	widget_container.set_halign(GTK_ALIGN_START);

	S_request_subwin.set_parent_widget(this);
	//S_request_note_win.set_emit_func(test_func);
	GS_cal_button_clicked.set_parent_widget(this);
	GS_button_clicked.set_parent_widget(this);
	signaler.set_parent_widget(this);
}

Scene::~Scene() {

}

void Scene::get_tagged_widgets(std::vector<gtkc::Widget*>& ret_vec, std::string tag) {
	widget_container.get_tagged_widgets(ret_vec, tag);
}

void Scene::set_custom_dimensions(const space::Point& dimensions) {
	this->custom_dimensions = dimensions;
}

const space::Point& Scene::get_custom_dimensions() {
	return custom_dimensions;
}

void Scene::set_resizability(bool state) {
	resizable_win = state;
}

bool Scene::get_resizability() {
	return resizable_win;
}

void Scene::signal_request_subwin(core::Message* message) {
	
}

const std::string& Scene::get_name() {
	return name;	
}

Signaler* Scene::get_signaler() {
	return &signaler;
}

void Scene::set_time_componet(core::TimeComponet* time_componet) {
	this->time_componet = time_componet;
}

core::TimeComponet* Scene::get_time_componet() {
	return time_componet;
}
}
