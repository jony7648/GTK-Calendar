#pragma once
#include "core/window.h"
#include "core/csv_writer.h"
#include "project/persist_data.h"

namespace project {
struct ActivateSignal {
	core::App* app = nullptr;
	core::csv::Writer* csv_writer = nullptr;
	data::PersistData* persist_data = nullptr;
};

struct ObjectSignal {
	void* object = nullptr;
};
}

namespace project::calendar {
	struct SigOpenNoteWindow {
		data::PersistData& persist_data;
		const core::Date& date;
	};
}
