#include "note_container.h"
#include "core/util.h"

#include <glib.h>

#define SAVE_ADJUST_COUNT 2

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

	mark_index %= MAX_NOTE_COUNT;

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
}

void NoteContainer::display_info() {
	g_print(
		"\nNote container info\nStart Index: %d\nEnd Index: %d\nReal Back Addr: %p\nBack Addr: %p\nContainer addr: %p\n\n", 
		start_index, end_index, &_note_arr[MAX_NOTE_COUNT-1], &back(), this
	);
}

void NoteContainer::add_note(const Note& note) {
	//work on making add notes work

	increment();
	Note& back_note = back();


	Entry entry {
		.note = back_note,
		.empty = true,
	};

	back_note = note;


	SigNoteAdded sig = {
		.back_note = back_note,
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

void NoteContainer::increment() {
	int shifted_end = (end_index+1) % MAX_NOTE_COUNT;

	//adjust start bound if end_index and start bound is the same

	if (start_index == shifted_end) {
		SigNotesReleased sig {
			.save_count = SAVE_ADJUST_COUNT
		};

		std::cout << "This was the start index" << start_index << "\n";
		sig_handler.emit_data(S_NotesReleased, &sig);
		start_index = (start_index + SAVE_ADJUST_COUNT) % MAX_NOTE_COUNT;
	}

	end_index = shifted_end;
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
}
