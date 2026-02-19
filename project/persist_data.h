#pragma once
#include <iostream>
#include "core/csv_writer.h"
#include "note_container.h"


namespace data {
struct DaySave {
	unsigned int year = 0, month = 0, day = 0;
	std::string note = "";	
};

struct EntryMarker {
	unsigned int year = 0, month = 0, day = 0;	
};



class PersistData {
private:
	std::vector<DaySave> _day_save_vec;
	std::vector<EntryMarker> _entry_marker_vec;
	NoteContainer note_container;
public:
	Note& add_note();
	EntryMarker& add_entry_marker();
	void store_day_save(DaySave& day_save);
	void store_entry_marker(EntryMarker& entry_marker);	
	bool note_exists(int day, int month, int year);
	void display_notes();
	Note* get_note(int day, int month, int year);
};

extern void save_data(struct DaySave& day_save);
extern void get_data(core::CsvWriter& csv_writer, std::vector<core::CsvWriter::DataMap>& data_map_vector);
}

