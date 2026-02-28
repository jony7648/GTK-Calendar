#include "text_field.h"

namespace gtkc {
TextField::TextField(const std::string& name, int scale_x, int scale_y) {
	_type = "TextField";

	set_name(name);
	set_scale(scale_x, scale_y);
	set_grid_point(0,0);
	set_css_file_name("text_field.css");
	set_gtk_widget(gtk_text_view_new());
	load_css("text_field");
	size_request(300,300);

	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(_gtk_widget), _wrap_mode);

	//gtk_text_view_set_editable(GTK_TEXT_VIEW(_gtk_widget), true);

	_gtk_text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(_gtk_widget));

	//gtk_text_buffer_insert(gtk_text_buffer, &gtk_text_iter, "nugs\n", 5);
	set_text(_empty_text);

}

void TextField::set_wrap_mode(GtkWrapMode wrap_mode) {
	_wrap_mode = wrap_mode;
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(_gtk_widget), wrap_mode);
}

void TextField::set_text(const std::string& text) {
	if (text.empty()) {
		gtk_text_buffer_set_text(_gtk_text_buffer, _empty_text.c_str(), -1);
		return;
	}

	gtk_text_buffer_set_text(_gtk_text_buffer, text.c_str(), -1);
}

std::string TextField::get_text() {
	char* c_str = nullptr;

	gtk_text_buffer_get_bounds(_gtk_text_buffer, &_iter_start, &_iter_end);
	c_str = gtk_text_buffer_get_text(_gtk_text_buffer, &_iter_start, &_iter_end, false);

	return std::string(c_str);
}

}
