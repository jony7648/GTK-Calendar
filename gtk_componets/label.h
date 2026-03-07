#pragma once
#include <iostream>
#include "widget.h"

namespace gtkc {
class Label : public Widget {
private:
	std::string text = "";
	const int MAX_FONT_SIZE = 8;	
	int font_size = 1;


public:
	Label();
	Label(const std::string& name, const std::string& text, int grid_x, int grid_y, int column_count, int row_count);
	~Label();
	void set_font_size(int font_size);
	void set_text(const std::string& new_text);
	const std::string& get_text();
};
}
