#pragma once
#include <iostream>
#include "core/time_componet.h"
#include "core/signal_handler.h"

const short int MAX_NOTE_COUNT = 4;
//const short int MAX_NOTE_COUNT = 10;
//const short int MAX_SAVE_COUNT = 20;

namespace data {
class NoteContainer;

struct save_bounds {
	short left_month, right_month;
};

struct Note {
	void display_info();

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
	enum SIGNALS {
		S_NOTE_ADDED,
		S_NOTE_SAVED,
		S_INCREMENTED,
		S_NotesReleased
	};

	struct Entry {
		Note note;
		bool empty = true;
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

	struct SigNotesReleased {
		unsigned int save_count = 0;
	};

	

	NoteContainer();
	void display_info();

	core::SigHandler<NoteContainer> sig_handler;
	void add_note(const Note& note);
	Note& after_back();
	void increment();
	void reset(int index);
	void reset(Note& note);
	void reset(Note* note);
	Iterator begin();
	Iterator end();
	short get_start_index();
	short get_end_index();
	Note& back();


private:
	Note _note_arr[MAX_NOTE_COUNT] = {};
	short start_index = 0, end_index = 0;
	short save_count = 0;
};
}
