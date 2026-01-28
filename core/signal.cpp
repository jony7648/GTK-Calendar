#include "signal.h"
#include "signaler.h"


static void process_signal(GtkWidget* gtk_widget, gpointer user_data) {
	//
	core::SigMessageLink* sig_link = static_cast<core::SigMessageLink*>(user_data);

	for (auto& signal : sig_link->signal_vec) {
		signal->emit_signal(sig_link->emitter_obj);	
	}
}

namespace core {

void Signal::listen(Signaler* signaler, const std::string& emit_type, void(*emit_func)(void*, void*)) {
	//this function will "listen" for GTK_signals and execute a function 
	//upon the receiving said GTK signal
	Message* message = new core::Message;
	SigMessageLink* sig_link = nullptr;
	void* parent_widget = signaler->get_parent_widget();

	message->widget = parent_widget;
	message->receiver_object = this->parent_widget;

	sig_link = signaler->connect_to_signal(this, emit_type, message);

	this->_emit_func = emit_func;
	g_signal_connect(signaler->get_parent_gtk(), emit_type.c_str(), G_CALLBACK(&process_signal), sig_link);
}



void Signal::emit_signal(void* emitter_obj) {
	//This function will execute the funciton set in the signal
	if (f_signal) {
		f_signal->emit_signal(this->parent_widget);
		return;
	}

	if (!_emit_func) {
		std::cout << "ERROR: emit_func is nullptr\n";
		return;
	}
	
	_emit_func(this->parent_widget, emitter_obj);
}

void Signal::pickup_signal(Signal* signal) {
	//This function can be used to pickup a singal and get it's information
	//even if the parent of this object doesn't hold the parent of the emitter
	signal->f_signal = this;
}

void Signal::set_parent_widget(void* parent_widget) {
	this->parent_widget = parent_widget;
}

void Signal::set_emit_func(void(*emit_func)(void*, void*)) {
	this->_emit_func = emit_func;
}

void Signal::disconnect(Signaler* signaler) {
	//Place Holder
}
}
