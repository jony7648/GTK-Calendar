#pragma once
#include <iostream>

namespace project {
class Config {
public:
	struct Entry {
		std::string key;
		std::string value;

		void display_info();
	};

	std::vector<Entry> entry_vec;

	Config(const std::string& config_path);
	void display_info();
	void read_config(const std::string& config_path);
	std::string get_value(const std::string& key) const;
private:
	void parse_config_contents(const std::vector<std::string>& file_contents);
};
}
