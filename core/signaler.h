#pragma once

#include <gtk/gtk.h>
#include "messenger.h"

namespace core {
struct Signal;

class Signaler {
private:
	void* parent_widget;
	GtkWidget* parent_gtk_widget;
	SignalMap signal_map;
public:
	~Signaler();
	SigMessageLink* connect_to_signal(Signal* signal, const std::string& emit_type, Message* message);
	void set_parent_widget(void* widget);
	void set_parent_gtk(GtkWidget* gtk_widget);
	GtkWidget* get_parent_gtk();
	void* get_parent_widget();
};
}
