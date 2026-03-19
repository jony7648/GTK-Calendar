#pragma once
#include <iostream>
#include <vector>

namespace util {
	void* dynamic_data_init(size_t data_size);
	void* dynamic_data_assign(void* dynamic_data, size_t new_struct_size);
	void dynamic_data_free(void* dynamic_data);
	void str_split(const std::string& src_str, char split_char, std::vector<std::string>& output_vec);
	void str_unsplit(std::vector<std::string>& str_vec, std::string& output_str, char split_char);
	int cycle_through_bounds(int start_value, int cycle_count, int start_bound, int end_bound);
	void str_strip(std::string& str);

	template <typename T>
	void display_vec_info(const T& vec) {
		for (auto& elem : vec) {
			std::cout << elem << " ";	
		}
	}

	template <typename T>
	void display_multivec_info(const T& multi_vec) {
		for (int i=0; i<multi_vec.size(); i++) {
			std::cout << i << ": ";
			for (auto& elem : multi_vec.at(i)) {
				std::cout << elem << " ";	

			}

			std::cout << "\n";
		}
	}
}
