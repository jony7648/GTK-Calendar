#include <iostream>
#include <gtk/gtk.h>
#include "core/app.h"
#include "core/window.h"
#include "core/space.h"
#include "project/scenes.h"
#include "gtk_componets.h"
#include "core/time_componet.h"
#include "core/messenger.h"
#include "core/util.h"

void activate(GtkApplication* gtk_app, gpointer user_data) {
	std::string title = "Calendar";

	core::Messenger<core::App*>* signal_data = (core::Messenger<core::App*>*)(user_data);
	core::App* app = signal_data->object;
	core::Window* window = new core::Window(gtk_app, title, app->get_win_dimensions());
	core::Window* subwin = new core::Window(gtk_app, title, app->get_win_dimensions());
	core::TimeComponet* time_componet = app->get_time_componet();

	core::Scene* main_scene = calender::create_main_scene(window, time_componet);
	core::Scene* note_scene = calender::create_note_scene();

	main_scene->container->present_widgets();

	app->attach_main_window(window);
	app->attach_sub_scene(note_scene);
	app->attach_subwin(subwin);
	app->attach_main_scene(main_scene);
	app->display_main_window();
	//subwin->display(note_scene);
}

int main(int argc, char *argv[]) {
	space::Point win_dimensions;

	core::Messenger<core::App*> signal;



	win_dimensions.x = 600;
	win_dimensions.y = 600;
	core::App app("org.jony.test", win_dimensions, argc, argv);
	std::cout << std::is_class_v<core::App> << "\n";

	signal.object = &app;

	app.set_subwin_cap(1);
	app.run(&activate, &signal);

	std::cout << "yes";
}
