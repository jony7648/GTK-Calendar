#pragma once
#include <iostream>
#include <vector>

namespace util {
	extern std::vector<std::string>& str_split(std::vector<std::string>& store_vector, const std::string& str, char split_char);
	extern int cycle_through_bounds(int start_value, int cycle_count, int start_bound, int end_bound);
}
