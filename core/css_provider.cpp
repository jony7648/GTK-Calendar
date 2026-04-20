#include "css_provider.h"

#include <iostream>
#include <gtk/gtk.h>

namespace core {

void CSSProvider::load_file(const std::string& css_path, GtkWidget* gtk_widget) {
	if (_css_path.empty()) {
		std::cout << "ERROR: css provider was created with an empty css path";
		return;
	}

	gtk_css_provider_load_from_path(_css_provider, _css_path.c_str());
	gtk_style_context_add_provider_for_display(_default_display, GTK_STYLE_PROVIDER(_css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	_gtk_widget = gtk_widget;
}

void CSSProvider::load_class(const std::string& class_name, Theme theme) {
	std::string theme_str = "";
	
	if (!_gtk_widget) {
		std::cout << "ERROR: can't css style gtk_widget is a nullptr\n";
		return;
	}

	if (!_css_provider) {
		std::cout << "Error: _css_provider is a nullptr!\n";
		return;
	}

	switch (theme) {
		case Theme::Light:
			theme_str = "Light";
			break;
		case Theme::Dark:
			theme_str = "Dark";
			break;
		case Theme::None:
			theme_str = "";
			break;
	}


	gtk_widget_remove_css_class(_gtk_widget, _css_class.c_str());

	_css_class = theme_str + class_name;
	gtk_widget_add_css_class(_gtk_widget, _css_class.c_str());
}

}


