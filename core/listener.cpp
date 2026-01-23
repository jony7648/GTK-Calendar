#include "listener.h"

void process_signal(GtkWidget* gtk_widget, gpointer user_data) {
	core::Message* message = (core::Message*)user_data;
	const std::string& widget_emit_type = message->emit_type;
	SignalMap* p_event_map = message->event_map;

	//std::cout << "Passed Test\n";

	for (auto& [listener, message_data_map] : *p_event_map) {
		//std::cout << "Listener: " << listener << "\n";
		for (auto& [emit_type, signal_func] : message_data_map) {
			//std::cout << "Message: " << message << "\n";
			if (emit_type == widget_emit_type) {
				signal_func(message);
			}
		}
	}

}


namespace core {
void Listener::connect(Listener* listener, void(*signal_func)(Message*), const std::string& emit_type) {
	Message* message = new Message;

	event_map[(void*)listener][emit_type] = signal_func;

	message->receiver_object = listener->get_parent_widget();
	//std::cout << message->receiver_object << "\n";
	message->widget = parent_widget;
	message->emit_type = emit_type;
	message->event_map = &this->event_map;

	//message_list.push_back(message);
	//std::cout << parent_gtk_widget << "\n";
	g_signal_connect(parent_gtk_widget, emit_type.c_str(), G_CALLBACK(&process_signal), message);
}

void Listener::set_parent_widget(void* parent_widget) {
	this->parent_widget = parent_widget;
}

void Listener::set_gtk_parent(GtkWidget* gtk_parent) {
	this->parent_gtk_widget = gtk_parent;
	//std::cout << gtk_parent << "\n";
}

void* Listener::get_parent_widget() {
	if (parent_widget == nullptr) {
		//std::cout << "ERROR: Parent widget is a nullptr\n";
	}

	//std::cout << parent_widget << "\n";

	return parent_widget;
}


}
