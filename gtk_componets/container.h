#pragma once
#include <vector>
#include <gtk/gtk.h>
#include "widget.h"

namespace gtkc {
class Container : public gtkc::Widget {
private:
	std::vector<Widget*> children_vector;
	GtkAlign halign = GTK_ALIGN_CENTER;
	GtkAlign valign = GTK_ALIGN_END;
	space::Point widget_spacing;

public:
	Container();
	~Container();
	void present_widgets();
	bool check_for_same_widget(const std::string& name);
	void add_widget(Widget* widget);
	void add_widget_arr(Widget* widget_arr[], size_t arr_size);
	void add_widget_vector(std::vector<Widget*>& widget_vector);
	void set_valign(GtkAlign align);
	void set_halign(GtkAlign align);
	void set_widget_spacing(int x, int y);
	void set_widget_spacing(int xy);
	const space::Point& get_widget_spacing();
};
}
