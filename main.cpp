#include <iostream>
#include <gtk/gtk.h>
#include "core/app.h"
#include "core/window.h"
#include "core/space.h"
#include "project/scenes.h"
#include "gtk_componets.h"
#include "core/time_componet.h"
#include "core/messenger.h"
#include "util.h"


void activate(GtkApplication* gtk_app, gpointer user_data) {
	std::string title = "Calendar";

	core::MessengerData<core::App>* signal_data = (core::MessengerData<core::App>*)(user_data);
	core::App* app = signal_data->object;
	core::Window* window = new core::Window(gtk_app, title, app->get_win_dimensions());
	core::TimeComponet* time_componet = app->get_time_componet();

	Scene* main_scene = calender::create_main_scene(time_componet);

	main_scene->container->present_widgets();
	window->set_scene(main_scene);
	window->display();
	app->attach_window(window);
}

int main(int argc, char *argv[]) {
	space::Point win_dimensions;

	core::MessengerData<core::App> signal;

	win_dimensions.x = 600;
	win_dimensions.y = 600;
	core::App app("org.jony.test", win_dimensions, argc, argv);

	signal.object = &app;

	app.run(&activate, &signal);
}
