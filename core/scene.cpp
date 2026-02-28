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


void Scene::emit_signal(int id) {
	sig_handler.emit(id);
}

void Scene::add_emit_func(int id, void(*emit_func)(void*, void*, void*), void* receiver_obj) {
	sig_handler.add_emit_func(id, emit_func, receiver_obj);
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

gtkc::GridContainer& Scene::get_widget_container() {
	return widget_container;	
}


const std::string& Scene::get_name() {
	return name;	
}

void Scene::set_time_componet(core::TimeComponet* time_componet) {
	this->time_componet = time_componet;
}

core::TimeComponet* Scene::get_time_componet() {
	return time_componet;
}
}
