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
#define TAG_MONTH_CALBUTTON "MonthCalButton"

data::PersistData* persist_data = nullptr;
project::Config* G_config = nullptr;

static void arrange_cal_buttons(gtkc::GridContainer* container, core::TimeComponet* time_componet, int month) {
	//create the system for other months
	gtkc::Widget* widget = nullptr;
	int grid_x = 0;
	int grid_y = 0;

	gtkc::Widget::Theme theme = gtkc::Widget::get_theme(G_config->get_value("theme"));

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

		if (persist_data->note_container.note_exists({.day=day, .month=month, .year=date.year})) {
			widget->load_css("ShadeButton", theme);
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

static void signal_advance_month_button_clicked(core::G_EmitData& emit_data) {
	core::Scene* scene = static_cast<core::Scene*>(emit_data.receiver);
	gtkc::Button* button = static_cast<gtkc::Button*>(emit_data.emitter);
	core::TimeComponet* time_componet = scene->get_time_componet();
	
	time_componet->advance_menu_month(button->get_sig_data());
}

static void signal_time_componet_date_changed(core::EmitData<core::TimeComponet>& emit_data) {
	core::Scene* scene = static_cast<core::Scene*>(emit_data.receiver);
	core::TimeComponet* time_componet = emit_data.holder;
	gtkc::GridContainer* container = nullptr;

	if (!scene) {
		std::cout << "Scene is a nullptr\n";
		return;
	}

	if (!time_componet) {
		std::cout << "TimeComponet is a nullptr\n";
		return;
	}

	container = scene->get_widget_container().get_child_container(CONTAINER_NAME_CALBUTTON);

	if (!container) {
		std::cout << "This Container is a nullptr\n";
		return;
	}
	
	arrange_cal_buttons(container, time_componet, time_componet->get_menu_month());

	for (gtkc::Widget* widget : scene->get_widget_container()) {
		if (widget->get_name() == "Date Container") {
			update_date_header(widget, scene->get_time_componet());
		}
	}
}

static void month_button_clicked(core::G_EmitData& emit_data) {
	core::Scene* scene = static_cast<core::Scene*>(emit_data.receiver);
	gtkc::Button* button = static_cast<gtkc::Button*>(emit_data.emitter);
	core::TimeComponet* time_componet = nullptr;

	if (!scene) {
		std::cout << "Scene is a nullptr!";
		return;
	}

	if (!button) {
		std::cout << "Button is a nullptr!";
		return;
	}

	time_componet = scene->get_time_componet();

	if (!time_componet) {
		std::cout << "TimeComponet is a nullptr!";
		return;
	}

	time_componet->set_menu_month(button->get_sig_data());
	
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

	scene->sig_handler.emit_data(core::Scene::S_REQUEST_SUBWIN, &request_data, button);
}

static void signal_note_container_note_added(core::EmitData<data::NoteContainer> emit_data) {
	core::Scene* scene = static_cast<core::Scene*>(emit_data.receiver);
	data::NoteContainer::SigNoteAdded* sig_data = static_cast<data::NoteContainer::SigNoteAdded*>(emit_data.sig_data);
	data::Note& received_note = sig_data->back_note;
	data::NoteContainer* note_container = emit_data.holder;

	gtkc::GridContainer* calbutton_container = nullptr;

	if (!received_note.new_note) {
		return;
	}

	received_note.new_note = false;


	//adjust the button shade that matches the notes date accordingly
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

		if (button->get_text() == std::to_string(received_note.date.day)) {
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

static void create_mini_cal_buttons(gtkc::GridContainer* day_container, core::Scene* scene) {
	const int MAX_DAYS_PER_MTH = 31;
	const int MONTHS_PER_ROW = 4;

	gtkc::Button* button = nullptr;

	std::string name = "";
	std::string text = "";

	core::TimeComponet* time_componet = scene->get_time_componet();


	gtkc::Widget::CommonInitProperties shared_properties = {
		.tag = TAG_MONTH_CALBUTTON,
		.scale = {1,1},
		.hexpand = false,
		.vexpand = false,
	};

	gtkc::Button::InitProperties button_properties = {
		.base_prop = {.shared_properties=shared_properties},
	};

	project::calendar::MonthButtonClicked sig_data = {
		.time_componet = *time_componet,
	};


	for (int i=0; i<core::TimeComponet::MONTH_COUNT; i++) {
		button_properties.base_prop.name = std::to_string(i+1);		
		button_properties.text = time_componet->get_short_month_name(i);
		button_properties.base_prop.grid_point = {.x=i%MONTHS_PER_ROW, .y=i/MONTHS_PER_ROW};
		button_properties.font_size = 2;
		button = new gtkc::Button(button_properties);
		button->set_sig_data(i);

		button->size_request(1,1);
		day_container->add_widget(button);
		scene->sig_handler.g_listen(button, "clicked", month_button_clicked);
		
	}
}

static void create_mini_advance_buttons(gtkc::GridContainer* grid_container, core::Scene* scene) {
	int month_jump_count_arr[] = {3, 6, 12, 15};
	size_t month_arr_size = sizeof(month_jump_count_arr)/sizeof(month_jump_count_arr[0]);
	
	int jump_pairs_per_row = 2;


	gtkc::Button* left_button = nullptr;
	gtkc::Button* right_button = nullptr;

	gtkc::Widget::CommonInitProperties shared_properties = {
		.scale = {1,1},
		.hexpand = true,
		.vexpand = true,
	};

	gtkc::Button::InitProperties left_button_properties = {
		.base_prop = {.shared_properties=shared_properties},
		.font_size = 2,
	};

	gtkc::Button::InitProperties right_button_properties = {
		.base_prop = {.shared_properties=shared_properties},
		.font_size = 2,
	};


	//configure and add the widgets
	for (int i=0; i<month_arr_size; i++) {
		int right_jump_count = month_jump_count_arr[i];
		
		left_button_properties.base_prop.name = "Left Button " + std::to_string(i);
		right_button_properties.base_prop.name = "Right Button " + std::to_string(i);

		left_button_properties.base_prop.grid_point = {
			(-i%jump_pairs_per_row)-1, -i/jump_pairs_per_row
		};	

		right_button_properties.base_prop.grid_point = {
			(i%jump_pairs_per_row)+1, -i/jump_pairs_per_row
		};	

		left_button_properties.base_prop.sig_data = -right_jump_count;	
		right_button_properties.base_prop.sig_data = right_jump_count;	


		left_button_properties.text = std::to_string(-right_jump_count);
		right_button_properties.text = '+' + std::to_string(right_jump_count);

		left_button = new gtkc::Button(left_button_properties);
		right_button = new gtkc::Button(right_button_properties);

		grid_container->add_widget(left_button);
		grid_container->add_widget(right_button);

		scene->sig_handler.g_listen(left_button, "clicked", signal_advance_month_button_clicked);
		scene->sig_handler.g_listen(right_button, "clicked", signal_advance_month_button_clicked);
	}
}

static gtkc::GridContainer* create_mini_cal_grid(core::Scene* scene) {
	if (!scene) {
		std::cout << "Scene is a nullptr!\n";
		return nullptr;
	}

	core::TimeComponet* time_componet = scene->get_time_componet();

	if (!time_componet) {
		std::cout << "No time componet\n";
		return nullptr;
	}


	gtkc::GridContainer* cal_grid = new gtkc::GridContainer("Mini Cal Grid", 3, 3);
	gtkc::GridContainer* advance_month_container = new gtkc::GridContainer("Advance Month Container", 5, 5);
	gtkc::GridContainer* day_container = new gtkc::GridContainer("Day container", 9, 5);
	

	day_container->set_grid_point(0,1);
	day_container->set_hexpand(true);
	day_container->set_vexpand(true);
	day_container->set_valign(GTK_ALIGN_FILL);
	day_container->set_halign(GTK_ALIGN_FILL);
	day_container->set_widget_spacing(10);

	cal_grid->set_grid_point(0, 1);
	cal_grid->set_widget_spacing(1, 23);
	cal_grid->set_hexpand(false);
	cal_grid->set_valign(GTK_ALIGN_FILL);
	cal_grid->set_halign(GTK_ALIGN_FILL);

	create_mini_cal_buttons(day_container, scene);
	create_mini_advance_buttons(advance_month_container, scene);

	day_container->present_widgets();

	advance_month_container->set_grid_point(0,0);
	advance_month_container->set_hexpand(true);
	advance_month_container->set_vexpand(true);
	advance_month_container->set_halign(GTK_ALIGN_FILL);
	advance_month_container->set_widget_spacing(10);

	advance_month_container->present_widgets();

	cal_grid->add_widget(advance_month_container);
	cal_grid->add_widget(day_container);

	cal_grid->present_widgets();

	return cal_grid;
}

static gtkc::GridContainer* create_info_container(core::Scene* scene) {
	gtkc::GridContainer* info_container = new gtkc::GridContainer("Info Contanier", 6,6);
	gtkc::GridContainer* top_container = new gtkc::GridContainer("Top Container", 6,1);
	gtkc::GridContainer* mini_cal_grid = create_mini_cal_grid(scene);

	info_container->set_valign(GTK_ALIGN_FILL);
	//info_container->set_halign(GTK_ALIGN_FILL);
	info_container->set_grid_point(0,0);
	info_container->set_hexpand(false);

	top_container->set_valign(GTK_ALIGN_START);
	top_container->set_halign(GTK_ALIGN_END);
	top_container->set_widget_spacing(64,1);
	top_container->set_grid_point(0,0);

	gtkc::Button* prev_month_button = new gtkc::Button("Previous Month", "Prev.", 0, 0, 1, 1);
	gtkc::Button* next_month_button = new gtkc::Button("Next Month", "Next", 1, 0, 1, 1);
	
	top_container->add_widget(prev_month_button);
	top_container->add_widget(next_month_button);


	prev_month_button->set_sig_data(-1);
	next_month_button->set_sig_data(1);
	prev_month_button->set_tag("Month Scroll");
	next_month_button->set_tag("Month Scroll");

	scene->sig_handler.g_listen(next_month_button, "clicked", signal_advance_month_button_clicked);
	scene->sig_handler.g_listen(prev_month_button, "clicked", signal_advance_month_button_clicked);

	top_container->present_widgets();
	info_container->add_widget(top_container);
	info_container->add_widget(mini_cal_grid);

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
core::Scene* create_main_scene(core::TimeComponet* time_componet, core::csv::Writer* csv_writer, data::PersistData* persist_data_object, project::Config& config) {
	persist_data = persist_data_object;
	G_config = &config;

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
	scene->set_time_componet(time_componet);

	scene->set_custom_dimensions(scene_dimensions);
	persist_data->note_container.sig_handler.add_emit_func(data::NoteContainer::S_NOTE_ADDED, signal_note_container_note_added, scene);

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
	
	
	add_weekday_header(time_componet, widget_vector);
	create_cal_buttons(scene, cal_button_vector);


	
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

	time_componet->sig_handler.add_emit_func(core::TimeComponet::S_MenuDateChanged, signal_time_componet_date_changed, scene);

	return scene;
}
}
