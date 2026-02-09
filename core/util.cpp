#include <iostream>
#include <vector>
#include "util.h"

namespace util {
int cycle_through_bounds(int start_value, int cycle_count, int start_bound, int end_bound) {
	//int cycled_position = start_value + cycle_count;
	int bound_len = end_bound - start_bound;

	if (bound_len <= 0) {
		return start_value;
	}

	int cycled_position = 0;
	cycled_position = (start_value + cycle_count) % (bound_len+1);

	if (cycled_position < 0) {
		cycled_position += bound_len+1;
	}

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

void str_strip(std::string& str) { 
	char target_char = ' ';
	char current_char;
	char prev_char;
	int index = 0;
	int index_offset = 0;
	int start_pos = -1;

	size_t strlen = str.length();
	size_t slice_count = 1;


	//std::cout << "Old String: " << str << "\n";

	for (int i=1; i<strlen; i++) {
		index = i - index_offset;

		current_char = str[i];
		prev_char = str[i-1];

		if (current_char != ' ' && start_pos != -1) {
			slice_count = i - start_pos + 1;
			//std::cout << "Pos: " << slice_count << " " << start_pos << "\n";
			
			str = str.substr(0, start_pos) + str.substr(i-1, strlen-i);
			//std::cout << "New String: " << str << "\n";
			index_offset += slice_count;
			strlen -= slice_count;

			
			start_pos = -1;
			break;
		}

		if (start_pos == -1 && current_char == target_char && prev_char == target_char) { 
			//std::cout << "Start pos tracked!\n";
			start_pos = i-1;
		}
	}
}
}
