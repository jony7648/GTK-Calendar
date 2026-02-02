#pragma once
#include "widget.h"

namespace gtkc {
class Button : public Widget {
private:
	std::string gtk_widget_type = "Button";
	std::string text = "Button";
	std::string image_path = "";
	void(*activate_func)(GtkWidget*, gpointer user_data);

public:
	Button(const std::string& name, const std::string& button_text, int grid_x, int grid_y, int width, int height);
	~Button();

	void set_text(const std::string& text);
	const std::string& get_text();

	void set_image(const std::string& text);
	const std::string& get_image_path();

	void set_activate_func(void(*activate_func)(GtkWidget*, gpointer user_data));
	

	GtkWidget* get_gtk_button();
};
}
