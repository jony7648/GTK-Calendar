#pragma once
#include "label.h"
#include "grid_container.h"

namespace gtkc {
class KeyMapLabel : public GridContainer {
private:
	std::string _key_text = "";
	std::string _value_text = "";
	
	gtkc::Label _key_label;
	gtkc::Label _value_label;

public:
	const std::string KEY_LABEL_NAME = "KeyLabel";
	const std::string VALUE_LABEL_NAME = "ValueLabel";

	KeyMapLabel();
	Label& get_key_label();
	Label& get_value_label();
	const std::string& get_key_text();
	void set_key_text(const std::string& new_text);
	const std::string& get_value_text();
	void set_value_text(const std::string& new_text);
	
};
}
