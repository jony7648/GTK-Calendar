#include <iostream>
#include <gtk/gtk.h>
#include "gtk_componets.h"
#include "scenes.h"
#include "core/window.h"
#include "core/util.h"

static void signal_open_note_window(void* receiver_obj, void* emitter_obj) {
	core::Scene* scene = static_cast<core::Scene*>(receiver_obj);
	gtkc::ButtonWidget* button = static_cast<gtkc::ButtonWidget*>(button);
	std::cout << scene << "\n";
	std::cout << button << "\n";

	scene->S_request_subwin.emit_signal(scene);


	 
	//gtkc::ButtonWidget* button = static_cast<gtkc::ButtonWidget*>(message->widget);

	//gtkc::ButtonWidget* button = static_cast<gtkc::ButtonWidget*>(message->widget);
	//core::Scene* main_scene = static_cast<core::Scene*>(message->receiver_object);
	//std::cout << button->get_text() << "\n";
}

static void add_weekday_header(const core::TimeComponet* time_componet, std::vector<gtkc::Widget*>& widget_vector) {
	gtkc::Widget* widget = nullptr;
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
		name = time_componet->weekday_arr[i];
		button_text = time_componet->weekday_arr[i];
		grid_x = i;
		grid_y = 0;
		widget = new gtkc::Label(name, button_text, grid_x, grid_y, 1,1);
		widget_vector.push_back(widget);
	}
}

static void create_cal_buttons(core::Scene* scene, std::vector<gtkc::Widget*>& widget_vector) {
	const int MAX_DAYS_PER_MTH = 31;

	gtkc::Widget* widget = nullptr;
	std::string name = "";
	std::string text = "";


	for (int i=0; i<MAX_DAYS_PER_MTH; i++) {
		name = std::to_string(i+1);
		text = name;
		widget = new gtkc::ButtonWidget(name, text, 0, 3, 1, 1);
		widget_vector.push_back(widget);
		scene->GS_cal_button_clicked.listen(widget->get_signaler(), "clicked", &signal_open_note_window);
		//widget->listener.connect(&scene->listener, scene->signal_request_subwin, "clicked");

	}
}

static void arrange_cal_buttons(core::TimeComponet* time_componet, std::vector<gtkc::Widget*>& widget_vector, int month) {
	//create the system for other months
	gtkc::Widget* widget = nullptr;
	int grid_x = 0;
	int grid_y = 0;

	//std::cout << "size: " << widget_vector.size();
	int start_weekday = time_componet->get_starting_weekday(11, 2032);
	int pos_index = 0;
	
	for (int i=0; i<widget_vector.size(); i++) {
		widget = widget_vector.at(i);
		pos_index = i + start_weekday;
		grid_x = (pos_index) % 7;
		grid_y = (pos_index / 7) + 1;

		widget->set_grid_point(grid_x, grid_y);
	}


}


/*
static void add_cal_days(core::Scene* scene, std::vector<gtkc::Widget*>& widget_vector, int day_count, int starting_weekday) {
	std::string days_of_week[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

	gtkc::Widget* widget = nullptr;
	std::string name = "";
	std::string button_text = "";
	int row = 0;
	int grid_x = 0;
	int grid_y = 0;
	int btn_index;
	int row_count = 1;
	int column_count = 1;
	int day_pos = 0;

	//create weekday header
	for (int i=0; i<sizeof(days_of_week)/sizeof(std::string); i++) {
		name = days_of_week[i];
		button_text = days_of_week[i];
		grid_x = i;
		grid_y = 0;
		widget = new gtkc::Label(name, button_text, grid_x, grid_y, 1,1);
		widget_vector.push_back(widget);
		
	}

	
	//create days (e.g 1-31)
	for (int i=0; i<day_count; i++) {
		day_pos = i + starting_weekday;

		grid_x = day_pos % 7;
		grid_y = (day_pos / 7) + 1;
		btn_index = i + 1;

		name = "Button " + std::to_string(btn_index);
		button_text = std::to_string(btn_index);
		widget = new gtkc::ButtonWidget(name, button_text, grid_x, grid_y, row_count, column_count);
		widget_vector.push_back(widget);
		widget->listener.connect(&scene->listener, signal_open_note_window, "clicked");
		//widget->signal_connect("clicked", &TEST_FOR_SIGNAL);
		grid_x++;
	}
}
*/



namespace calender {
core::Scene* create_main_scene(core::Window* window, core::TimeComponet* time_componet) {
	const std::string scene_name = "Main Scene";
	

	core::Scene* scene = new core::Scene("Main Scene", 32,5);
	gtkc::Container* main_container = scene->container;
	main_container->set_valign(GTK_ALIGN_FILL);
	main_container->set_halign(GTK_ALIGN_FILL);
	main_container->set_widget_spacing(15);

	space::Point scene_dimensions;
	scene_dimensions.x = 600;
	scene_dimensions.y = 600;

	scene->set_custom_dimensions(scene_dimensions);
	

	//widget vectors
	std::vector<gtkc::Widget*> widget_vector;
	std::vector<gtkc::Widget*> cal_button_vector;


	int day_count = time_componet->get_day_count();

	
	//add_cal_days(scene, widget_vector, day_count, starting_weekday);
	
	add_weekday_header(time_componet, widget_vector);
	create_cal_buttons(scene, cal_button_vector);
	arrange_cal_buttons(time_componet, cal_button_vector, 1);

	scene->container->add_widget_vector(widget_vector);
	scene->container->add_widget_vector(cal_button_vector);


	return scene;
}

core::Scene* create_note_scene() {
	const std::string scene_name = "Note Scene";
	space::Point widget_spacing;
	widget_spacing.x = 32;
	widget_spacing.y = 30;

	core::Scene* scene = new core::Scene(scene_name, widget_spacing.x, widget_spacing.y);
	scene->set_resizability(false);

	return scene;	
}
}
