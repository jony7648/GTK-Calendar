#pragma once
#include <gtk/gtk.h>
#include <iostream>
#include <unordered_map>

namespace gtkc{class Widget;};

namespace core {
template <typename T>
struct Messenger {
	T object;
};

template <typename T1, typename T2>
struct DoubleMessenger {
	T1 object1;
	T2 object2;
};

template <typename T1, typename T2>
struct DataMessenger {
	T1 object = nullptr;
	T2 data = nullptr;
};


}
