#include "container.h"

namespace gtkc {
Container::Container() {

}

Container::~Container() {
	GtkWidget* gtk_widget = get_gtk_widget();

	for (Widget* widget : children_vector) {
		if (widget != nullptr) {
			delete widget;
		}
	}

	if (gtk_widget == nullptr) {
		g_object_unref(get_gtk_widget());
	}
}

bool Container::check_for_same_widget(const std::string& name) {
	//check if a widget has the same name as another widget 

	for (Widget* widget : children_vector) {
		if (widget->get_name() == name) {
			return true;
		}
	}

	return false;	
}

void Container::add_widget(Widget *widget) {
	//this meathod adds a widget to the children vector
	const std::string& widget_name = widget->get_name();
	const GtkWidget* gtk_widget = widget-> get_gtk_widget();

	if (widget == nullptr) {
		std::cout << "Widget is a nullptr";
	}

	if (gtk_widget == nullptr) {
		std::cout << "gtk_widget is a nullptr!\n";
		return;
	}

	if (widget_name.empty()) {
		std::cout << "Widget does not have a name!\n";
		return;
	}


	if (Container::check_for_same_widget(widget_name)) {
		std::cout << "Widget: " << widget_name << " already exists in the scene!\n";
		return;
	}

	if (widget != nullptr) {
		children_vector.push_back(widget);
	}
}

void Container::add_widget_arr(Widget* widget_arr[], size_t arr_size) {
	//this meathod adds an array of widgets to the children vector
	Widget* widget = nullptr;

	for (int i=0; i<arr_size; i++) {
		widget = widget_arr[i];

		add_widget(widget);
	}	
}

void Container::add_widget_vector(std::vector<Widget*>& widget_vector) {
	//this meathod adds a vector of widgets to the children vector

	for (Widget* widget : widget_vector) {
		add_widget(widget);
	}

}

void Container::present_widgets() {
	//meathod takes widgets from the children_vector and displays them

	GtkWidget* gtk_widget = get_gtk_widget();
	const space::Point& widget_spacing = get_scale();

	gtk_grid_set_column_spacing(GTK_GRID(gtk_widget), widget_spacing.x);
	gtk_grid_set_row_spacing(GTK_GRID(gtk_widget), widget_spacing.y);
	gtk_widget_set_halign(gtk_widget, GTK_ALIGN_CENTER);
	gtk_widget_set_valign(gtk_widget, GTK_ALIGN_END);

	for (Widget* widget : children_vector) {
		const space::Point& widget_scale = widget->get_scale();
		const space::Point& widget_grid_point = widget->get_grid_point();

		gtk_grid_attach(GTK_GRID(gtk_widget), widget->get_gtk_widget(), widget_grid_point.x,widget_grid_point.y,widget_scale.x,widget_scale.y);
		//gtk_widget_set_size_request(widget->get_gtk_widget(), widget_scale.x, widget_scale.y);

	}
}
}
