#pragma once
#include <iostream>
#include <vector>

namespace util {
	void* dynamic_data_init(size_t data_size);
	void* dynamic_data_assign(void* dynamic_data, size_t new_struct_size);
	void dynamic_data_free(void* dynamic_data);
	void str_split(const std::string& src_str, char split_char, std::vector<std::string>& output_vec);
	int cycle_through_bounds(int start_value, int cycle_count, int start_bound, int end_bound);
	void str_strip(std::string& str);
}
