#include <iostream>
#include <gtk/gtk.h>
#include "gtk_componets.h"
#include "scenes.h"
#include "core/window.h"
#include "core/util.h"

static void signal_open_note_window(void* receiver_obj, void* emitter_obj) {
	core::Scene* scene = static_cast<core::Scene*>(receiver_obj);
	gtkc::Button* button = static_cast<gtkc::Button*>(button);
	std::cout << scene << "\n";
	std::cout << button << "\n";

	scene->S_request_subwin.emit_signal(scene);


	 
	//gtkc::Button* button = static_cast<gtkc::Button*>(message->widget);

	//gtkc::Button* button = static_cast<gtkc::Button*>(message->widget);
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
		widget = new gtkc::Button(name, text, 0, 3, 1, 1);
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
	int start_weekday = time_componet->get_starting_weekday();
	int pos_index = 0;
	
	for (int i=0; i<widget_vector.size(); i++) {
		widget = widget_vector.at(i);
		pos_index = i + start_weekday;
		grid_x = (pos_index) % 7;
		grid_y = (pos_index / 7) + 1;

		widget->set_grid_point(grid_x, grid_y);
	}
}


static gtkc::GridContainer* create_info_container() {
	gtkc::GridContainer* info_container = new gtkc::GridContainer("Info Contanier", 1,1);
	info_container->set_grid_point(0,0);
	info_container->set_scale(1,1);

	gtkc::Button* left_button = new gtkc::Button("Previous Month", "Left", 0, 0, 32, 32);

	info_container->add_widget(left_button);

	return info_container;	
}


namespace calender {
core::Scene* create_main_scene(core::Window* window, core::TimeComponet* time_componet) {
	const std::string scene_name = "Main Scene";
	int day_count = time_componet->get_day_count();

	gtkc::GridContainer* info_container = nullptr;
	space::Point scene_dimensions;
	scene_dimensions.x = 600;
	scene_dimensions.y = 600;

	std::vector<gtkc::Widget*> widget_vector;
	std::vector<gtkc::Widget*> cal_button_vector;



	core::Scene* scene = new core::Scene("Main Scene", 32,5);
	scene->set_custom_dimensions(scene_dimensions);

	gtkc::Container* main_container = scene->container;
	main_container->set_valign(GTK_ALIGN_FILL);
	main_container->set_halign(GTK_ALIGN_FILL);
	main_container->set_widget_spacing(15);

	gtkc::GridContainer* cal_container = new gtkc::GridContainer("Cal Container", 32, 32);
	cal_container->set_valign(GTK_ALIGN_FILL);
	cal_container->set_halign(GTK_ALIGN_FILL);
	cal_container->set_grid_point(1,0);
	cal_container->set_widget_spacing(15);

	info_container = create_info_container();
	//cal_container->set_scale(3,3);


	
	//add_cal_days(scene, widget_vector, day_count, starting_weekday);
	
	add_weekday_header(time_componet, widget_vector);
	create_cal_buttons(scene, cal_button_vector);
	arrange_cal_buttons(time_componet, cal_button_vector, 1);

	cal_container->add_widget_vector(widget_vector);
	cal_container->add_widget_vector(cal_button_vector);
	cal_container->present_widgets();

	info_container->present_widgets();

	main_container->add_widget(cal_container);
	main_container->add_widget(info_container);


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
