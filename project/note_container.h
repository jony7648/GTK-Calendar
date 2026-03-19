#pragma once
#include <iostream>
#include "core/time_componet.h"
#include "core/signal_handler.h"

const short int MAX_NOTE_COUNT = 4;
//const short int MAX_SAVE_COUNT = 20;

namespace data {
class NoteContainer;

struct Note {
	void display_info();

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
		Note* note = nullptr;
	};

	struct SigNoteSaved {
		Note* note = nullptr;	
	};

	struct SigIncremented {
		Note* end_note = nullptr;	
		int start_index = 0, end_index = 0;

	};

	NoteContainer();
	~NoteContainer();
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
	Note note_arr[MAX_NOTE_COUNT] = {};
	short start_index = 0, end_index = 0;
	short save_count = 0;
	void* _sig_data = nullptr;
};
}
