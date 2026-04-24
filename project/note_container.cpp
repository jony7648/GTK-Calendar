#include "note_container.h"
#include "core/util.h"
#include "core/time_componet.h"

#include <glib.h>

#define SAVE_ADJUST_COUNT 2
#define MONTHS_PER_YEAR 12

int calculate_month_distance(const data::NoteContainer::SaveAnchor& save_anchor, const core::Date& compare_date) {
	int month_dist = abs(
		compare_date.month - save_anchor.month //+ 
		//(compare_date.year - save_anchor.year) * MONTHS_PER_YEAR		
	);

	return month_dist;
}

static void signal_time_componet_menu_date_advance(core::EmitData<core::TimeComponet>& emit_data) {
	core::TimeComponet::SigMenuDateChanged* sig_data = static_cast<core::TimeComponet::SigMenuDateChanged*>(emit_data.sig_data);
	data::NoteContainer* note_container = static_cast<data::NoteContainer*>(emit_data.receiver);

	const core::Date& date = sig_data->date;

	const data::NoteContainer::SaveAnchor& save_anchor = note_container->get_save_anchor();

	int month_dist = calculate_month_distance(save_anchor, date);

	if (month_dist > data::NoteContainer::MONTH_SEEK_COUNT) {
		data::NoteContainer::SigNeedNotes signal_data = {
			.menu_date = date,
		};

		note_container->set_save_anchor(date);	
		note_container->sig_handler.emit_data(data::NoteContainer::S_ClearOldNotes, &signal_data);		
		note_container->load_new_notes();
	}

}



