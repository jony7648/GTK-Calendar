#pragma once
#include <iostream>
#include <gtk/gtk.h>
#include "space.h"
#include "scene.h"
#include "error_code.h"
#include "app.h"


namespace core {
class App;

class Window {
	
public:
	enum SIGNALS {
		S_REQUEST,
		S_END_PROGRAM,
		S_WINDOW_CLOSE
	};

	GtkWidget* get_gtk_window();
	Window(GtkApplication* gtk_app_ptr, const std::string& title);
	~Window();
	Error display(Scene* scene);
	void signal_set_close(App*, bool(*func)(GtkWidget* widget, gpointer user_data));

	void set_scene(Scene* scene);
	core::Scene* get_scene();
	
	void close();
	void show();
	void hide();


	
	void set_attached_state();
	bool get_attached_state();

	void set_dimensions(const space::Point& dimensions);

	void set_as_main_window();
	bool get_is_main_window();

	bool get_display_state();
	bool get_visibility();

	void emit_signal(int id, void* parent = nullptr);
	void add_emit_func(int id, void(*emit_func)(void*, void*, void*), void* receiver_obj);

	SigHandler sig_handler;



private:
	std::string title = "";
	space::Point dimensions;
	GtkWidget* gtk_window = nullptr;
	Scene* current_scene = nullptr;
	void(*_signal_close)(GtkWidget*, gpointer user_data);
	bool is_main_window = false;
	bool is_attached = false;
	bool is_displaying = false;
	bool is_visible = false;
	

};
}
