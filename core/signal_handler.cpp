#include <iostream>
#include "signal_handler.h"
#include "widget.h"



void process_signal(GtkWidget*, gpointer user_data) {
	core::GMessage* message = static_cast<core::GMessage*>(user_data);
	message->emit_func(message->parent_obj, message->emitter_obj);
}

bool check_for_id(core::SigHandler::SigMap& sig_map, int id) {
	for (auto& [key, value] : sig_map) {
		if (key == id) {
			return true;
		}
	}

	return false;
}

namespace core {
SigHandler::~SigHandler() {
	for (GMessage* message : _message_vec) {
		delete message;
	}
}

void SigHandler::display_info() {
	std::cout << "Sig Data\n";
	for (auto& [key, value] : _sig_map) {
		std::cout << "Key: " << key << " Count: " << value.size() << "\n";
	}
}

void SigHandler::add_signal(int id) {
	_sig_map[id];
}



void SigHandler::g_listen(gtkc::Widget* emitter_obj, const std::string& emit_type, void(*emit_func)(void*,void*)) {
	GMessage* message = new GMessage;

	message->emitter_obj = emitter_obj;
	message->parent_obj = _parent;
	message->sig_handler = this;
	message->emit_func = emit_func;

	_message_vec.push_back(message);
	
	g_signal_connect(emitter_obj->get_gtk_widget(), emit_type.c_str(), G_CALLBACK(&process_signal), message);
}

void SigHandler::set_parent_object(void* parent) {
	_parent = parent;
}

void SigHandler::emit(int id, void* emitter_obj) {
	bool found_id = false;

	found_id = check_for_id(_sig_map, id);

	if (!found_id) {
		std::cout << "ERROR: signal id " << id << " could not be found\n";
		return;
	}

	std::vector<FuncParentPair>& pair_vec = _sig_map[id];

	if (!emitter_obj) {
		emitter_obj = _parent;
	}

	for (FuncParentPair& pair : pair_vec) {
		if (pair.emit_func) {
			pair.emit_func(pair.func_holder, emitter_obj, nullptr);
		}
	}
}

void SigHandler::emit_data(int id, void* sig_data, void* emitter_obj) {
	bool found_id = false;

	found_id = check_for_id(_sig_map, id);

	if (!found_id) {
		std::cout << "ERROR: signal id " << id << " could not be found\n";
		return;
	}


	std::vector<FuncParentPair>& pair_vec = _sig_map[id];

	if (!emitter_obj) {
		emitter_obj = _parent;
	}

	for (FuncParentPair& pair : pair_vec) {
		if (pair.emit_func) {
			pair.emit_func(pair.func_holder, emitter_obj, sig_data);
		}
	}
}


void SigHandler::add_emit_func(int id, void (*emit_func)(void*, void*, void*), void* receiver_obj) {
	
	// the function pointer passed into this function will be executed
	// when the signal id is executed
	
	//std::cout << "ID being passed " << id << "\n";
	//std::cout << "Address being passed " << emit_func << "\n";
	//std::cout << "Parent " << receiver_obj << "\n";

	if (!check_for_id(_sig_map, id)) {
		std::cout << "Error: signal with id " << id << " could not be executed\n";
		return;
	}

	if (emit_func == nullptr) {
		std::cout << "Tried to add an emit func with non existent id " << id << "\n";
	}

	std::vector<FuncParentPair>& pair_vec = _sig_map[id];
	pair_vec.push_back(FuncParentPair(emit_func, receiver_obj));
}
}
