#pragma once
#include <map>
#include <functional>
#include <gtk/gtk.h>

namespace core {class SigHandler;};
namespace gtkc {class Widget;};

namespace core {
struct GMessage {
	core::SigHandler* sig_handler = nullptr;
	gtkc::Widget* emitter_obj = nullptr;
	void* parent_obj = nullptr;
	void(*emit_func)(void*,void*);
};


struct FuncParentPair {
	FuncParentPair(void(*emit_func)(void*, void*, void*), void* func_holder) {
		this->emit_func = emit_func;
		this->func_holder = func_holder;
	}

	FuncParentPair() = default;

	void(*emit_func)(void*, void*, void*);
	void* func_holder;
};


class SigHandler {
public:
	using SigMap = std::unordered_map<int, std::vector<FuncParentPair>>;
	~SigHandler();
	void display_info();
	void add_signal(int id);
	void add_emit_func(int id, void(*emit_func)(void*, void*, void*), void* receiver_obj);
	void emit(int id, void* emitter_obj = nullptr);
	void emit_data(int id, void* sig_data, void* emitter_obj = nullptr);
	void g_listen(gtkc::Widget* sig_holder_obj, const std::string& emit_type, void(*emit_func)(void*, void*)); 
	void set_parent_object(void* parent);

private:
	void* _parent = nullptr;
	SigMap _sig_map;
	GtkWidget* _gtk_parent = nullptr;
	std::vector<GMessage*> _message_vec;
}; 
}
