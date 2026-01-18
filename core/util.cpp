#include <iostream>
#include <vector>
#include "util.h"

namespace util {
int cycle_thorugh_bounds(int start_value, int cycle_count, int start_bound, int end_bound) {
	//int cycled_position = start_value + cycle_count;
	int bound_len = end_bound - start_bound;

	if (bound_len <= 0) {
		return start_value;
	}

	int cycled_position = 0;
	cycled_position = (start_value + cycle_count) % (bound_len+1);

	return cycled_position;
}

std::vector<std::string>& str_split(std::vector<std::string>& store_vector, const std::string& str, char split_char) {
	//this function splits a string when it hits a specified character
	size_t strlen = str.length();
	int copy_count = 0;
	int sub_start = 0;
	int sub_end = 0;
	std::string sub_str = "";

	for (int i=0; i<strlen; i++) {
		char character = str[i];
		copy_count = i - sub_start;

		if (i == strlen - 1) {
			copy_count++;
		}

		if (character == split_char || i == strlen - 1){
			//std::cout << copy_count << "\n" << sub_start << "\n";
			sub_str = str.substr(sub_start, copy_count);
			store_vector.push_back(sub_str);
			sub_start = i+1;
		}
	}

	return store_vector;
}
}
