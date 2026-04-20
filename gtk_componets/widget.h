#pragma once
#include <iostream>
#include <vector>
#include <gtk/gtk.h>
#include "core/constants.h"
#include "core/space.h"
#include "core/messenger.h"
#include "types.h"
//#include "core/signal_handler.h"

namespace core {
	template <typename ObjType>
	class SigHandler;
}

namespace gtkc {
class Container;

class Widget {
public:
	enum class Theme {
		Light,
		Dark,
		None,
	};

	struct CommonInitProperties {
		std::string tag;
		space::Point scale;
		space::Point size_request;
		bool hexpand = false;
		bool vexpand = false;
	};

	struct BaseInitProperties {
		const CommonInitProperties& shared_properties;
		std::string name;
		space::Point grid_point;
		
		int sig_data = 0;
	};

	//void load_css();
	static Theme get_theme(const std::string& line);
	void load_css(const std::string& class_name = "Default", Theme theme = Theme::None);
	void apply_provider();
	Widget();
	Widget(const BaseInitProperties& prop);
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
	void set_scale(const space::Point& scale);
	const space::Point& get_scale();

	void size_request(int x, int y);
	void size_request(const space::Point& size);

	void set_grid_point(int x, int y);
	void set_grid_point(const space::Point& point);
	const space::Point& get_grid_point();

	bool is_type(Type type);
	Type get_type();


	void set_hexpand(bool hexpand);
	void set_vexpand(bool vexpand);
	bool get_hexpand();
	bool get_vexpand();
	void set_presenting(bool state);


	void set_sig_data(int data);
	int get_sig_data();

protected:
	GtkWidget* _gtk_widget;

	void set_css_file_name(const std::string& css_path);
	const std::string& get_css_class();
	void set_type(Type type);


private:
	std::string _name = "";
	Type _type = Type::Widget;
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

//	std::vector<core::Messenger<Widget*>*> messenger_vector; //vector holds the addresses of used messenger_datas, so they can be deleted when widget is freed


};
}
