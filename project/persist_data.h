#pragma once
#include <iostream>
#include "core/csv_writer.h"
#include "note_container.h"
#include "core/date.h"
#include "core/signal_handler.h"


namespace data {
struct DaySave {
	unsigned int year = 0, month = 0, day = 0;
	std::string note = "";	
};

struct EntryMarker {
	unsigned int year = 0, month = 0, day = 0;	
};



class PersistData {

	

public:
	NoteContainer note_container;
	EntryMarker& add_entry_marker();
	void store_day_save(DaySave& day_save);
	void store_entry_marker(EntryMarker& entry_marker);	
	void display_notes();
	Note* get_note(int day, int month, int year);
	Note* get_note(const core::Date& date);

	NoteContainer& get_note_container();
private:
	std::vector<DaySave> _day_save_vec;
	std::vector<EntryMarker> _entry_marker_vec;
};

extern void save_data(struct DaySave& day_save);
extern void get_data(core::csv::Writer& csv_writer, std::vector<core::csv::Entry>& data_map_vector);
}

