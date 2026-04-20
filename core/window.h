#pragma once
#include <iostream>
#include <gtk/gtk.h>
#include "space.h"
#include "scene.h"
#include "error.h"
#include "app.h"
#include "css_provider.h"


namespace core {
class App;

class Window {
public:
	enum class Type {
		Main,
		Sub,
		SubFocus,
	};

	enum SIGNALS {
		S_REQUEST,
		S_END_PROGRAM,
		S_WINDOW_CLOSE
	};

	struct SignalWindowClose {
		Scene* subscene = nullptr;
	};

	


	GtkWidget* get_gtk_window();
	Window(GtkApplication* gtk_app_ptr, const std::string& title);
	~Window();
	Error display(Scene& scene);

	void set_scene(Scene* scene);
	core::Scene* get_scene();
	
	void close();
	void show();
	void hide();

	void set_sensitivity(bool state);
	
	void set_attached_state();
	bool get_attached_state();

	void set_dimensions(const space::Point& dimensions);

	void set_type(Type type);
	bool is_type(Type type);
	bool is_main();
	Type get_type();
		

	bool get_display_state();
	bool get_visibility();

	SigHandler<Window> sig_handler;



private:
	std::string _title = "";
	space::Point _dimensions;
	Scene* _p_current_scene = nullptr;
	void(*_signal_close)(GtkWidget*, gpointer user_data);
	bool _is_attached = false;
	bool _is_displaying = false;
	bool _is_visible = false;
	Type _type = Type::Sub;

	GtkWidget* _gtk_window = nullptr;
	CSSProvider _css_provider;
};
}
