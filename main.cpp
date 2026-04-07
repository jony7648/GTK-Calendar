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

void load_save_data(data::NoteContainer& note_container, core::csv::Writer& csv_writer) {
	std::vector<core::csv::Entry> data_map_vector;

	csv_writer.read_csv(data_map_vector);

	try {
		for (core::csv::Entry& entry : data_map_vector) {
			data::Note note;

			note.date.day = stoi(entry.get("Day"));
			note.date.month = stoi(entry.get("Month"));
			note.date.year = stoi(entry.get("Year"));
			note.text = entry.get("Note");
			
			note_container.add_note(note);
		}
	}


	catch (std::exception& e) {
		std::cout << "File failed to load, likely due to bad formatting!\n";
	}
}

void save_app_data(data::NoteContainer& note_container, core::csv::Writer& csv_writer, int save_count=-1) {
	std::vector<core::csv::Entry> csv_map_vec;

	int going_to_save_count = 0;
	for (data::Note& note : note_container) {
		if (save_count == 0) {
			break;
		}
		going_to_save_count++;

		save_count--;

		if (!note.should_save) {
			//std::cout << "SHOULD NOT SAVE\n";
			//note.display_info();
			//std::cout << "\n\n";
			continue;
		}

		csv_map_vec.push_back(core::csv::Entry());
		core::csv::Entry& entry = csv_map_vec.back();

		entry.add("Day",  std::to_string(note.date.day));	
		entry.add("Month", std::to_string(note.date.month));	
		entry.add("Year", std::to_string(note.date.year));	
		entry.add("Note", note.text);	

	}

	//fix the csv writer as it can only save a single note at a time

	csv_writer.write_csv(csv_map_vec);
	std::cout << "WE ARE ABOUT TO SAVE " << going_to_save_count << " notes!\n";
}

void signal_persist_data_note_released(core::EmitData<data::NoteContainer>& emit_data) {
	data::NoteContainer* note_container = emit_data.holder;
	core::csv::Writer* csv_writer = static_cast<core::csv::Writer*>(emit_data.receiver);
	auto* sig_data = static_cast<data::NoteContainer::SigNotesReleased*>(emit_data.sig_data);

	std::cout << "Hit the signal time to save now\n";

	//save_app_data(*note_container, *csv_writer, sig_data->save_count);

}

int main(int argc, char *argv[]) {
	std::vector<std::string> csv_header = {"Year", "Month", "Day", "Note"};
	std::string save_path = "test.save";

	data::PersistData persist_data;
	data::NoteContainer& note_container = persist_data.get_note_container();
	core::csv::Writer csv_writer(save_path, csv_header);
	csv_writer.set_equivalnce_bounds(0, 3);


	note_container.sig_handler.add_emit_func (
		data::NoteContainer::S_NotesReleased,
		signal_persist_data_note_released,
		&csv_writer
	);


	//load_save_data(persist_data.get_note_container(), csv_writer);

	note_container.attach_csv_writer(&csv_writer);
	note_container.load_new_notes();

	space::Point win_dimensions = {
		.x = 1200,
		.y = 600,
	};

	core::App app("org.jony.test", win_dimensions, argc, argv);
	core::TimeComponet* time_componet = app.get_time_componet();


	note_container.connect_time_componet_signals(*time_componet);

	project::ActivateSignal activate_signal = {
		.app = &app,
		.csv_writer = &csv_writer,
		.persist_data = &persist_data,
	};


	app.run(&activate, &activate_signal);

	//save_app_data(persist_data.get_note_container(), csv_writer);
}
