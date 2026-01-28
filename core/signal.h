#pragma once

#include <functional>
#include <gtk/gtk.h>
#include "messenger.h"
#include "signaler.h"

namespace core {
struct Signaler;

class Signal {
private:
	void* parent_widget;
	GtkWidget* parent_gtk_widget;
	std::vector<Message*> message_vec;
	std::function<void(void*, void*)> _emit_func = nullptr;
public:
	Signal* f_signal = nullptr;
	void listen(Signaler* signaler, const std::string& emit_type, void(*emit_func)(void*, void*));
	void pickup_signal(Signal* signal);
	void set_parent_widget(void* parent_widget);
	void set_emit_func(void(*emit_func)(void*, void*));
	void emit_signal(void* emitter_obj);
	void disconnect(Signaler* signaler);
};
}
