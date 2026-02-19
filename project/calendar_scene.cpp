#include <iostream>
#include <gtk/gtk.h>
#include "gtk_componets.h"
#include "calendar_scene.h"
#include "core/window.h"
#include "core/util.h"
#include "persist_data.h"

data::PersistData* persist_data = nullptr;

static void arrange_cal_buttons(core::TimeComponet* time_componet, std::vector<gtkc::Widget*>& widget_vector, int month) {
	//create the system for other months
	gtkc::Widget* widget = nullptr;
	int grid_x = 0;
	int grid_y = 0;

	int day = 0;
	int year = time_componet->get_menu_year();
	std::cout << year << "\n";
	int start_weekday = time_componet->get_starting_weekday(month, year);
	int day_count = time_componet->get_day_count(month);
	int pos_index = 0;

	for (int i=0; i<widget_vector.size(); i++) {
		day = i+1;
		widget = widget_vector.at(i);

		if (i >= day_count) {
			widget->hide();
			break;
		}

		if (persist_data->note_exists(day, month, year)) {
			std::cout << "Day " << day << " exists!\n";
			widget->load_css("ShadeButton");
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
	int month = time_componet->get_menu_month();
	int	year = time_componet->get_menu_year();

	gtkc::GridContainer* header_container = static_cast<gtkc::GridContainer*>(container_location);
	gtkc::Label* label = nullptr;

	for (gtkc::Widget* widget : *header_container) {
		if (widget->get_type() != "Label") {
			continue;	
		}
		label = static_cast<gtkc::Label*>(widget);
	}

	if (label == nullptr) {
		std::cout << "Failed to find date field!\n";

		return;
	}

	const std::string& month_name = time_componet->get_long_month_name(month);

	label->set_text(month_name + " " + std::to_string(year));

}

static void signal_button_clicked(void* receiver_obj, void* emitter_obj) {
	core::Scene* scene = static_cast<core::Scene*>(receiver_obj);
	core::TimeComponet* time_componet = scene->get_time_componet();
	gtkc::Button* button = static_cast<gtkc::Button*>(emitter_obj);
	gtkc::GridContainer* cal_container = nullptr;

	std::vector<gtkc::Widget*> widget_vec;
	std::vector<gtkc::Widget*> cal_container_vec;

	scene->get_tagged_widgets(widget_vec, "Cal Buttons");
	scene->get_tagged_widgets(cal_container_vec, "CalButton Container");

	/*
	for (auto widget : widget_vec) {
		std::cout << "Iterated Widget " << widget->get_name() << "\n";
	}
	*/

	for (gtkc::Widget* widget : widget_vec) {
		//std::cout << widget->get_name() << "\n";
	}

	cal_container = static_cast<gtkc::GridContainer*>(cal_container_vec.at(0));


	time_componet->advance_menu_month(button->get_sig_data());
	arrange_cal_buttons(scene->get_time_componet(), widget_vec, time_componet->get_menu_month());

	for (gtkc::Widget* widget : scene->get_widget_container()) {
		if (widget->get_name() == "Date Container") {
			update_date_header(widget, scene->get_time_componet());
		}
	}
	
}


static void signal_open_note_window(void* receiver_obj, void* emitter_obj) {
	core::Scene* scene = static_cast<core::Scene*>(receiver_obj);
	gtkc::Button* button = static_cast<gtkc::Button*>(emitter_obj);

	core::SigData sig_data;

	sig_data.str = "Note Scene";

	scene->S_request_subwin.emit_signal(emitter_obj, &sig_data);
	//gtkc::Button* button = static_cast<gtkc::Button*>(message->widget);

	//gtkc::Button* button = static_cast<gtkc::Button*>(message->widget);
	//core::Scene* main_scene = static_cast<core::Scene*>(message->receiver_object);
	//std::cout << button->get_text() << "\n";
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

	std::string* weekday_arr = time_componet->get_weekday_arr();


	for (int i=0; i<time_componet->WEEKDAY_COUNT; i++) {
		name = weekday_arr[i];
		button_text = weekday_arr[i];
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

		button->set_tag("Cal Buttons");
		widget_vector.push_back(button);
		scene->GS_cal_button_clicked.listen(button->get_signaler(), "clicked", &signal_open_note_window);

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
	scene->GS_button_clicked.listen(prev_month_button->get_signaler(), "clicked", signal_button_clicked);
	scene->GS_button_clicked.listen(next_month_button->get_signaler(), "clicked", signal_button_clicked);

	top_container->present_widgets();
	info_container->add_widget(top_container);

	info_container->present_widgets();

	return info_container;	
}

static gtkc::GridContainer* create_cal_button_container() {
	gtkc::GridContainer* button_container = nullptr;

	button_container = new gtkc::GridContainer("Button Contanier", 1, 1);
	button_container->set_grid_point(0,1);
	button_container->set_widget_spacing(15);

	button_container->set_valign(GTK_ALIGN_FILL);
	button_container->set_halign(GTK_ALIGN_FILL);

	return button_container;
}

static gtkc::GridContainer* create_date_header_container(const core::Scene* scene) {
	gtkc::GridContainer* date_container = nullptr;	
	gtkc::Label* date_field = nullptr;
	
	
	date_container = new gtkc::GridContainer("Date Container", 1, 1);
	date_container->set_grid_point(0,0);
	date_container->set_vexpand(false);


	date_field = new gtkc::Label("Date Field", "February 2026", 0, 0, 1, 1);
	date_field->set_hexpand(true);
	date_field->set_font_size(8);

	date_container->add_widget(date_field);

	date_container->set_valign(GTK_ALIGN_FILL);
	date_container->set_halign(GTK_ALIGN_START);
	date_container->set_widget_spacing(15);
	date_container->present_widgets();

	return date_container;
}


namespace project {
core::Scene* create_main_scene(core::TimeComponet* time_componet, core::CsvWriter* csv_writer, data::PersistData* persist_data_object) {
	persist_data = persist_data_object;

	const std::string scene_name = "Main Scene";
	int day_count = time_componet->get_day_count();

	gtkc::GridContainer* info_container = nullptr;
	gtkc::GridContainer* date_header_container = nullptr;
	gtkc::GridContainer* cal_button_container = nullptr;

	space::Point scene_dimensions;
	scene_dimensions.x = 600;
	scene_dimensions.y = 600;

	std::vector<gtkc::Widget*> widget_vector;
	std::vector<gtkc::Widget*> cal_button_vector;



	core::Scene* scene = new core::Scene("Main Scene", 32,5);

	scene->set_custom_dimensions(scene_dimensions);

	gtkc::Container& main_container = scene->get_widget_container();
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
	date_header_container = create_date_header_container(scene);
	cal_button_container = create_cal_button_container();
	
	//cal_container->set_scale(3,3);


	
	//add_cal_days(scene, widget_vector, day_count, starting_weekday);
	
	add_weekday_header(time_componet, widget_vector);
	create_cal_buttons(scene, cal_button_vector);


	arrange_cal_buttons(time_componet, cal_button_vector, time_componet->get_menu_month());


	
	//info_container->present_widgets();
	

	cal_button_container->add_widget_vector(widget_vector);
	cal_button_container->add_widget_vector(cal_button_vector);
	cal_button_container->present_widgets();


	cal_container->add_widget(date_header_container);
	cal_container->add_widget(cal_button_container);
	
	cal_container->present_widgets();


	//main_container.add_widget(date_header_container);
	main_container.add_widget(cal_container);
	main_container.add_widget(info_container);

	std::cout << "Main Container Address: " << &main_container << "\n";

	return scene;
}
}
