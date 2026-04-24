#include <iostream>

#include "core/app.h"
#include "core/window.h"
#include "core/space.h"
#include "gtk_componets.h"
#include "core/time_componet.h"
#include "core/messenger.h"
#include "core/util.h"
#include "project/project_scenes.h"
#include "project/debug_scene.h"
#include "project/signals.h"
#include "project/persist_data.h"
#include "project/config.h"

#include <gtk/gtk.h>

const bool DEBUG = false;

void activate(GtkApplication* gtk_app, gpointer user_data) {
	std::string title = "Calendar";

	project::ActivateSignal* signal_data = static_cast<project::ActivateSignal*>(user_data);

	core::Scene* debug_scene = nullptr;

	core::App* app = signal_data->app;
	core::Window* window = new core::Window(gtk_app, title);
	core::TimeComponet* time_componet = app->get_time_componet();

	core::Scene* main_scene = project::create_main_scene(time_componet, signal_data->csv_writer, signal_data->persist_data, signal_data->config);
	core::Scene* note_scene = project::create_note_scene(time_componet);

	main_scene->get_widget_container().present_widgets();
	note_scene->get_widget_container().present_widgets();

	app->attach_main_window(window);
	app->create_subwins(3);
	app->attach_sub_scene(note_scene);
	app->attach_main_scene(main_scene, signal_data);


	//app->apply_provider("gtk_componets/css_styles");
	//subwin->display(note_scene);

	//do debug stuff
	if (DEBUG) {
		debug_scene = project::create_debug_scene(signal_data);
		app->attach_sub_scene(debug_scene);
		app->request_subwin(*debug_scene, nullptr);
	}

	app->display_main_window();
}

int main(int argc, char *argv[]) {
	core::set_errors(); //set error type messages
	std::string config_path = "config.cfg";

	project::Config config(config_path);

	std::vector<std::string> csv_header = {"Year", "Month", "Day", "Note"};
	std::string save_path = "calendar_save.csv";

	data::PersistData persist_data;
	data::NoteContainer& note_container = persist_data.get_note_container();
	core::csv::Writer csv_writer(save_path, csv_header);
	csv_writer.set_equivalnce_bounds(0, 3);

	space::Point win_dimensions = {
		.x = 1200,
		.y = 600,
	};

	core::App app("org.jony.Calendar", win_dimensions, argc, argv);
	core::TimeComponet* time_componet = app.get_time_componet();


	note_container.attach_csv_writer(&csv_writer);
	note_container.connect_time_componet_signals(*time_componet);
	note_container.load_new_notes();

	core::CssProvider::Theme theme = core::CssProvider::get_theme(config.get_value("theme"));


	core::CssProvider::set_styles_dir("css_styles");
	core::CssProvider::change_global_theme(theme);

	project::ActivateSignal activate_signal = {
		.app = &app,
		.csv_writer = &csv_writer,
		.persist_data = &persist_data,
		.config = config,
	};

	app.run(&activate, &activate_signal);
	note_container.save_notes();

	config.save_config({
		.config_path = config_path,
		.theme = core::CssProvider::get_G_theme(),
	});
}
