#include <iostream>
#include <gtk/gtk.h>
#include <core/time_componet.h>
#include "scene.h"

namespace core {
Scene::Scene(const std::string& name, int widget_x_spacing, int widget_y_spacing) {
	this->name = name;
	container = new gtkc::GridContainer("MainSceneContainer", 30, 30);

	std::cout << "This is the window address: " <<  this << "\n";
	S_request_subwin.set_parent_widget(this);
	//S_request_note_win.set_emit_func(test_func);
	GS_cal_button_clicked.set_parent_widget(this);
	signaler.set_parent_widget(this);
}

Scene::~Scene() {
	delete container;	
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
