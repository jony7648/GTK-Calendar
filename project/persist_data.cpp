#include "core/csv_writer.h"
#include "persist_data.h"
#include "core/time_componet.h"

namespace data {

PersistData::PersistData() {
	
}

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
		if (note.date.year == year && note.date.month == month && note.date.day == day) {
			return true;
		}

	}
	return false;
}

void PersistData::display_notes() {
	for (Note& note : note_container) {
		std::println(
			"Day: {}, Month: {}, Year: {}, Note: {}, Should Save: {}",
			note.date.day, note.date.month, note.date.year, note.text, note.should_save
		);
	}
}

Note* PersistData::get_note(int day, int month, int year) {
	for (Note& note : note_container) {
		if (note.date.year == year && note.date.month == month && note.date.day == day) {
			return &note;
		}
	}
	
	return nullptr;
}

Note* PersistData::get_note(const core::Date& date) {
	for (Note& note : note_container) {
		if (note.date.year == date.year && note.date.month == date.month && note.date.day == date.day) {
			return &note;
		}
	}
	
	return nullptr;
}

NoteContainer& PersistData::get_note_container() {
	return note_container;
}

void get_data(core::CsvWriter& csv_writer, std::vector<core::CsvWriter::CsvMap>& data_map_vector) {
	csv_writer.read_csv(data_map_vector);
}
}
