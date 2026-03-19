#include <vector>
#include "csv_entry.h"

namespace core::csv {
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
	for (const std::string& header : header_vec) {
		str_vec.push_back(_map[header]);
	}
}
}
