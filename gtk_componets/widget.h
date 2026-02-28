#pragma once
#include <iostream>
#include <vector>
#include <gtk/gtk.h>
#include "core/constants.h"
#include "core/space.h"
#include "core/messenger.h"
#include "core/signal_handler.h"

namespace gtkc {
class Container;

class Widget {

public:
	void load_css();
	void load_css(const std::string& class_name);
	void apply_provider();
	Widget();
	~Widget();

	void display_info();
	void attach(Container* container);


	void reattach();

	void show();
	void hide();
	bool get_visibility();

	void set_name(const std::string& name);
	const std::string& get_name();

	void set_tag(const std::string& tag);
	const std::string& get_tag();
	bool has_tag(const std::string&);


	void set_gtk_widget(GtkWidget* gtk_widget);
	GtkWidget* get_gtk_widget();

	void set_scale(int x, int y);
	const space::Point& get_scale();

	void size_request(int x, int y);

	void set_grid_point(int x, int y);
	const space::Point& get_grid_point();

	bool is_type(const std::string& type);
	const std::string& get_type();


	void set_hexpand(bool hexpand);
	void set_vexpand(bool vexpand);
	bool get_hexpand();
	bool get_vexpand();
	void set_presenting(bool state);


	void set_sig_data(int data);
	int get_sig_data();

protected:
	std::string _type = "UNDEFINED TYPE";
	GtkWidget* _gtk_widget;

	void set_css_file_name(const std::string& css_path);


private:
	std::string _name = "";
	std::string _tag = "";
	std::string _css_class = "N/A";
	int _grid_pos = 0;
	space::Rect _transform;
	space::Point _grid_point;
	space::Point _scale;
	bool _hexpand = true;
	bool _vexpand = true;
	int _sig_data = 0;
	bool _presenting = false;
	bool is_visible = false;

	GtkWidget* _holder_gtk_widget = nullptr;

	std::string _css_path = "";
	GtkCssProvider* _css_provider = nullptr;
	GdkDisplay* _default_display = nullptr;

	core::SigHandler sig_handler;
	
//	std::vector<core::Messenger<Widget*>*> messenger_vector; //vector holds the addresses of used messenger_datas, so they can be deleted when widget is freed


};
}
