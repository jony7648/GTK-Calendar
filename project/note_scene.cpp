#include <iostream>
#include "note_scene.h"

namespace project {
core::Scene* create_note_scene(core::TimeComponet* time_componet) {
	const std::string scene_name = "Note Scene";
	space::Point win_size;
	win_size.x = 300;
	win_size.y = 64;

	space::Point widget_spacing;
	widget_spacing.x = 32;
	widget_spacing.y = 30;

	core::Scene* scene = new core::Scene(scene_name, widget_spacing.x, widget_spacing.y);
	scene->set_custom_dimensions(win_size);
	scene->set_resizability(true);

	return scene;	

}
}
