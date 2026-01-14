#pragma once
#include <iostream>
#include <vector>

namespace util {
	extern std::vector<std::string>& str_split(std::vector<std::string>& store_vector, const std::string& str, char split_char);
}
