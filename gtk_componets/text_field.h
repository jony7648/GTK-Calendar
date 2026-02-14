#pragma once
#include <iostream>
#include "widget.h"

namespace gtkc {
class TextField : public Widget {
private:
	std::string _emtpy_str = "Please enter some Text";
	std::string _text = "";
	GtkTextBuffer* _gtk_text_buffer = nullptr;
	GtkTextIter _iter_start, _iter_end;
	GtkWrapMode _wrap_mode = GTK_WRAP_WORD_CHAR;

public:
	TextField(const std::string& name, int scale_x, int scale_y);
	void set_wrap_mode(GtkWrapMode wrap_mode);
	std::string get_text();
};
}

