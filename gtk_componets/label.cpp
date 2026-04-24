#include <iostream>
#include "label.h"

namespace gtkc {

Label::Label() {
	text = "Empty";
	set_type(Type::Label);
	set_gtk_widget(gtk_label_new(text.c_str()));
	css_provider.load_file("label.css");
	set_font_size(2);
}

Label::Label(const std::string& name, const std::string& text, int grid_x, int grid_y, int width, int height) : Widget() {
	GtkWidget* gtk_widget = gtk_label_new(text.c_str());

	set_name(name);
	set_type(Type::Label);
	css_provider.load_file("label.css");
	set_gtk_widget(gtk_widget);
	set_grid_point(grid_x, grid_y);
	set_scale(width, height);
	set_font_size(2);
	this->text = text;
}

Label::~Label() {
	g_object_unref(get_gtk_widget());
}

void Label::set_font_size(int font_size) {
	if (font_size > MAX_FONT_SIZE) {
		std::cout << "ERROR: font size for is to big! Max Size is: " << MAX_FONT_SIZE << " " << "Widget info displayed below...\n";
		display_info();
	}

	this->font_size = font_size;
	css_provider.load_class("h" + std::to_string(font_size));
}

void Label::set_text(const std::string& new_text) {
	text = new_text;
	gtk_label_set_text(GTK_LABEL(_gtk_widget), text.c_str());
}

const std::string& Label::get_text() {
	return text;
}
}
