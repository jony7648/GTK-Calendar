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
	Window* main_window = nullptr;
	Scene* main_scene = nullptr;
	GtkApplication* gtk_app = nullptr;
	core::TimeComponet time_componet;
	std::string title;
	space::Point win_dimensions;

	Signaler signaler;

	int argc = 0;
	char** argv = nullptr;
	int subwin_cap = 5;
	int app_status = 0;
public:
	std::vector<Scene*> sub_scene_vect;
	std::vector<Window*> subwin_vect;
	App(const std::string& title, const space::Point& dimensions,  int argc, char* argv[]);
	~App();
	//bool process_close_request(GtkWidget* gtk_widget, gpointer user_data);
	void run(void(*activate_func)(GtkApplication*, gpointer), core::Messenger<App*>* signal_ptr);
	Error attach_main_window(Window* window);
	int attach_subwin(Window* window);
	Error attach_main_scene(Scene* scene);
	Error attach_sub_scene(Scene* scene);
	void display_main_window();
	bool request_subwin(const std::string& scene_name);


	Signal S_scene_request_subwin;

	Scene* get_main_scene();
	void set_subwin_cap(int cap);
	GtkWidget* get_scene_container();
	core::TimeComponet* get_time_componet();
	const space::Point& get_win_dimensions();

};
}
