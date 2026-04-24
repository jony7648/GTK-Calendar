#include "config.h"
#include "core/error.h"
#include "core/util.h"
#include "core/file_util.h"

#include <iostream>
#include <fstream>

namespace project {

void Config::Entry::display_info() {
	std::cout << "\n---Config entry info---\n";

	std::cout << "key: " << key << "\n";
	std::cout << "value: " << value << "\n\n";
}

Config::Config(const std::string& config_path) {
	read_config(config_path);
}
	
void Config::display_info() {
	std::cout << "\n---Config info---\n";

	for (Entry entry : entry_vec) {
		entry.display_info();
	}
}

std::string Config::get_value(const std::string& key) const {
	for (Config::Entry entry : entry_vec) {
		if (entry.key == key) {
			return entry.value;
		}
	}
	
	return "";
}

void Config::parse_config_contents(const std::vector<std::string>& file_contents) {
	char border_char = ':';
	char strip_char = ' ';


	for (std::string line : file_contents) {
		Entry entry;

		size_t line_len = line.length();
		int copy_count = 0;

		size_t i=0;

		for (i=0; line_len; i++) {
			if (line.at(i) != strip_char) {
				break;
			}
		}


		for (i=i; i < line_len; i++) {
			char curr_char = line.at(i);

			if (curr_char == border_char) {
				entry.key = line.substr(0, copy_count);
				i++;
				break;
			}

			copy_count++;	
		}

		for (i=i; i < line_len; i++) {
			char curr_char = line.at(i);

			if (curr_char != strip_char) {
				entry.value = line.substr(i, line_len - i);
				break;
			}
		}

		entry_vec.push_back(entry);
	}
}

core::Error Config::read_config(const std::string& config_path) {
	std::vector<std::string> file_vec;
	std::string current_line;

	std::ifstream file_stream(config_path);



	core::Error error = file_util::get_file_contents(file_stream, file_vec);	

	if (error.exists()) {
		std::cout << "Error getting file contents\n";
		return error;
	}

	parse_config_contents(file_vec);

	return core::Error(core::ErrorType::Clear);
}

core::Error Config::save_config(const Values& values) {
	using Theme = core::CssProvider::Theme;
	std::ofstream stream(values.config_path);

	std::string output_str;

	output_str = "theme: ";



	switch(values.theme) {
		case Theme::Light:
			output_str += "Light";
			break;
		case Theme::Dark:
			output_str += "Dark";
			break;
		default:
			break;
	}

	stream << output_str;
	stream.close();

	return core::Error(core::ErrorType::Clear);


}
}
