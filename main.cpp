#include <iostream>
#include <gtk/gtk.h>
#include "core/app.h"
#include "core/window.h"
#include "core/space.h"
#include "gtk_componets.h"
#include "core/time_componet.h"
#include "core/messenger.h"
#include "core/util.h"
#include "project/project_scenes.h"
#include "project/signals.h"
#include "project/persist_data.h"

void activate(GtkApplication* gtk_app, gpointer user_data) {
	std::string title = "Calendar";

	project::ActivateSignal* signal_data = static_cast<project::ActivateSignal*>(user_data);

	core::App* app = signal_data->app;
	core::Window* window = new core::Window(gtk_app, title, app->get_win_dimensions());
	core::Window* subwin = new core::Window(gtk_app, title, app->get_win_dimensions());
	core::TimeComponet* time_componet = app->get_time_componet();

	core::Scene* main_scene = project::create_main_scene(time_componet, signal_data->csv_writer, signal_data->persist_data);
	core::Scene* note_scene = project::create_note_scene(time_componet);

	main_scene->get_widget_container().present_widgets();
	note_scene->get_widget_container().present_widgets();

	app->attach_main_window(window);
	app->attach_sub_scene(note_scene);
	app->attach_subwin(subwin);
	app->attach_main_scene(main_scene, signal_data);
	//app->apply_provider("gtk_componets/css_styles");
	app->display_main_window();
	//subwin->display(note_scene);
}

void load_save_data(data::PersistData& persist_data, core::CsvWriter& csv_writer) {
	std::vector<core::CsvWriter::DataMap> data_map_vector;

	data::get_data(csv_writer, data_map_vector);

	for (auto& map : data_map_vector) {
		data::Note& note = persist_data.add_note();
		note.day = stoi(map["Day"]);
		note.month = stoi(map["Month"]);
		note.year = stoi(map["Year"]);
		note.text = map["Note"];
	}
}

int main(int argc, char *argv[]) {
	std::string save_path = "test.save";

	data::PersistData persist_data;
	core::CsvWriter csv_writer(save_path);

	load_save_data(persist_data, csv_writer);
	

	persist_data.display_notes();

	space::Point win_dimensions;


	project::ActivateSignal activate_signal;



	win_dimensions.x = 1200;
	win_dimensions.y = 600;
	core::App app("org.jony.test", win_dimensions, argc, argv);

	activate_signal.app = &app;
	activate_signal.csv_writer = &csv_writer;
	activate_signal.persist_data = &persist_data;


	app.set_subwin_cap(1);
	app.run(&activate, &activate_signal);

}
