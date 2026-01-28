#include "signaler.h"
#include "signal.h"

namespace core {
Signaler::~Signaler() {
	for (auto& [emit_type, sig_link] : signal_map) {
		for(auto& signal : sig_link.signal_vec) {
			signal->disconnect(this);
		}

		delete sig_link.message;
	}
}

SigMessageLink* Signaler::connect_to_signal(Signal* signal, const std::string& emit_type, Message* message) {
	SigMessageLink* sig_link = &signal_map[emit_type];

	sig_link->emitter_obj = parent_widget;
	sig_link->emit_type = emit_type;
	sig_link->message = message;
	sig_link->signal_vec.push_back(signal);

	return sig_link;
}

void Signaler::set_parent_gtk(GtkWidget* gtk_widget) {
	parent_gtk_widget = gtk_widget;
}

void Signaler::set_parent_widget(void* widget) {
	parent_widget = widget;
}

void* Signaler::get_parent_widget() {
	return parent_widget;	
}

GtkWidget* Signaler::get_parent_gtk() {
	return parent_gtk_widget;	
}
}
