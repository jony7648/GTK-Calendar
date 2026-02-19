#pragma once

#include <functional>
#include <gtk/gtk.h>
#include "messenger.h"
#include "signaler.h"

namespace core {
struct Signaler;

class Signal {
protected:
	void* parent_widget;
	GtkWidget* parent_gtk_widget;
	std::function<void(void*, void*)> _emit_func = nullptr;
	std::function<void(void*, void*, void*)> _param_emit_func = nullptr;
public:
	~Signal();
	Signal* f_signal = nullptr;
	void listen(Signaler* signaler, const std::string& emit_type, void(*emit_func)(void*, void*));
	void listen(Signaler* signaler, const std::string& emit_type, void(*emit_func)(void*, void*, void*), void* signal_data);
	void pickup_signal(Signal* signal);
	void set_parent_widget(void* parent_widget);
	void set_emit_func(void(*emit_func)(void*, void*));
	void set_emit_func(void(*emit_func)(void*, void*, void*));
	void emit_signal(void* emitter_obj = nullptr);
	void emit_signal(void* emitter_obj, void* sig_data);
	void disconnect(Signaler* signaler);
};
}
