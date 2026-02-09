#pragma once
#include <gtk/gtk.h>
#include <iostream>
#include "util.h"
#include "messenger.h"
#include "space.h"
#include "window.h"
#include "time_componet.h"
#include "scene.h"
#include "error_code.h"


namespace core {
class Window;

class App {
private:
	Scene* main_scene = nullptr;
	std::vector<Scene*> sub_scene_vect;
	GtkApplication* gtk_app = nullptr;
	core::TimeComponet time_componet;
	std::string title;
	space::Point win_dimensions;

	Window* main_window = nullptr;
	std::vector<Window*> subwin_vect;

	Signaler signaler;

	GtkCssProvider* _css_provider = nullptr;
	GdkDisplay* _default_display = nullptr;

	int argc = 0;
	char** argv = nullptr;
	int subwin_cap = 5;
	int app_status = 0;

public:
	App(const std::string& title, const space::Point& dimensions,  int argc, char* argv[]);
	~App();
	//bool process_close_request(GtkWidget* gtk_widget, gpointer user_data);
	void run(void(*activate_func)(GtkApplication*, gpointer), core::Messenger<App*>* signal_ptr);
	void close();
	Error attach_main_window(Window* window);
	int attach_subwin(Window* window);
	Error attach_main_scene(Scene* scene);
	Error attach_sub_scene(Scene* scene);
	void display_main_window();
	void apply_provider(const std::string& css_dir_path);
	bool request_subwin(const std::string& scene_name);



	Signal S_scene_request_subwin;
	Signal S_window_end_program;

	Scene* get_main_scene();
	void set_subwin_cap(int cap);
	GtkWidget* get_scene_container();
	core::TimeComponet* get_time_componet();
	const space::Point& get_win_dimensions();

};
}