namespace data {
void Note::display_info() {
	g_print(
		"\nNote Info\nYear: %d\nMonth: %d\nDay: %d\nText: %s\nShould Save: %d\nAddress: %p\n",
		date.year, date.month, date.day, text.c_str(), should_save, this
	);
}


NoteContainerIterator::NoteContainerIterator(NoteContainer* note_container, Note note_arr[], short mark_index, size_t container_size) {
	this->note_container = note_container;
	this->mark_index = mark_index;
	this->container_size = container_size;
	this->note_arr = note_arr;

	
}

bool NoteContainerIterator::operator==(NoteContainerIterator& it) {
	if (mark_index == it.mark_index) {
		return true;
	}

	mark_index %= data::NoteContainer::MAX_NOTE_COUNT;

	return false;
}

Note* NoteContainerIterator::operator->() {
	Note* note = &note_arr[mark_index];
	return note;
}

Note& NoteContainerIterator::operator*() {
	Note& note = note_arr[mark_index];

	return note;
}

NoteContainerIterator& NoteContainerIterator::operator++() {
	mark_index = mark_index+1;

	return *this;
}

NoteContainer::NoteContainer() {
	sig_handler.set_parent_object(this);
	sig_handler.add_signal(S_NOTE_ADDED);
	sig_handler.add_signal(S_NOTE_SAVED);
	sig_handler.add_signal(S_INCREMENTED);
	sig_handler.add_signal(S_NotesReleased);
	sig_handler.add_signal(data::NoteContainer::S_ClearOldNotes);		
}

void NoteContainer::attach_csv_writer(core::csv::Writer* writer) {
	_p_csv_writer = writer;
}

void NoteContainer::connect_time_componet_signals(core::TimeComponet& time_componet) {
	time_componet.sig_handler.add_emit_func(core::TimeComponet::S_MenuDateChanged, signal_time_componet_menu_date_advance, this);

	_save_anchor = {
		.month = time_componet.get_month(),
		.year = time_componet.get_menu_year(),
	};
}

void NoteContainer::display_info() {
	g_print(
		"\nNote container info\nStart Index: %d\nEnd Index: %d\nReal Back Addr: %p\nBack Addr: %p\nContainer addr: %p\n\n", 
		start_index, end_index, &_note_arr[MAX_NOTE_COUNT-1], &back(), this
	);
}

bool NoteContainer::note_exists(core::Date date) {
	for (int i=0; i<=end_index; i++) {
		if (_note_arr[i].date == date) {
			return true;
		}
	}

	return false;
}

void NoteContainer::add_note(Note& note) {
	//work on making add notes work

	_save_anchor.month = note.date.month;
	_save_anchor.year = note.date.year;

	Note* back_note = increment();
	

	if (!back_note) {
		std::cout << "Critical error no back note\n";
		return;
	}

	*back_note = note;

	Entry entry {
		.note = *back_note,
		.empty = false,
	};


	SigNoteAdded sig = {
		.back_note = entry.note,
		.note_container = *this,
		.start_index = start_index,

		.end_index = end_index,
	};

	sig_handler.emit_data(S_NOTE_ADDED, &sig);
}

Note& NoteContainer::after_back() {
	unsigned short index = (end_index+1) % MAX_NOTE_COUNT;

	return _note_arr[index];
}

Note& NoteContainer::back() {
	unsigned short index = (end_index) % MAX_NOTE_COUNT;

	return _note_arr[index];
}

Note* NoteContainer::increment() {
	bool should_increment = end_index < MAX_NOTE_COUNT-1;
	Note* note_ptr = nullptr;

	if (should_increment) { 
		note_ptr = &_note_arr[end_index];
	}
	else {
		note_ptr = get_note_entry();
	}

	//increment start index only if we should still be incrementing
	end_index = (end_index) + (should_increment);
	return note_ptr;
}

Note* NoteContainer::get_note_entry() {
	int i = curr_note_index;
	Note* new_note = nullptr;
	Note* old_note = nullptr;
	
	do {
		old_note = &_note_arr[i];

		int month_dist = calculate_month_distance(_save_anchor, old_note->date);

		if (month_dist > MONTH_SEEK_COUNT) {
			new_note = old_note;
			std::cout << "found an old note to replace "<< "\n";
			old_note->display_info();
			break;
		}

		i = (i + 1) % MAX_NOTE_COUNT;

	} while (i != curr_note_index);

	curr_note_index = i;

	
	return new_note;
}

void NoteContainer::reset(int index) {
	if (index < 0 || index >= MAX_NOTE_COUNT) {
		std::cout << "You are trying to reset index " << index << ", which is out of bounds!\n";
	}

	Note& note = _note_arr[index];

	note.date.year = 0;
	note.date.month = 0;
	note.date.day = 0;
	note.text = "";
}

void NoteContainer::reset(Note& note) {
	note.date.year = 0;
	note.date.month = 0;
	note.date.day = 0;
	note.text = "";
}

void NoteContainer::reset(Note* note) {
	if (note == nullptr) {
		std::cout << "Cannot reset note cause it is nullptr!\n";
		return;
	}

	note->date.year = 0;
	note->date.month = 0;
	note->date.day = 0;
	note->text = "";
}

core::Error NoteContainer::save_notes(short save_count) {
	std::vector<core::csv::Entry> csv_map_vec;

	if (!_p_csv_writer) {
		return core::Error(core::ErrorType::Nullptr);
	}

	int going_to_save_count = 0;

	for (data::Note& note : *this) {
		if (save_count == 0) {
			break;
		}
		going_to_save_count++;

		save_count--;

		if (!note.should_save) {
			//std::cout << "SHOULD NOT SAVE\n";
			//note.display_info();
			//std::cout << "\n\n";
			continue;
		}

		csv_map_vec.push_back(core::csv::Entry());
		core::csv::Entry& entry = csv_map_vec.back();

		entry.add("Day",  std::to_string(note.date.day));	
		entry.add("Month", std::to_string(note.date.month));	
		entry.add("Year", std::to_string(note.date.year));	
		entry.add("Note", note.text);	

	}

	//fix the csv writer as it can only save a single note at a time

	_p_csv_writer->write_csv(csv_map_vec);
	std::cout << "WE ARE ABOUT TO SAVE " << going_to_save_count << " notes!\n";

	return core::Error(core::ErrorType::Clear);
}

core::Error NoteContainer::load_new_notes() {
	if (!_p_csv_writer) {
		std::cout << "No csv writer in note container\n";
		return (core::Error(core::ErrorType::Nullptr));	
	}

	int month_dist = 0;
	Note* new_note = nullptr;

	std::vector<core::csv::Entry> entry_vec;

	_p_csv_writer->read_csv(entry_vec);


	for (core::csv::Entry& entry : entry_vec) {
		Note note;

		try {
			note.date = {
				.day = std::stoi(entry.get("Day")),
				.month = std::stoi(entry.get("Month")),
				.year = std::stoi(entry.get("Year")),
			};
			note.text = entry.get("Note");
		}
		catch (std::exception& e) {
			std::cout << "Something went wrong when trying to load new notes!";
			return core::Error(core::ErrorType::FileDataParse);
		}

		month_dist = calculate_month_distance(_save_anchor, note.date);


		if (month_dist > MONTH_SEEK_COUNT ||  note_exists(note.date)) {
			continue;	
		}

		add_note(note);
	}

	return core::Error(core::ErrorType::Clear);
}

NoteContainerIterator NoteContainer::begin() {
	return NoteContainerIterator(this, _note_arr, start_index, MAX_NOTE_COUNT);
}

NoteContainerIterator NoteContainer::end() {
	return NoteContainerIterator(this, _note_arr, end_index+1, MAX_NOTE_COUNT);
}

short NoteContainer::get_start_index() {
	return start_index;
}

short NoteContainer::get_end_index() {
	return start_index;
}

void NoteContainer::set_save_anchor(const core::Date& date) {
	_save_anchor = {
		.month = date.month,
		.year = date.year,
	};
}
const NoteContainer::SaveAnchor& NoteContainer::get_save_anchor() const {
	return _save_anchor;
}
}
