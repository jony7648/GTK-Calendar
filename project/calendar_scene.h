#pragma once

#include "core/scene.h"
#include "core/window.h"
#include "persist_data.h"
#include "config.h"

namespace project {
	core::Scene* create_main_scene(core::TimeComponet* time_componet, core::csv::Writer* csv_writer, data::PersistData* persist_data_object, project::Config& config);
	core::Scene* create_note_scene();
}
