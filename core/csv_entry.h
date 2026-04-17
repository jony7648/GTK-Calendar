#pragma once
#include <iostream>
#include <unordered_map>

namespace core::csv {

class Entry {

public:
	using CsvMap = std::unordered_map<std::string, std::string>;
	void display_info();
	void add(const std::string& key, const std::string& value);
	std::string get(const std::string& key);
	void remove(const std::string& key);
	bool already_exists = false;

	void to_vec(const std::vector<std::string>& header_vec, std::vector<std::string>& str_vec);

	CsvMap begin();
	CsvMap end();

private:
	CsvMap _map;
};
}
