#pragma once
#include <gtk/gtk.h>
#include "container.h"

namespace gtkc {
class GridContainer : public Container {

private:
	space::Point grid_size;
public:
	GridContainer();
	GridContainer(const std::string& name, int size_x, int size_y);

	const space::Point& get_grid_size();

};
}
