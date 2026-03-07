#include <iostream>
#include "gtk_componets/gtk_componets.h"
#include "debug_scene.h"
#include "persist_data.h"

template <typename T>
using PropertyMap = std::unordered_map<std::string, T>;

int label_count = 0;

PropertyMap<int> int_property_map = {
	{"StartIndex", 3},
	{"EndIndex", 4},
};

PropertyMap<std::string> str_property_map = {
	{"LastNoteText", "N/A"}
};

void create_labels(PropertyMap<int>& property_map, gtkc::Container* container) {
	gtkc::KeyMapLabel* key_label = nullptr;

	for (auto& [key, value] : property_map) {
		key_label = new gtkc::KeyMapLabel();
		key_label->set_name("KeyMapLabel " + std::to_string(label_count));
		key_label->set_key_text(key);
		key_label->set_value_text(std::to_string(value));


		container->add_widget(key_label);
	
		
		
		label_count++;
		break;
	}
}

void create_labels(PropertyMap<std::string>& property_map, gtkc::Container* container) {
	for (auto& [key, value] : property_map) {
		std::cout << "yes tastic\n";
		gtkc::Label* name_label = new gtkc::Label("LabelKey " + std::to_string(label_count), key, 0, label_count, 1, 1);
		gtkc::Label* value_label = new gtkc::Label("LabelValue " + std::to_string(label_count), value, 1, label_count, 1, 1);

		container->add_widget(name_label);
		container->add_widget(value_label);
		
		
		label_count++;
	}
}

template <typename T>
void refresh_property_label(const std::string& key, T& property_map, gtkc::GridContainer& grid) {
	gtkc::Label* label = nullptr;	

	for (gtkc::Widget* widget : grid) {
		if (widget->get_type() != "Label") {
			continue;
		}

		label = static_cast<gtkc::Label*>(widget);

		if (label->get_text() == key) {
			label->set_text(property_map[key]);
			label->reattach();
		}
	}
}


static void signal_note_incremented(void* receiver_obj, void* emitter_obj, void* sig_addr) {
	//auto sig_data = static_cast<data::NoteContainer::SigIncremented*>(sig_addr);

	//e∫data::Note* end_note = sig_data->end_note;

	core::Scene* scene = static_cast<core::Scene*>(receiver_obj);
	auto* sig_data = static_cast<data::NoteContainer::SigIncremented*>(sig_addr);
	gtkc::GridContainer& main_container = scene->get_widget_container();
	

	scene->get_name();


	
	std::cout << "Sig Address: " << sig_data << "\n"	;
	str_property_map["LastNoteText"] = sig_data->end_note->text;
	refresh_property_label("LastNoteText", str_property_map, scene->get_widget_container());

	//std::cout << "SONIC MAN THIS IS WORKIGN AHH MAN: " << /*<< end_note->text <<*/ "\n";

	//refresh_property_labels(property_map, );
	//implement this so you can change the sig data
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
