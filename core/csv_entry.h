#pragma once
#include <iostream>
#include <unordered_map>

namespace core::csv {

class Entry {
private:
	std::unordered_map<std::string, std::string> _map;

public:
	void add(const std::string& key, const std::string& value);
	std::string get(const std::string& key);
	void remove(const std::string& key);

	void to_vec(const std::vector<std::string>& header_vec, std::vector<std::string>& str_vec);
};
}

