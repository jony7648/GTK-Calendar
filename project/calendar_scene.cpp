#include <iostream>
#include <gtk/gtk.h>
#include "gtk_componets.h"
#include "calendar_scene.h"
#include "signals.h"
#include "core/window.h"
#include "core/util.h"
#include "persist_data.h"
#include "note_container.h"

#define CONTAINER_NAME_CALBUTTON "CalButton Container"
#define TAG_CALBUTTON "CalButton"

data::PersistData* persist_data = nullptr;

static void arrange_cal_buttons(gtkc::GridContainer* container, core::TimeComponet* time_componet, int month) {
	//create the system for other months
	gtkc::Widget* widget = nullptr;
	int grid_x = 0;
	int grid_y = 0;

	const core::Date& date = time_componet->get_menu_date();

	int day = 0;
	int start_weekday = time_componet->get_starting_weekday(month, date.year);
	int day_count = time_componet->get_day_count(month) + time_componet->is_leap_month(date.month, date.year);
	int pos_index = 0;

	const std::vector<gtkc::Widget*>& widget_vector = container->get_widget_vector();


	for (int i=-1, vec_i=0; vec_i<widget_vector.size(); vec_i++) {
		widget = widget_vector.at(vec_i);
		if (!widget || widget->get_tag() != TAG_CALBUTTON) {
			continue;
		}

		i++;

		day = i+1;

		//if the month has less than 31 days hide the buttons that
		//have a higher day value than the month's day count
		if (i >= day_count) {
			widget->hide();
			continue;

		}

		if (persist_data->note_exists(day, month, date.year)) {
			widget->load_css("ShadeButton");
		}
		else {
			widget->load_css();
		}

		pos_index = i + start_weekday;
		grid_x = (pos_index) % 7;
		grid_y = (pos_index / 7) + 1;

		widget->set_grid_point(grid_x, grid_y);
		widget->show();
		//add a refresh function to allow widgets to move
		//even when already presenting and let widgets know if they
		//are presenting or not
	}
}

static void update_date_header(gtkc::Widget* container_location, core::TimeComponet* time_componet) {
	const core::Date& date = time_componet->get_menu_date();
	std::string header_str;

	gtkc::GridContainer* header_container = static_cast<gtkc::GridContainer*>(container_location);
	gtkc::Label* label = nullptr;

	for (gtkc::Widget* widget : *header_container) {
		if (!widget->is_type(gtkc::Type::Label)) {
			continue;	
		}

		label = static_cast<gtkc::Label*>(widget);
	}

	if (label == nullptr) {
		std::cout << "Failed to find date field!\n";
		return;
	}

	time_componet->get_month_year_str(date, header_str);

	label->set_text(header_str);

}

static void signal_button_clicked(core::G_EmitData& emit_data) {
	core::Scene* scene = static_cast<core::Scene*>(emit_data.receiver);
	gtkc::Button* button = static_cast<gtkc::Button*>(emit_data.emitter);
	core::TimeComponet* time_componet = scene->get_time_componet();
	gtkc::GridContainer* container = nullptr;

	container = scene->get_widget_container().get_child_container(CONTAINER_NAME_CALBUTTON);


	time_componet->advance_menu_month(button->get_sig_data());
	arrange_cal_buttons(container, time_componet, time_componet->get_menu_month());

	for (gtkc::Widget* widget : scene->get_widget_container()) {
		if (widget->get_name() == "Date Container") {
			update_date_header(widget, scene->get_time_componet());
		}
	}
}


static void signal_open_note_window(core::G_EmitData& emit_data) {
	core::Scene* scene = static_cast<core::Scene*>(emit_data.receiver);
	gtkc::Button* button = static_cast<gtkc::Button*>(emit_data.emitter);
	core::TimeComponet* time_componet = scene->get_time_componet();
	data::Note* note_ptr = nullptr;

	project::calendar::SigOpenNoteWindow sig_data = {
		.persist_data = *persist_data,
		.date = time_componet->get_menu_date(),
	};


	core::Scene::SigRequestSubwin request_data = {
		.scene_name = "Note Scene",
		.sig_ptr = &sig_data,
	};

	if (time_componet == nullptr) {
		return;	
	}


	time_componet->set_menu_day(std::stoi(button->get_text()));
	const core::Date& date = time_componet->get_menu_date();
	

	note_ptr = persist_data->get_note(date);


	/*
	if (note_ptr) {
		sig_data.obj_ptr = note_ptr;
	}
	else {
		note_ptr = &persist_data->note_container.after_back();
		persist_data->note_container.reset(note_ptr);

		note_ptr->date.day = std::stoi(button->get_text());
		note_ptr->date.month = time_componet->get_menu_month();
		note_ptr->date.year = time_componet->get_menu_year();

		sig_data.obj_ptr = note_ptr;
	}
	*/



	scene->sig_handler.emit_data(core::Scene::S_REQUEST_SUBWIN, &request_data, button);
}

