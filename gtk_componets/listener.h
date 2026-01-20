#pragma once

#include <iostream>
#include <gtk/gtk.h>
#include <unordered_map>
#include <vector>

void process_signal(GtkWidget* gtk_widget, gpointer user_data);
namespace gtkc{struct Message;};

using SignalMap = std::unordered_map<void*, std::unordered_map<std::string, void (*)(gtkc::Message*)>>;


namespace gtkc {
struct Message {
	void* widget;
	std::string emit_type;
	SignalMap* event_map;
};

template <typename T>
struct Listener {


T widget;
std::string emit_type;
GtkWidget* parent_gtk_widget;
SignalMap event_map;
std::vector<gtkc::Message*> message_list;

void connect(Listener<T>, void(*signal_func)(Message*), const std::string& emit_type) {
	Message* message = new Message;

	message->widget = nullptr;
	message->emit_type = emit_type;
	message->event_map = &this->event_map;
	message_list.push_back(message);
	g_signal_connect(parent_gtk_widget, emit_type.c_str(), G_CALLBACK(&process_signal), message);
}




void set_gtk_parent(GtkWidget* gtk_widget) {
	this->parent_gtk_widget = gtk_widget;
}

};


/*
class Listener {
using SignalMap = std::unordered_map<Listener*, std::unordered_map<std::string, void (*)(Message*)>>;

private:
	GtkWidget* parent_gtk_widget = nullptr;
	Listener* recipiant;
	SignalMap event_map;
	
public:
	//Listener(GtkWidget* parent_gtk_widget);
	//void connect(std::string emit_type, void(*signal_func)(std::string, gpointer));
	//void process_signal(GtkWidget* gtk_widget, gpointer user_data);
	

Listener(GtkWidget* parent_gtk_widget) {
	this->parent_gtk_widget = parent_gtk_widget;		
}


void set_recipiant(Listener* recipiant) {
	this->recipiant = recipiant;
}

void connect(void (*process_func)(), std::string emit_type) {
	void* messenger_data = nullptr;

	g_signal_connect(parent_gtk_widget, emit_type.c_str(), G_CALLBACK(process_func), messenger_data);
}

void process_signal(GtkWidget* gtk_widget, gpointer user_data) {
	Message* message = (Message*)user_data;
	Widget* widget = message->widget;
	const std::string& widget_emit_type = message->emit_type;

	for (auto& [listener, message_data_map] : event_map) {
		for (auto& [emit_type, signal_func] : message_data_map) {
			if (emit_type == widget_emit_type) {
				signal_func(message);
			}
		}
	}

}
};

struct ListenerMessage {
	Listener* listener;
};
*/

}



