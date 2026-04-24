#pragma once

#include <gtk/gtk.h>

#include <iostream>



namespace core {
class CssProvider {

public:
	enum class Theme {
		Light,
		Dark,
		None,
	};

	CssProvider();
	static void change_global_theme(Theme theme);
	static void switch_light_and_dark();
	static void set_styles_dir(const std::string& styles_dir);
	static Theme get_G_theme();
	static Theme get_theme(const std::string& line);
	const std::string& get_css_class();
	void attach_gtk_widget(GtkWidget* gtk_widget);
	void load_file(const std::string& css_path);
	void load_class(const std::string& class_name, Theme theme=_G_theme); 
	void reload_class(Theme theme);
private:
	static Theme _G_theme;
	static std::string _styles_dir;
	std::string _css_path;
	std::string _css_class;	
	std::string _old_theme_prefix;
	GdkDisplay* _default_display = nullptr;
	GtkCssProvider* _css_provider = nullptr;
	GtkWidget* _gtk_widget;

};
}
