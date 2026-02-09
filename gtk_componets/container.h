#pragma once
#include <vector>
#include <gtk/gtk.h>
#include "widget.h"

namespace gtkc {
class Container;

class ContainerIterator {
private:
	//pointer m_data*
	Container* container;
	std::vector<gtkc::Widget*>* widget_vector;
	int _index;
	//int _end_index = widget_vector.size() + 1;
	Widget* _ptr = nullptr;




public:
	using PointerType = Widget*;
	using refrence = Widget&;
	//using ValueType = typename Container::ValueType;
	//const std::vector<Widget*>& widget_vector = container->get_widget_vector();

	ContainerIterator(std::vector<Widget*>* widget_vector, int index) {
		this->widget_vector = widget_vector;
		_index = index;
	}

	Widget* operator*() {
		return widget_vector->at(_index);
	}


	ContainerIterator& operator++() {
		_index++;	

		
		return *this;
	}

	Widget* operator=(Widget* widget_ptr) {
		return widget_vector->at(_index);
	}

	bool operator==(ContainerIterator it) {
		std::cout << "Index " << _index << "\n";
		if (_index == it._index) {
			return true;
		}

		return false;
	}
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

	ContainerIterator begin();
	ContainerIterator end();

	void set_valign(GtkAlign align);
	void set_halign(GtkAlign align);
	void set_widget_spacing(int x, int y);
	void set_widget_spacing(int xy);
	const space::Point& get_widget_spacing();
};
}
