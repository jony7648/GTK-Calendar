#include "note_container.h"
#include "core/util.h"

namespace data {
NoteContainerIterator::NoteContainerIterator(NoteContainer* note_container, Note note_arr[], short start_index, short end_index, size_t container_size) {
	this->note_container = note_container;
	this->start_index = start_index;
	this->end_index = end_index;
	this->container_size = container_size;
	this->note_arr = note_arr;

	}

bool NoteContainerIterator::operator==(NoteContainerIterator it) {
	if (current_index == end_index) {
		return true;
	}

	return false;
}

Note* NoteContainerIterator::operator->() {
	Note* note = &note_arr[current_index];

	return note;
}

Note& NoteContainerIterator::operator*() {
	Note& note = note_arr[current_index];

	return note;
}

NoteContainerIterator& NoteContainerIterator::operator++() {
	current_index = (current_index+1) % MAX_NOTE_COUNT;
	
	return *this;
}


NoteContainer::NoteContainer() {
	sig_handler.set_parent_object(this);
	sig_handler.add_signal(S_NOTE_ADDED);
	sig_handler.add_signal(S_NOTE_SAVED);
	sig_handler.add_signal(S_INCREMENTED);

	_sig_data = util::dynamic_data_init(sizeof(SigNoteAdded));
}

NoteContainer::~NoteContainer() {
	util::dynamic_data_free(_sig_data);
}

Note& NoteContainer::add_note() {
	if (start_index == MAX_NOTE_COUNT) {
		start_index = 0;
	}

	if (end_index == MAX_NOTE_COUNT) {
		end_index = 0;
	}

	reset(end_index);


	end_index++;
	return note_arr[end_index-1];
}

Note& NoteContainer::after_end() {
	unsigned short index = (end_index + 1) % MAX_NOTE_COUNT;

	return note_arr[index];
	
}

void NoteContainer::increment() {
	end_index = (end_index + 1) % MAX_NOTE_COUNT;
	
	_sig_data = util::dynamic_data_assign(_sig_data, sizeof(SigIncremented));
	SigIncremented* signal = (SigIncremented*)_sig_data;

	signal->end_note = &note_arr[end_index];
	
	sig_handler.emit_data(S_INCREMENTED, signal);
}

void NoteContainer::reset(int index) {
	if (index < 0 || index >= MAX_NOTE_COUNT) {
		std::cout << "You are trying to reset index " << index << ", which is out of bounds!\n";
	}

	Note& note = note_arr[index];

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
	return NoteContainerIterator(this, note_arr, start_index, end_index, MAX_NOTE_COUNT);
}

NoteContainerIterator NoteContainer::end() {
	return NoteContainerIterator(this, note_arr, start_index, end_index, MAX_NOTE_COUNT);
}

short NoteContainer::get_start_index() {
	return start_index;
}

short NoteContainer::get_end_index() {
	return start_index;
}


}
