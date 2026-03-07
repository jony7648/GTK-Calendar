#include <iostream>
#include "gtk_componets/gtk_componets.h"
#include "note_scene.h"
#include "persist_data.h"


const std::string NOTE_CONTAINER_STR = "Note Container";
const std::string NOTE_INPUT_NAME = "Note TextField";

data::Note* current_note = nullptr;

static void save_note_text(void* receiver_obj, void* emitter_obj, void*) {
	return;
	core::Scene* scene = static_cast<core::Scene*>(receiver_obj);

	gtkc::GridContainer* note_container = static_cast<gtkc::GridContainer*>(scene->get_widget_container().get_child_container(NOTE_CONTAINER_STR));
	gtkc::TextField* note_text_field = nullptr;
	std::string note_str = "";

	for (gtkc::Widget* widget : *note_container) {
		if (widget->get_name() == NOTE_INPUT_NAME) {
			note_text_field = static_cast<gtkc::TextField*>(widget);	
		}
	}

	//now we need to create a utility function to save this text

	note_str = note_text_field->get_text();

	std::cout << note_str << "\n";

	data::DaySave day_save;

	day_save.year = 2026;
	day_save.month = 5;
	day_save.day = 3;
	day_save.note = "Apples";

	data::save_data(day_save);



}

void prepare_note_scene(core::Scene* scene, data::Note* note) {
	core::TimeComponet* time_componet = nullptr;
	gtkc::GridContainer& main_container = scene->get_widget_container();
	gtkc::GridContainer* date_container = nullptr;
	gtkc::GridContainer* note_container = nullptr;
	gtkc::Label* date_label = nullptr;
	std::string date_str = "";
	gtkc::TextField* text_field = nullptr;


	time_componet = scene->get_time_componet();
	const std::string& month_str = time_componet->get_long_month_name(note->date.month);



	
	date_container = main_container.get_child_container("Date Container");	
	note_container = main_container.get_child_container("Date Container");	

	if (!date_container || !note_container) {
		return;
	}
	
	
	for (gtkc::Widget* widget : *date_container) {
		if (widget->get_name() == "Date Field") {
			date_label = static_cast<gtkc::Label*>(widget);	
		}

	}

	for (gtkc::Widget* widget : scene->get_widget_container()) {
		if (widget->get_name() == NOTE_INPUT_NAME) {
			text_field = static_cast<gtkc::TextField*>(widget);
		}
	}

	if (text_field == nullptr) {
		return;
	}
	
	//time_componet->get_full_day_str(note->day, note->month, note->year, date_str);
	time_componet->get_full_day_str(note->date, date_str);
	text_field->set_text(note->text);
	date_label->set_text(date_str);
	current_note = note;
}

void signal_window_displayed(void* receiver_obj, void* emitter_obj, void* sig_data_addr) {
	core::SigData* sig_data = static_cast<core::SigData*>(sig_data_addr);
	core::Scene* scene = static_cast<core::Scene*>(receiver_obj);
	gtkc::Button* emit_button = static_cast<gtkc::Button*>(emitter_obj);
	std::cout << receiver_obj << " " << emitter_obj << " " << &sig_data << "\n";

	data::Note* note = static_cast<data::Note*>(sig_data->obj_ptr);
	std::cout << "Note Text before text field " << note->text << "\n";
	std::cout << emit_button->get_text() << "\n";


	prepare_note_scene(scene, note);
}



gtkc::GridContainer* create_date_container(core::Scene* scene) {
	gtkc::GridContainer* date_container = new gtkc::GridContainer();
	date_container->set_name("Date Container");
	date_container->set_valign(GTK_ALIGN_CENTER);
	date_container->set_halign(GTK_ALIGN_CENTER);

	gtkc::Label* date_label = new gtkc::Label("Date Field", "Date", 0, 0, 1, 1);
	date_label->set_font_size(5);

	date_container->add_widget(date_label);
	date_container->present_widgets();


	return date_container;
}

gtkc::GridContainer* create_note_container(core::Scene* scene) {
	gtkc::GridContainer* note_container = new gtkc::GridContainer("Note Container", 1, 1);
	gtkc::TextField* text_field = new gtkc::TextField(NOTE_INPUT_NAME, 1, 1);
	//gtkc::Label* text_field = new gtkc::Label("Test", "Test", 0,0,1,1);

	note_container->set_grid_point(0, 1);


	note_container->add_widget(text_field);
	note_container->present_widgets();
	return note_container;
}

static void signal_window_closed(void* scene_addr, void* emitter_obj, void*) {
	core::Scene* scene = static_cast<core::Scene*>(scene_addr);
	gtkc::TextField* text_field = nullptr;

	for (gtkc::Widget* widget : scene->get_widget_container()) {
		if (widget->get_name() == NOTE_INPUT_NAME) {
			text_field = static_cast<gtkc::TextField*>(widget);
		}
	}


	const std::string& text_field_text = text_field->get_text();

	text_field->display_info();

	std::cout << "Compare: " << text_field_text << " " << current_note->text << "\n";;

	if (text_field_text != current_note->text) {
		std::cout << "The compare did pass\n";
		current_note->text = text_field_text;
		current_note->should_save = true;
	}
}

namespace project {
core::Scene* create_note_scene(core::TimeComponet* time_componet) {
	const std::string scene_name = "Note Scene";

	core::Scene* scene = nullptr;
	gtkc::GridContainer* date_container = nullptr;
	gtkc::GridContainer* note_container = nullptr;
	space::Point win_size;
	space::Point widget_spacing;

	win_size.x = 600;
	win_size.y = 600;

	widget_spacing.x = 32;
	widget_spacing.y = 30;
	
	scene = new core::Scene(scene_name, widget_spacing.x, widget_spacing.y);
	scene->set_custom_dimensions(win_size);
	scene->set_resizability(false);
	scene->sig_handler.add_emit_func(core::Scene::S_WINDOW_DISPLAYED, signal_window_displayed, scene);
	scene->sig_handler.add_emit_func(core::Scene::S_WINDOW_CLOSED, signal_window_closed, scene);

	gtkc::GridContainer& main_container = scene->get_widget_container();
	main_container.set_valign(GTK_ALIGN_START);
	main_container.set_halign(GTK_ALIGN_CENTER);

	date_container = create_date_container(scene);
	note_container = create_note_container(scene);

	main_container.add_widget(date_container);
	main_container.add_widget(note_container);



	return scene;	
}
}
