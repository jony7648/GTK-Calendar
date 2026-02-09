#include "container.h"

namespace gtkc {



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


ContainerIterator Container::begin() {
	return ContainerIterator(&children_vector, 0);
}

ContainerIterator Container::end() {
	return ContainerIterator(&children_vector, children_vector.size());
}


const std::vector<Widget*>& Container::get_widget_vector() {
	return children_vector;
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

void Container::get_tagged_widgets(std::vector<gtkc::Widget*>& widget_vec, const std::string& tag) {
	std::string container_type = "Grid Container";
	gtkc::Container* container = nullptr;

	for (gtkc::Widget* widget : children_vector) {
		if (widget->is_type(container_type)) {
			container = static_cast<gtkc::Container*>(widget);
			container->get_tagged_widgets(widget_vec, tag);
		}


		if (widget->has_tag(tag)) {
			widget_vec.push_back(widget);
		}
	}
}

void Container::present_widgets() {
	//meathod takes widgets from the children_vector and displays them

	bool child_hexpand = false;
	bool child_vexpand = false;

	GtkWidget* gtk_widget = get_gtk_widget();

	gtk_grid_set_column_spacing(GTK_GRID(gtk_widget), widget_spacing.x);
	gtk_grid_set_row_spacing(GTK_GRID(gtk_widget), widget_spacing.y);
	gtk_widget_set_halign(gtk_widget, halign);
	gtk_widget_set_valign(gtk_widget, valign);



	for (Widget* widget : children_vector) {
		const space::Point& widget_scale = widget->get_scale();
		const space::Point& widget_grid_point = widget->get_grid_point();
		GtkWidget* child_gtk_widget = widget->get_gtk_widget();
		child_hexpand = widget->get_hexpand();
		child_vexpand = widget->get_vexpand();

		//gtk_grid_attach(GTK_GRID(gtk_widget), child_gtk_widget, widget_grid_point.x,widget_grid_point.y,widget_scale.x,widget_scale.y);
		widget->attach(this);
		widget->apply_provider();

		gtk_widget_set_hexpand(child_gtk_widget, child_hexpand);
		gtk_widget_set_vexpand(child_gtk_widget, child_vexpand);

		widget->set_presenting(true);

		//gtk_widget_set_size_request(widget->get_gtk_widget(), widget_scale.x, widget_scale.y);
	}
}

void Container::set_valign(GtkAlign align) {
	valign = align;
	gtk_widget_set_valign(get_gtk_widget(), align);
}

void Container::set_halign(GtkAlign align) {
	halign = align;
	gtk_widget_set_halign(get_gtk_widget(), align);
}

void Container::set_widget_spacing(int x, int y) {
	widget_spacing.x = x;
	widget_spacing.y = y;
}

void Container::set_widget_spacing(int xy) {
	widget_spacing.x = xy;	
	widget_spacing.y = xy;	
}

const space::Point& Container::get_widget_spacing() {
	return widget_spacing;
}
}
