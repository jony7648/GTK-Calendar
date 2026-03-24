#pragma once
#include <iostream>

#include "util.h"
#include "messenger.h"
#include "space.h"
#include "window.h"
#include "time_componet.h"
#include "scene.h"
#include "error.h"
#include "signal_handler.h"

#include <gtk/gtk.h>

namespace core {
class Window;

class App {

public:
	enum SIGNALS {
		S_SCENE_REQUEST_SUBWIN,
		S_WINDOW_END_PROGRAM,
		S_SUBWIN_CLOSE,
	};


	template <typename T>
	struct SigSceneRequestSubwin {
		const std::string& scene_name;
		T& obj;
	};

	App(const std::string& title, const space::Point& dimensions,  int argc, char* argv[]);
	~App();
	//bool process_close_request(GtkWidget* gtk_widget, gpointer user_data);
	void run(void(*activate_func)(GtkApplication*, gpointer), void* activate_signal);
	void close();
	Error attach_main_window(Window* window);
	void create_subwins(unsigned int count);
	int attach_subwin(Window* window);
	Error attach_main_scene(Scene* scene, void* signal_data = nullptr);
	Error attach_sub_scene(Scene* scene);
	void display_main_window();
	void apply_provider(const std::string& css_dir_path);
	//Error request_subwin(void* emitter_obj, SigData* sig_data);
	Error request_subwin(core::Scene& scene, core::Window* p_emit_window = nullptr, void* emitter_obj = nullptr, core::Scene::SigRequestSubwin* sig_data = nullptr);

	SigHandler<App> sig_handler;
	
	//Signal S_scene_request_subwin;
	//Signal S_window_end_program;
	//Signal S_subwin_close;

	Scene* get_main_scene();
	std::vector<Scene*>& get_sub_scene_vect();
	GtkWidget* get_scene_container();
	core::TimeComponet* get_time_componet();
	const space::Point& get_win_dimensions();



private:
	Scene* main_scene = nullptr;
	std::vector<Scene*> sub_scene_vect;
	GtkApplication* gtk_app = nullptr;
	core::TimeComponet time_componet;
	std::string title;
	space::Point win_dimensions;

	Window* main_window = nullptr;
	std::vector<Window*> subwin_vect;

	GtkCssProvider* _css_provider = nullptr;
	GdkDisplay* _default_display = nullptr;

	int argc = 0;
	char** argv = nullptr;
	const short DEFAULT_SUBWIN_COUNT = 5;
	int app_status = 0;
};
}