static void signal_window_closed(core::EmitData<core::Scene> emit_data) {
	auto* sig_data = static_cast<core::Window::SignalWindowClose*>(emit_data.sig_data);

	core::Scene* scene = emit_data.holder;
	core::Scene* sub_scene = static_cast<core::Scene*>(sig_data->subscene);	
	data::Note& back_note = persist_data->note_container.back();
	data::NoteContainer& note_container = persist_data->note_container;

	gtkc::GridContainer* calbutton_container = nullptr;


	
	if (!sub_scene) {
		std::cout << "Error: Sub Scene is nullptr";
	}

	if (!back_note.new_note) {
		return;
	}

	back_note.new_note = false;


	//adjust the button shade that matches the notes date accodingly
	for (gtkc::Widget* widget : scene->get_widget_container()) {
		if (widget->get_name() == CONTAINER_NAME_CALBUTTON) {
			calbutton_container = static_cast<gtkc::GridContainer*>(widget);
		}
	}

	if (!calbutton_container) {
		std::cout << "Error cal button is a nullptr\n";
		return;
	}

	for (gtkc::Widget* widget : *calbutton_container) {
		gtkc::Button* button = nullptr;

		if (!widget->is_type(gtkc::Type::Button)) {
			continue;
		}

		button = static_cast<gtkc::Button*>(widget);

		if (button->get_text() == std::to_string(back_note.date.day)) {
			button->load_css("ShadeButton");
			button->reattach();
		}
	}
}

static void add_weekday_header(core::TimeComponet* time_componet, std::vector<gtkc::Widget*>& widget_vector) {
	gtkc::Label* widget = nullptr;
	std::string name = "";
	std::string button_text = "";
	int row = 0;
	int grid_x = 0;
	int grid_y = 0;
	int btn_index;
	int row_count = 1;
	int column_count = 1;
	int day_pos = 0;


	for (int i=0; i<time_componet->WEEKDAY_COUNT; i++) {
		name = time_componet->get_weekday_name(i);
		button_text = name;
		grid_x = i;
		grid_y = 0;
		widget = new gtkc::Label(name, button_text, grid_x, grid_y, 1,1);
		widget->set_font_size(4);
		widget_vector.push_back(widget);
	}
}

static void create_cal_buttons(core::Scene* scene, std::vector<gtkc::Widget*>& widget_vector) {
	const int MAX_DAYS_PER_MTH = 31;

	gtkc::Button* button = nullptr;
	std::string name = "";
	std::string text = "";


	for (int i=0; i<MAX_DAYS_PER_MTH; i++) {
		name = std::to_string(i+1);
		text = name;
		button = new gtkc::Button(name, text, 0, 3, 1, 1);

		button->set_tag(TAG_CALBUTTON);
		widget_vector.push_back(button);
		scene->sig_handler.g_listen(button, "clicked", signal_open_note_window);
	}
} 

static gtkc::GridContainer* create_info_container(core::Scene* scene) {
	gtkc::GridContainer* info_container = new gtkc::GridContainer("Info Contanier", 6,6);
	gtkc::GridContainer* top_container = new gtkc::GridContainer("Top Container", 6,1);

	info_container->set_valign(GTK_ALIGN_FILL);
	//info_container->set_halign(GTK_ALIGN_FILL);
	info_container->set_grid_point(0,0);
	info_container->set_hexpand(false);

	top_container->set_valign(GTK_ALIGN_START);
	top_container->set_halign(GTK_ALIGN_END);
	top_container->set_widget_spacing(64,1);
	top_container->set_grid_point(1,0);

	gtkc::Button* prev_month_button = new gtkc::Button("Previous Month", "Prev.", 0, 0, 1, 1);
	gtkc::Button* next_month_button = new gtkc::Button("Next Month", "Next", 1, 0, 1, 1);

	
	top_container->add_widget(prev_month_button);
	top_container->add_widget(next_month_button);


	prev_month_button->set_sig_data(-1);
	next_month_button->set_sig_data(1);
	prev_month_button->set_tag("Month Scroll");
	next_month_button->set_tag("Month Scroll");

	scene->sig_handler.g_listen(next_month_button, "clicked", signal_button_clicked);
	scene->sig_handler.g_listen(prev_month_button, "clicked", signal_button_clicked);

	top_container->present_widgets();
	info_container->add_widget(top_container);

	info_container->present_widgets();

	return info_container;	
}

