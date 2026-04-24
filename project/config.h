#pragma once
#include <iostream>
#include "core/error.h"
#include "core/css_provider.h"

namespace project {



class Config {
public:
	struct Values {
		std::string config_path = "";
		core::CssProvider::Theme theme = core::CssProvider::Theme::Light;	
	};

	struct Entry {
		std::string key;
		std::string value;

		void display_info();
	};

	std::vector<Entry> entry_vec;

	Config(const std::string& config_path);
	void display_info();
	core::Error read_config(const std::string& config_path);
	core::Error create_config();
	core::Error save_config(const Values& values);
	std::string get_value(const std::string& key) const;
private:
	void parse_config_contents(const std::vector<std::string>& file_contents);
};
}
