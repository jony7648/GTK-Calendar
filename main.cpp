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

#include <gtk/gtk.h>



const bool DEBUG = true;

void activate(GtkApplication* gtk_app, gpointer user_data) {
	std::string title = "Calendar";

	project::ActivateSignal* signal_data = static_cast<project::ActivateSignal*>(user_data);

	core::Scene* debug_scene = nullptr;

	core::App* app = signal_data->app;
	core::Window* window = new core::Window(gtk_app, title);
	core::TimeComponet* time_componet = app->get_time_componet();

	core::Scene* main_scene = project::create_main_scene(time_componet, signal_data->csv_writer, signal_data->persist_data);
	core::Scene* note_scene = project::create_note_scene(time_componet);

	main_scene->get_widget_container().present_widgets();
	note_scene->get_widget_container().present_widgets();

	app->attach_main_window(window);
	app->create_subwins(8);
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

void load_save_data(data::PersistData& persist_data, core::csv::Writer& csv_writer) {
	std::vector<core::csv::Entry> data_map_vector;

	data::NoteContainer& note_container = persist_data.get_note_container();

	csv_writer.read_csv(data_map_vector);
	try {
		for (core::csv::Entry& entry : data_map_vector) {
			note_container.add_note(data::Note());

			data::Note& note = note_container.back();

			note.date.day = stoi(entry.get("Day"));
			note.date.month = stoi(entry.get("Month"));
			note.date.year = stoi(entry.get("Year"));
			note.text = entry.get("Note");
		}
	}

	catch (std::exception& e) {
		std::cout << "File failed to load, likely due to bad formatting!\n";
	}
}

void save_app_data(data::PersistData& persist_data, core::csv::Writer& csv_writer) {
	std::vector<core::csv::Entry> csv_map_vec;
	std::vector<std::string> a;

	for (const data::Note& note : persist_data.get_note_container()) {
		if (!note.should_save) {
			continue;
		}

		csv_map_vec.push_back(core::csv::Entry());
		core::csv::Entry& entry = csv_map_vec.back();

		entry.add("Day",  std::to_string(note.date.day));	
		entry.add("Month", std::to_string(note.date.month));	
		entry.add("Year", std::to_string(note.date.year));	
		entry.add("Note", note.text);	
		entry.to_vec({"aiace"}, a);
	}

	csv_writer.write_csv(csv_map_vec);

}

int main(int argc, char *argv[]) {
	std::vector<std::string> csv_header = {"Year", "Month", "Day", "Note"};
	std::string save_path = "test.save";

	data::PersistData persist_data;
	core::csv::Writer csv_writer(save_path, csv_header);
	csv_writer.set_equivalnce_bounds(0, 3);


	load_save_data(persist_data, csv_writer);

	space::Point win_dimensions;

	project::ActivateSignal activate_signal;



	win_dimensions.x = 1200;
	win_dimensions.y = 600;

	core::App app("org.jony.test", win_dimensions, argc, argv);

	activate_signal.app = &app;
	activate_signal.csv_writer = &csv_writer;
	activate_signal.persist_data = &persist_data;


	app.run(&activate, &activate_signal);



	persist_data.display_notes();

	save_app_data(persist_data, csv_writer);
}
