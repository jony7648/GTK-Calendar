#include "key_map_label.h"

namespace gtkc {
KeyMapLabel::KeyMapLabel() : _key_label(), _value_label(){
	
	_key_label.set_name(KEY_LABEL_NAME);
	_value_label.set_name(VALUE_LABEL_NAME);

	_key_label.set_grid_point(0,0);
	_value_label.set_grid_point(1,0);

	_key_label.set_font_size(4);
	_value_label.set_font_size(4);

	set_gtk_widget(gtk_grid_new());
	apply_provider();

	set_widget_spacing(20);
	set_halign(GTK_ALIGN_FILL);
	set_hexpand(true);



	add_widget(&_key_label);
	add_widget(&_value_label);

	
	present_widgets();
}

const std::string& KeyMapLabel::get_key_text() {
	return _key_label.get_text();
}

const std::string& KeyMapLabel::get_value_text() {
	return _value_label.get_text();
}

const Label& KeyMapLabel::get_key_label() {
	return _key_label;
}

const Label& KeyMapLabel::get_value_label() {
	return _value_label;
}

void KeyMapLabel::set_key_text(const std::string& key_text) {
	_key_text = key_text;
	_key_label.set_text(_key_text);
}

void KeyMapLabel::set_value_text(const std::string& value_text) {
	_value_text = value_text;
	_value_label.set_text(_value_text);
}
}
