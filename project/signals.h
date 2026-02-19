#pragma once
#include "core/window.h"
#include "core/csv_writer.h"
#include "project/persist_data.h"

namespace project {
struct ActivateSignal {
	core::App* app = nullptr;
	core::CsvWriter* csv_writer = nullptr;
	data::PersistData* persist_data = nullptr;
};
}
