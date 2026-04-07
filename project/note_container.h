#pragma once
#include <iostream>
#include "core/time_componet.h"
#include "core/signal_handler.h"
#include "core/error.h"


//const short int MAX_NOTE_COUNT = 10;
//const short int MAX_SAVE_COUNT = 20;

namespace data {
class NoteContainer;



struct Note {
	void display_info();
	bool operator==(Note& note);

	core::Date date;
	std::string text = "";	
	bool should_save = false;
	bool new_note = false;
};

class NoteContainerIterator {
public:
	using Pointer = Note*;
	using refrence = Note&;

	NoteContainerIterator(NoteContainer* note_container, Note note_arr[], short mark_index, size_t container_size);

	bool operator==(NoteContainerIterator& it);

	Note& operator*();
	Note* operator->();

	NoteContainerIterator& operator++();


	//Note* operator=(Note* widget_ptr);


private:
	NoteContainer* note_container;
	Note* note_arr;
	int mark_index = 0, current_index = 0;
	size_t container_size = 0;
};

class NoteContainer {
	using Iterator = NoteContainerIterator;

public:
	const static short MONTH_SEEK_COUNT = 2;
	const static short DAYS_PER_MONTH = 31;
	//const static short int MAX_NOTE_COUNT = MONTH_SEEK_COUNT*DAYS_PER_MONTH;
	const static short MAX_NOTE_COUNT = 4;

	enum SIGNALS {
		S_NOTE_ADDED,
		S_NOTE_SAVED,
		S_INCREMENTED,
		S_NotesReleased,
		S_ClearOldNotes,
	};

	struct Entry {
		Note note;
		bool empty = true;
	};

	struct SaveAnchor {
		int month = 0, year = 0;
	};

	struct SigNoteAdded {
		Note& back_note;
		NoteContainer& note_container;
		Entry* entry_arr;
		int start_index = 0, end_index = 0;
	};

	struct SigNoteSaved {
		Note* note = nullptr;	
	};

	struct SigIncremented {
		Note* end_note = nullptr;	
		int start_index = 0, end_index = 0;
	};

	struct SigNeedNotes {
		const core::Date& menu_date;
	};

	struct SigNotesReleased {
		unsigned int save_count = 0;
	};

	NoteContainer();
	void attach_csv_writer(core::csv::Writer* writer);
	void connect_time_componet_signals(core::TimeComponet& time_componet);
	void display_info();


	bool note_exists(core::Date date);

	void add_note(Note& note);
	Note& after_back();
	void reset(int index);
	void reset(Note& note);
	void reset(Note* note);
	Iterator begin();
	Iterator end();
	short get_start_index();
	short get_end_index();
	Note& back();

	core::Error load_new_notes();

	void set_save_anchor(const core::Date& date);
	const SaveAnchor& get_save_anchor() const;


	core::SigHandler<NoteContainer> sig_handler;

private:
	Note _note_arr[MAX_NOTE_COUNT] = {};

	SaveAnchor _save_anchor;
	short start_index = 0, end_index = 0;
	short curr_note_index = 0;
	short save_count = 0;

	core::csv::Writer* _p_csv_writer = nullptr;

	Note* increment();
	Note* get_note_entry();
};
}
