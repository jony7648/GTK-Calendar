#pragma once

#include "core/scene.h"
#include "core/window.h"

namespace calender {
	extern core::Scene* create_main_scene(core::Window* window, core::TimeComponet* time_componet);
	extern core::Scene* create_note_scene();
}
