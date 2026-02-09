#pragma once

#include "core/scene.h"
#include "core/window.h"

namespace project {
	extern core::Scene* create_main_scene(core::TimeComponet* time_componet);
	extern core::Scene* create_note_scene();
}
