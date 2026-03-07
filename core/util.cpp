#include <iostream>
#include <vector>
#include "util.h"

struct DynamicHeader {
	size_t size;
};

namespace util {

void* dynamic_data_init(size_t struct_size) {
	//function allocates some data from the heap attaches a header
	//to said data and returns the data
	DynamicHeader* header = static_cast<DynamicHeader*>(malloc(sizeof(struct_size) + sizeof(DynamicHeader)));
	void* data = header+1;

	header->size = struct_size;

	return data;
}

void* dynamic_data_assign(void* dynamic_data, size_t new_struct_size) {
	//this function allows one to assign previously dynamicly allocated
	//data to another data sturcture avoiding the need to allocate more
	//memory

	if (dynamic_data == nullptr) {
		std::cout << "ERROR: dynamic data is a nullptr\n";	
	}

	DynamicHeader* header = static_cast<DynamicHeader*>(dynamic_data);
	header--;

	//if additional memory is needed reallocate the data and 
	//reassign the pointers accordingly
	if (new_struct_size > header->size) {
		header = (DynamicHeader*)realloc(header, new_struct_size + sizeof(DynamicHeader));
		dynamic_data = header+1;
	}
	
	return dynamic_data;
}

void dynamic_data_free(void* dynamic_data) {
	DynamicHeader* header = (DynamicHeader*)dynamic_data;
	header--;
	free(header);
}

int cycle_through_bounds(int start_value, int cycle_count, int start_bound, int end_bound) {
	//int cycled_position = start_value + cycle_count;
	int bound_len = end_bound - start_bound;

	if (bound_len <= 0) {
		return start_value;
	}

	int cycled_position = 0;
	cycled_position = (start_value + cycle_count) % (bound_len);

	if (cycled_position < 0) {
		cycled_position += bound_len;
	}

	return cycled_position;
}

void str_split(const std::string& src_str, char split_char, std::vector<std::string>& output_vec) {
	unsigned int start_pos = 0, copy_count = 0, copy_offset = 0, final_index = 0;
	size_t strlen = src_str.length();
	char curr_char = ' ';
	std::string sub_str = "";

	//use vector.at notation to preserve orginal implementatino


	final_index = strlen - 1;

	for (int i=0; i<strlen; i++) {
		curr_char = src_str.at(i);
		
		if (i == final_index) {
			copy_offset++;
		}
	
		if (curr_char == split_char || i == final_index) {
			copy_count = i - start_pos + copy_offset;

			sub_str = src_str.substr(start_pos, copy_count);
			output_vec.push_back(sub_str);

			start_pos = i+1;
		}
	}
}

void str_unsplit(std::vector<std::string>& str_vec, std::string& output_str, char split_char) {
	output_str = "";

	for (const std::string& str : str_vec) {
		output_str += str + split_char;
	}

	output_str.pop_back();
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
