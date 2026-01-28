#pragma once
#include <gtk/gtk.h>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace core{struct Signal; struct SigMessageLink;};
namespace gtkc{class Widget;};


using SignalMap = std::unordered_map<std::string, core::SigMessageLink>;

namespace core {
template <typename T>
struct Messenger {
	T object;
};

template <typename T1, typename T2>
struct DoubleMessenger {
	T1 object1;
	T2 object2;
};

struct Message {
	void* receiver_object;
	void* widget;
		
	std::string emit_type;
	SignalMap* event_map;
};

struct SigMessageLink {
	std::string emit_type = "";
	void* emitter_obj = nullptr;
	Message* message = nullptr;
	std::vector<Signal*> signal_vec;
};

template <typename T1, typename T2>
struct DataMessenger {
	T1 object = nullptr;
	T2 data = nullptr;
};


}
