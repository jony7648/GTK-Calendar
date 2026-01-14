#pragma once
#include <vector>
#include <gtk/gtk.h>
#include "widget.h"

namespace gtkc {
class Container : public gtkc::Widget {
private:
	std::vector<Widget*> children_vector;
	int halign = GTK_ALIGN_CENTER;
	int valign = GTK_ALIGN_END;

public:
	Container();
	~Container();
	void present_widgets();
	bool check_for_same_widget(const std::string& name);
	void add_widget(Widget* widget);
	void add_widget_arr(Widget* widget_arr[], size_t arr_size);
	void add_widget_vector(std::vector<Widget*>& widget_vector);
};
}
