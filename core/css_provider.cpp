#include "css_provider.h"
#include "util.h"

#include <iostream>
#include <gtk/gtk.h>

std::vector<core::CssProvider*> provider_ptr_vec;

namespace core {

//initialise the global theme
CssProvider::Theme CssProvider::_G_theme = CssProvider::Theme::Light;
std::string CssProvider::_styles_dir = "";

CssProvider::CssProvider() {
	provider_ptr_vec.push_back(this);	
	_css_provider = gtk_css_provider_new();
	_default_display = gdk_display_get_default();
}

void CssProvider::set_styles_dir(const std::string& styles_dir) {
	_styles_dir = styles_dir;
}


CssProvider::Theme CssProvider::get_G_theme() {
	return _G_theme;
}

CssProvider::Theme CssProvider::get_theme(const std::string& line) {
	std::string line_cpy = util::copy_str_strip(line);

	line_cpy = util::str_to_lower(line_cpy);

	if (line_cpy == "dark") {
		return Theme::Dark;
	}

	std::cout << "Lowered str: " <<line_cpy << "\n";

	return Theme::Light;
}

void CssProvider::attach_gtk_widget(GtkWidget* gtk_widget) {
	_gtk_widget = gtk_widget;
}

void CssProvider::load_file(const std::string& css_path) {
	if (css_path.empty()) {
		std::cout << "ERROR: css provider was created with an empty css path\n";
		return;
	}

	_css_path = _styles_dir + "/" + css_path;

	gtk_css_provider_load_from_path(_css_provider, _css_path.c_str());
	gtk_style_context_add_provider_for_display(_default_display, GTK_STYLE_PROVIDER(_css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

const std::string& CssProvider::get_css_class() {
	return _css_class;	
}

void CssProvider::load_class(const std::string& class_name, Theme theme) {
	std::string theme_prefix = "";
	std::string theme_str = "";
	std::string old_theme_str = "";
	
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
			theme_prefix = "Light";
			break;
		case Theme::Dark:
			theme_prefix = "Dark";
			break;
		case Theme::None:
			theme_prefix = "";
			break;
	}

	theme_str = theme_prefix + class_name;
	old_theme_str = _old_theme_prefix + _css_class;

	if (!_css_class.empty()) {
		gtk_widget_remove_css_class(_gtk_widget, old_theme_str.c_str());
	}

	gtk_widget_add_css_class(_gtk_widget, theme_str.c_str());
	_css_class = class_name;
	_old_theme_prefix = theme_prefix;
}

void CssProvider::reload_class(Theme theme) {
	load_class(_css_class, theme);
}

void CssProvider::switch_light_and_dark() {
	switch (_G_theme) {
		case Theme::Light:
			std::cout << "Dark Theme\n";
			change_global_theme(Theme::Dark);
			break;
		case Theme::Dark:
			std::cout << "Light Theme\n";
			change_global_theme(Theme::Light);
			break;
		case Theme::None:
			break;
	}
}

void CssProvider::change_global_theme(Theme theme) {
	_G_theme = theme;

	for (CssProvider* p_provider : provider_ptr_vec) {
		if (!p_provider) {
			std::cout << "Cannot change all provider themes as one is a nullptr\n";
			continue;
		}

		p_provider->reload_class(theme);
	}
}
}

