#pragma once
#include <iostream>
#include "core/time_componet.h"
#include "core/signal_handler.h"

const short int MAX_NOTE_COUNT = 50;
const short int MAX_SAVE_COUNT = 20;

namespace data {
class NoteContainer;

struct Note {
	core::Date date;
	std::string text = "";	
	bool should_save = false;
};



class NoteContainerIterator {
public:
	

	using Pointer = Note*;
	using refrence = Note&;

	NoteContainerIterator(NoteContainer* note_container, Note note_arr[], short start_index, short end_index, size_t container_size);

	bool operator==(NoteContainerIterator it);

	Note& operator*();
	Note* operator->();

	NoteContainerIterator& operator++();


	//Note* operator=(Note* widget_ptr);


private:
	NoteContainer* note_container;
	Note* note_arr;
	int start_index = 0, end_index = 0, current_index = 0;
	size_t container_size = 0;
	

};

class NoteContainer {
	using Iterator = NoteContainerIterator;

public:

	enum SIGNALS {
		S_NOTE_ADDED,
		S_NOTE_SAVED,
		S_INCREMENTED,
	};

	struct SigNoteAdded {
		Note* note;

	};

	struct SigNoteSaved {
		Note* note;	

	};

	struct SigIncremented {
		Note* end_note;	

	};

	NoteContainer();
	~NoteContainer();
	core::SigHandler sig_handler;
	Note& add_note();
	Note& after_end();
	void increment();
	void reset(int index);
	void reset(Note& note);
	void reset(Note* note);
	Iterator begin();
	Iterator end();
	short get_start_index();
	short get_end_index();


private:
	Note note_arr[MAX_NOTE_COUNT] = {};
	short start_index = 0, end_index = 0;
	short save_count = 0;
	void* _sig_data = nullptr;
};
}
