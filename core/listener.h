#pragma once

#include <iostream>
#include <gtk/gtk.h>
#include <unordered_map>
#include <vector>

void process_signal(GtkWidget* gtk_widget, gpointer user_data);
namespace core{struct Message;};

using SignalMap = std::unordered_map<void*, std::unordered_map<std::string, void (*)(core::Message*)>>;


namespace core {
struct Message {
	void* receiver_object;
	void* widget;
	std::string emit_type;
	SignalMap* event_map;
};

class Listener {
private:
	std::string emit_type;
	GtkWidget* parent_gtk_widget;
	void* parent_widget;
	SignalMap event_map;
	std::vector<Message*> message_list;

public:
	void* get_parent_widget();
	void connect(Listener* listener, void(*signal_func)(Message*), const std::string& emit_type);
	void set_parent_widget(void* parent_widget);
	void set_gtk_parent(GtkWidget* gtk_widget);
};
}



