#pragma once
#include <vector>
#include <gtk/gtk.h>
#include "widget.h"

struct ContainerIntPair {
	ContainerIntPair() = default;

	ContainerIntPair(gtkc::Container* container, int index) {
		this->container = container;
		this->index = index;
	}


	gtkc::Container* container = nullptr;
	int index = 0;
};

namespace gtkc {
class ContainerIterator {
private:
	Container* container = nullptr;
	Widget* iter_widget = nullptr;
	Widget* _ptr = nullptr;

	std::unordered_map<int, ContainerIntPair> container_map;
	int iter_level = 0;
	Container* iter_container = nullptr;

public:
	using PointerType = Widget*;
	using refrence = Widget&;
	bool done = false;

	ContainerIterator(gtkc::Container* container, int index);

	Widget* operator*();


	ContainerIterator& operator++();

	Widget* operator=(Widget* widget_ptr);

	bool operator==(ContainerIterator it);
	
};



class Container : public Widget {
private:
	GtkAlign halign = GTK_ALIGN_CENTER;
	GtkAlign valign = GTK_ALIGN_END;
	space::Point widget_spacing;

protected:
	std::vector<Widget*> children_vector;

public:
	//using ValueType = Widget*;
	//using Iterator = ContainerIterator<Container>;

	~Container();

	void present_widgets();

	bool check_for_same_widget(const std::string& name);
	const std::vector<Widget*>& get_widget_vector();
	void add_widget(Widget* widget);
	void add_widget_arr(Widget* widget_arr[], size_t arr_size);
	void add_widget_vector(std::vector<Widget*>& widget_vector);
	void get_tagged_widgets(std::vector<gtkc::Widget*>&, const std::string& tag);
	Container* get_child_container(const std::string& name);

	ContainerIterator begin();
	ContainerIterator end();

	void set_valign(GtkAlign align);
	void set_halign(GtkAlign align);
	void set_widget_spacing(int x, int y);
	void set_widget_spacing(int xy);
	const space::Point& get_widget_spacing();
};
}
