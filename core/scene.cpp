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

	sig_handler.set_parent_object(this);
	sig_handler.add_signal(S_READY);
	sig_handler.add_signal(S_WINDOW_DISPLAYED);
	sig_handler.add_signal(S_WINDOW_CLOSED);
	sig_handler.add_signal(S_REQUEST_SUBWIN);
}
Scene::~Scene() {

}


void Scene::set_custom_dimensions(const space::Point& dimensions) {
	this->custom_dimensions = dimensions;
}

const space::Point& Scene::get_custom_dimensions() const {
	return custom_dimensions;
}

void Scene::set_resizability(bool state) {
	resizable_win = state;
}

bool Scene::get_resizability() const {
	return resizable_win;
}

gtkc::GridContainer& Scene::get_widget_container() {
	return widget_container;	
}

void Scene::set_parent_win(core::Window* p_win) {
	p_parent_win = p_win;
}

void Scene::set_takes_focus(bool focus) {
	_takes_focus = focus;
}

bool Scene::get_takes_focus() {
	return _takes_focus;
}

core::Window& Scene::get_parent_win() {
	return *p_parent_win;
}

const std::string& Scene::get_name() const {
	return name;	
}

void Scene::set_time_componet(core::TimeComponet* time_componet) {
	this->time_componet = time_componet;
}

core::TimeComponet* Scene::get_time_componet() {
return time_componet;
}
}
