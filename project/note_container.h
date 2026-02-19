#pragma once
#include <iostream>

const short int MAX_NOTE_COUNT = 50;

namespace data {
class NoteContainer;

struct Note {
	unsigned int year = 0, month = 0, day = 0;
	std::string text = "";	
};

class NoteContainerIterator {
private:
	NoteContainer* note_container;
	int start_index = 0, end_index = 0, current_index = 0;
	size_t container_size = 0;
	Note* note_arr;
public:
	using Pointer = Note*;
	using refrence = Note&;

	NoteContainerIterator(NoteContainer* note_container, Note note_arr[], short start_index, short end_index, size_t container_size);

	bool operator==(NoteContainerIterator it);

	Note& operator*();
	Note* operator->();

	NoteContainerIterator& operator++();


	//Note* operator=(Note* widget_ptr);


};

class NoteContainer {
private:
	using Iterator = NoteContainerIterator;
	Note note_arr[MAX_NOTE_COUNT] = {};
	short start_index = 0, end_index = 0;
public:
	Note& add_note();
	void reset(int index);
	Iterator begin();
	Iterator end();
	short get_start_index();
	short get_end_index();
};
}
