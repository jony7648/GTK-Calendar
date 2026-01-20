#include "listener.h"

void process_signal(GtkWidget* gtk_widget, gpointer user_data) {
	gtkc::Message* message = (gtkc::Message*)user_data;
	const std::string& widget_emit_type = message->emit_type;
	SignalMap* p_event_map = message->event_map;

	std::cout << "Passed Test\n";

	for (auto& [listener, message_data_map] : *p_event_map) {
		for (auto& [emit_type, signal_func] : message_data_map) {
			if (emit_type == widget_emit_type) {
				signal_func(message);
			}
		}
	}

}
