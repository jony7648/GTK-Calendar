#include "core/csv_writer.h"
#include "persist_data.h"

namespace data {



Note& PersistData::add_note() {
	Note& note = note_container.add_note();
	return note;
}

EntryMarker& PersistData::add_entry_marker() {
	_entry_marker_vec.push_back(EntryMarker());
	return _entry_marker_vec.back();
}

void PersistData::store_day_save(DaySave& day_save) {
	_day_save_vec.push_back(day_save);
}

void PersistData::store_entry_marker(EntryMarker& entry_marker) {
	_entry_marker_vec.push_back(entry_marker);
}

bool PersistData::note_exists(int day, int month, int year) {
	for (Note& note : note_container) {
		if (note.year == year && note.month == month && note.day == day) {
			return true;
		}

	}
	return false;
}

void PersistData::display_notes() {
	for (Note& note : note_container) {
		std::println(
			"Day: {}, Month: {}, Year: {}, Note: {}",
			note.day, note.month, note.year, note.text
		);
	}
}

Note* PersistData::get_note(int day, int month, int year) {
	for (Note& note : note_container) {
		if (note.year == year && note.month == month && note.day == day) {
			return &note;
		}
	}
	
	return nullptr;
}

void save_data(struct DaySave& day_save) {
	core::CsvWriter csv_writer("test.save");

	std::vector<core::CsvWriter::DataMap> data_map_vector;

	
	//data_map["year"] = std::to_string(day_save.year);
	//data_map["month"] = std::to_string(day_save.month);
	//data_map["day"] = std::to_string(day_save.day);
	//data_map["note"] = day_save.note;
	

	//csv_writer.save_map(data_map);
	
	csv_writer.get_csv_data(data_map_vector);
}



void get_data(core::CsvWriter& csv_writer, std::vector<core::CsvWriter::DataMap>& data_map_vector) {
	csv_writer.get_csv_data(data_map_vector);
}
}