static gtkc::GridContainer* create_cal_button_container() {
	gtkc::GridContainer* button_container = nullptr;

	button_container = new gtkc::GridContainer(CONTAINER_NAME_CALBUTTON, 1, 1);
	button_container->set_grid_point(0,1);
	button_container->set_widget_spacing(15);

	button_container->set_valign(GTK_ALIGN_FILL);
	button_container->set_halign(GTK_ALIGN_FILL);

	return button_container;
}

static gtkc::GridContainer* create_date_header_container(const core::Scene* scene, const std::string& date_str) {
	gtkc::GridContainer* date_container = nullptr;	
	gtkc::Label* date_field = nullptr;
	
	
	date_container = new gtkc::GridContainer("Date Container", 1, 1);
	date_container->set_grid_point(0,0);
	date_container->set_vexpand(false);


	date_field = new gtkc::Label("Date Field", "February 2026", 0, 0, 1, 1);
	date_field->set_hexpand(true);
	date_field->set_font_size(8);
	date_field->set_text(date_str);

	date_container->add_widget(date_field);

	date_container->set_valign(GTK_ALIGN_FILL);
	date_container->set_halign(GTK_ALIGN_START);
	date_container->set_widget_spacing(15);
	date_container->present_widgets();

	return date_container;
}


namespace project {
core::Scene* create_main_scene(core::TimeComponet* time_componet, core::csv::Writer* csv_writer, data::PersistData* persist_data_object) {
	persist_data = persist_data_object;

	const std::string scene_name = "Main Scene";
	int day_count = time_componet->get_day_count();

	gtkc::GridContainer* info_container = nullptr;
	gtkc::GridContainer* date_header_container = nullptr;
	gtkc::GridContainer* cal_button_container = nullptr;

	std::string full_day_str;
	core::Date date;

	space::Point scene_dimensions;
	scene_dimensions.x = 600;
	scene_dimensions.y = 600;

	std::vector<gtkc::Widget*> widget_vector;
	std::vector<gtkc::Widget*> cal_button_vector;


	date = time_componet->get_menu_date();
	time_componet->get_month_year_str(date, full_day_str);

	core::Scene* scene = new core::Scene("Main Scene", 32,5);

	scene->set_custom_dimensions(scene_dimensions);
	scene->sig_handler.add_emit_func(core::Scene::S_WINDOW_CLOSED, signal_window_closed, scene);

	gtkc::GridContainer& main_container = scene->get_widget_container();
	main_container.set_valign(GTK_ALIGN_FILL);
	main_container.set_halign(GTK_ALIGN_FILL);
	main_container.set_widget_spacing(30,1);

	gtkc::GridContainer* cal_container = new gtkc::GridContainer("Cal Container", 32, 32);
	cal_container->set_tag("CalButton Container");
	cal_container->set_valign(GTK_ALIGN_FILL);
	cal_container->set_halign(GTK_ALIGN_FILL);
	cal_container->set_grid_point(1,0);
	cal_container->set_widget_spacing(15);

	info_container = create_info_container(scene);
	date_header_container = create_date_header_container(scene, full_day_str);
	cal_button_container = create_cal_button_container();
	
	//cal_container->set_scale(3,3);


	
	//add_cal_days(scene, widget_vector, day_count, starting_weekday);
	
	add_weekday_header(time_componet, widget_vector);
	create_cal_buttons(scene, cal_button_vector);



	
	//info_container->present_widgets();
	

	cal_button_container->add_widget_vector(widget_vector);
	cal_button_container->add_widget_vector(cal_button_vector);
	cal_button_container->present_widgets();

	

	cal_container->add_widget(date_header_container);
	cal_container->add_widget(cal_button_container);
	
	cal_container->present_widgets();

	arrange_cal_buttons(cal_button_container, time_componet, time_componet->get_menu_month());


	//main_container.add_widget(date_header_container);
	main_container.add_widget(cal_container);
	main_container.add_widget(info_container);


	return scene;
}
}
