#pragma once
#include <map>
#include <functional>
#include <gtk/gtk.h>
#include <iostream>
#include "widget.h"

//namespace gtkc {class Widget;};


namespace core {

//bool check_for_id(SignalHandlerSigMap& sig_map, int id);
void process_signal(GtkWidget*, gpointer user_data);

struct ProcessSignalData {
	void* message_data;
	void* sig_data;
	void* parent_obj = nullptr;
	void(*emit_func)(ProcessSignalData*);
};



template <typename SigMap>
bool check_for_id(SigMap& sig_map, int id) {
	for (auto& [key, value] : sig_map) {
		if (key == id) {
			return true;
		}
	}
	return false;
}

template <typename ObjType>
struct EmitData {
	ObjType* holder;
	void* receiver;
	void* sig_data;
	void* emitter;
};

struct G_EmitData {
	void* receiver;
	void* emitter;
	void* sig_data;
};

struct GMessage {
	G_EmitData emit_data;
	void(*emit_func)(G_EmitData&) = nullptr;
};



template <typename ObjType>
class SigHandler {


private:
	using EmitDataType = EmitData<ObjType>;


struct FuncParentPair {
	FuncParentPair(std::function<void(EmitDataType&)> emit_func, void* func_holder) {
		this->emit_func = emit_func;
		this->func_holder = func_holder;
	}

	FuncParentPair() = default;

	//void(*emit_func)(EmitDataType);

	std::function<void(EmitDataType&)> emit_func;
	void* func_holder;
};


	using SigMap = std::unordered_map<int, std::vector<FuncParentPair>>;

	ObjType* _parent = nullptr;
	SigMap _sig_map;
	GtkWidget* _gtk_parent = nullptr;
	std::vector<GMessage*> _message_vec;
	EmitDataType _emit_data;

public:
SigHandler() = default;
SigHandler(ObjType parent) {
	_parent = parent;
}

~SigHandler() {
	for (GMessage* message : _message_vec) {
		delete message;
	}
}

void display_info() {
	std::cout << "Sig Data\n";
	for (auto& [key, value] : _sig_map) {
		std::cout << "Key: " << key << " Count: " << value.size() << "\n";
	}
}

void add_signal(int id) {
	_sig_map[id];
}


void g_listen(gtkc::Widget* emitter_obj, const std::string& emit_type, void(*emit_func)(G_EmitData&)) {
	GMessage* message = new GMessage;

	message->emit_data.emitter = emitter_obj;
		

	message->emit_data.receiver = _parent;
	message->emit_func = emit_func;

	_message_vec.push_back(message);
	
	g_signal_connect(emitter_obj->get_gtk_widget(), emit_type.c_str(), G_CALLBACK(&process_signal), message);
}

void set_parent_object(ObjType* parent) {
	_parent = parent;
}

void emit(int id, void* emitter_obj = nullptr) {
	bool found_id = false;

	found_id = check_for_id(_sig_map, id);

	if (!found_id) {
		std::cout << "ERROR: signal id " << id << " could not be found\n";
		return;
	}

	std::vector<FuncParentPair>& pair_vec = _sig_map[id];

	for (FuncParentPair& pair : pair_vec) {
		if (pair.emit_func) {
			_emit_data.receiver = pair.func_holder;
			_emit_data.holder = _parent;
			_emit_data.sig_data = nullptr;
			_emit_data.emitter = emitter_obj;

			pair.emit_func(_emit_data);
		}
	}
}

void emit_data(int id, void* sig_data, void* emitter_obj = nullptr) {
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
			_emit_data.receiver = pair.func_holder;
			_emit_data.holder = _parent;
			_emit_data.sig_data = sig_data;
			_emit_data.emitter = emitter_obj;
			pair.emit_func(_emit_data);
		}
	}
}


void add_emit_func(int id, std::function<void(EmitDataType&)> emit_func, void* receiver_obj = nullptr) {
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
};
}
