#pragma once
#include "widget.h"
#include "core/signal_handler.h"

namespace gtkc {
class Button : public Widget {
public:
	struct InitProperties {
		BaseInitProperties base_prop;	
		std::string text;
		int font_size = 1;
	};

	Button(const std::string& name, const std::string& button_text, int grid_x, int grid_y, int width, int height);
	Button(const InitProperties& prop);
	~Button();

	void set_font_size(int font_size);

	void set_text(const std::string& text);
	const std::string& get_text();

	void set_image(const std::string& text);
	const std::string& get_image_path();

	void set_activate_func(void(*activate_func)(GtkWidget*, gpointer user_data));

	core::SigHandler<Button> sig_handler;
	

	GtkWidget* get_gtk_button();


private:
	int _font_size = 1;
	std::string gtk_widget_type = "Button";
	std::string text = "Button";
	std::string image_path = "";
	void(*activate_func)(GtkWidget*, gpointer user_data);

};
}
