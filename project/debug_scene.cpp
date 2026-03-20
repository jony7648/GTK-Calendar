#include <iostream>

#include "gtk_componets/gtk_componets.h"
#include "debug_scene.h"
#include "persist_data.h"
#include "core/core_essentials.h"

template <typename T>
using PropertyMap = std::map<std::string, T>;

int label_count = 0;

PropertyMap<int> int_property_map = {
	{"StartIndex", 3},
	{"EndIndex", 4},
};

PropertyMap<std::string> str_property_map = {
	{"LastNoteText", "N/A"}
};

void create_labels(PropertyMap<int>& property_map, gtkc::Container* container) {
	gtkc::KeyMapLabel* key_map = nullptr;

	for (auto& [key, value] : property_map) {
		key_map = new gtkc::KeyMapLabel();
		key_map->set_grid_point(0, label_count);
		key_map->set_name("KeyMapLabel " + std::to_string(label_count));
		key_map->set_key_text(key);
		key_map->set_value_text(std::to_string(value));

		container->add_widget(key_map);
		label_count++;
	}
}

void create_labels(PropertyMap<std::string>& property_map, gtkc::Container* container) {
	gtkc::KeyMapLabel* key_map = nullptr;

	for (auto& [key, value] : property_map) {
		key_map = new gtkc::KeyMapLabel();
		key_map->set_grid_point(0,label_count);
		key_map->set_name("KeyMapLabel " + std::to_string(label_count));
		key_map->set_key_text(key);
		key_map->set_value_text(value);

		container->add_widget(key_map);		
		label_count++;
	}
}

template <typename T>
void refresh_property_label(const std::string& key, PropertyMap<T>& property_map, gtkc::GridContainer& grid, T value) {
	gtkc::KeyMapLabel* key_map = nullptr;	

	for (gtkc::Widget* widget : grid) {
		if (!widget->is_type(gtkc::Type::KeyMapLabel)) {
			continue;
		}


		key_map = static_cast<gtkc::KeyMapLabel*>(widget);
		gtkc::Label& key_label = key_map->get_key_label();
		gtkc::Label& value_label = key_map->get_value_label();

		if (key_label.get_text() == key) {
			value_label.set_text(std::to_string(value));
			key_map->reattach();
		}
	}
}


template <>
void refresh_property_label<std::string>(const std::string& key, PropertyMap<std::string>& property_map, gtkc::GridContainer& grid, std::string value) {
	gtkc::KeyMapLabel* key_map = nullptr;	

	for (gtkc::Widget* widget : grid) {
		if (!widget->is_type(gtkc::Type::KeyMapLabel)) {
			continue;
		}


		key_map = static_cast<gtkc::KeyMapLabel*>(widget);
		gtkc::Label& key_label = key_map->get_key_label();
		gtkc::Label& value_label = key_map->get_value_label();

		if (key_label.get_text() == key) {
			value_label.set_text(value);
			key_map->reattach();
		}
	}
}


static void signal_note_incremented(core::EmitData<data::NoteContainer> emit_data) {
	//auto sig_data = static_cast<data::NoteContainer::SigIncremented*>(sig_addr);

	//e∫data::Note* end_note = sig_data->end_note;
	

	core::Scene* scene = static_cast<core::Scene*>(emit_data.receiver);
	auto* sig_data = static_cast<data::NoteContainer::SigIncremented*>(emit_data.sig_data);
	gtkc::GridContainer& main_container = scene->get_widget_container();


	gtkc::GridContainer& grid = scene->get_widget_container();

	str_property_map["LastNoteText"] = sig_data->end_note->text;
	str_property_map["StartIndex"] = 7;
	str_property_map["EndIndex"] = sig_data->end_index;
	refresh_property_label("LastNoteText", str_property_map, grid, sig_data->end_note->text);
	refresh_property_label("StartIndex", int_property_map, grid, sig_data->start_index);
	refresh_property_label("EndIndex", int_property_map, grid, sig_data->end_index);
}

namespace project {
	core::Scene* create_debug_scene(ActivateSignal* activate_signal) {
		data::PersistData* persist_data = activate_signal->persist_data;

		space::Point dimensions;

		dimensions.x = 300;
		dimensions.y = 300;

		core::Scene* debug_scene = new core::Scene("DebugScene", 1, 1); 
		debug_scene->set_custom_dimensions(dimensions);

		gtkc::GridContainer& main_container = debug_scene->get_widget_container();
		data::NoteContainer& note_container = persist_data->get_note_container();



		note_container.sig_handler.add_emit_func(data::NoteContainer::S_INCREMENTED, &signal_note_incremented, debug_scene);


		gtkc::GridContainer* debug_container = new gtkc::GridContainer("Debug Container", 1, 1);
		debug_container->set_hexpand(true);
		debug_container->set_vexpand(true);
		debug_container->set_valign(GTK_ALIGN_START);
		debug_container->set_halign(GTK_ALIGN_START);


		create_labels(int_property_map, debug_container);
		create_labels(str_property_map, debug_container);

		debug_container->present_widgets();

		main_container.add_widget(debug_container);
		main_container.set_halign(GTK_ALIGN_FILL);
		main_container.set_valign(GTK_ALIGN_FILL);
		main_container.present_widgets();

		return debug_scene;

	}
}
