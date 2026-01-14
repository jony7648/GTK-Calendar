#pragma once
#include <iostream>
#include "widget.h"

namespace gtkc {
class Label : public Widget {
private:
	std::string text = "";


public:
	Label(const std::string& name, const std::string& text, int grid_x, int grid_y, int column_count, int row_count);
	~Label();
	void set_text();
};
}
