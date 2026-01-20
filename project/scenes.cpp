#include <iostream>
#include <gtk/gtk.h>
#include "gtk_componets.h"
#include "scenes.h"
#include "core/window.h"
#include "core/util.h"

static void signal_open_note_window(gtkc::Message* message) {
	std::cout << "Nugget worked yes\n";	
}


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
		widget->listener.connect(scene->listener, signal_open_note_window, "clicked");
		//widget->signal_connect("clicked", &TEST_FOR_SIGNAL);
		grid_x++;
	}
}



namespace calender {
core::Scene* create_main_scene(core::Window* window, core::TimeComponet* time_componet) {
	const std::string scene_name = "Main Scene";
	

	core::Scene* scene = new core::Scene("Main Scene", 32,5);
	gtkc::Container* main_container = scene->container;
	main_container->set_valign(GTK_ALIGN_FILL);
	main_container->set_halign(GTK_ALIGN_FILL);
	main_container->set_widget_spacing(15);
	

	std::vector<gtkc::Widget*> widget_vector;

	int day_count = time_componet->get_day_count();

	int starting_weekday = time_componet->get_starting_weekday();
	
	add_cal_days(scene, widget_vector, day_count, starting_weekday);

	scene->container->add_widget_vector(widget_vector);


	return scene;
}

core::Scene* create_note_scene() {
	const std::string scene_name = "Note Scene";
	space::Point widget_spacing;
	widget_spacing.x = 32;
	widget_spacing.y = 30;

	core::Scene* scene = new core::Scene(scene_name, widget_spacing.x, widget_spacing.y);

	return scene;	
}
}
