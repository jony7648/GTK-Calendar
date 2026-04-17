#include <vector>
#include "csv_entry.h"

namespace core::csv {

void Entry::display_info() {
	std::cout << "\n---Entry info---\n";

	for (auto& [key, value] : _map) {
		std::cout << key << ": " << value << "\n";
	}
}

void Entry::add(const std::string& key, const std::string& value) {
	_map[key] = value;
}

std::string Entry::get(const std::string& key) {
	if (_map.contains(key)) {
		return _map[key];
	}

	return "";
}

void Entry::remove(const std::string& key) {
	_map.erase(key);
}


void Entry::to_vec(const std::vector<std::string>& header_vec, std::vector<std::string>& str_vec) {
	std::string line_str;
	for (const std::string& header : header_vec) {
		line_str = "";

		for (char iter_char : _map[header]) {
			if (iter_char == '\n') {
				line_str += "\\n";
				continue;
			}

			line_str += iter_char;
		}
		
		str_vec.push_back(line_str);
		
	}
}
}
