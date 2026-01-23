#include <iostream>
#include <gtk/gtk.h>
#include <core/time_componet.h>
#include "scene.h"

namespace core {
Scene::Scene(const std::string& name, int widget_x_spacing, int widget_y_spacing) {
	this->name = name;
	container = new gtkc::GridContainer("MainSceneContainer", 30, 30);
	listener.set_parent_widget(this);
	//gtk_container->widget_spacing.x = widget_x_spacing;
	//this->widget_spacing.y = widget_y_spacing;
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

void Scene::set_time_componet(core::TimeComponet* time_componet) {
	this->time_componet = time_componet;
}

core::TimeComponet* Scene::get_time_componet() {
	return time_componet;
}
}
