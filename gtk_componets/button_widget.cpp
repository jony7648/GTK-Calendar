#include "button_widget.h"

namespace gtkc {
ButtonWidget::ButtonWidget(const std::string& name, const std::string& text, int grid_x, int grid_y, int width, int height) {
	if (text.empty()) {
		set_gtk_widget(gtk_button_new());
	}
	else {
		set_gtk_widget(gtk_button_new_with_label(text.c_str()));
	}

	set_name(name);
	set_grid_point(grid_x, grid_y);
	set_scale(width, height);
	this->text = text;
}

ButtonWidget::~ButtonWidget() {
	//free image and self
	
	g_object_unref(get_gtk_widget());
}

void ButtonWidget::set_text(const std::string& text) {
	//sets the text that will be displayed inside 
	//the button
	this->text = text;

	GtkWidget* gtk_button = get_gtk_widget();

	gtk_button_set_label(GTK_BUTTON(gtk_button), text.c_str());
}

GtkWidget* ButtonWidget::get_gtk_button() {
	return get_gtk_widget();
}

const std::string& ButtonWidget::get_text() {
	return text;
}
}
