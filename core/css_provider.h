#pragma once

#include <gtk/gtk.h>

#include <iostream>



namespace core {
class CSSProvider {
public:
	enum class Theme {
		Light,
		Dark,
		None,
	};

	void load_file(const std::string& css_path, GtkWidget* gtk_widget);
	void load_class(const std::string& class_name, Theme theme); 

private:
	std::string _css_path;
	std::string _css_class;	
	GdkDisplay* _default_display = nullptr;
	GtkCssProvider* _css_provider = nullptr;
	GtkWidget* _gtk_widget;
};
}
