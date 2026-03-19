#include <iostream>
#include "signal_handler.h"
#include "widget.h"





void core::process_signal(GtkWidget*, gpointer user_data) {
	core::GMessage* g_message = static_cast<core::GMessage*>(user_data);
	g_message->emit_func(g_message->emit_data);
}

