#include <iostream>
#include <vector>
#include "util.h"

namespace util {
std::vector<std::string>& str_split(std::vector<std::string>& store_vector, const std::string& str, char split_char) {
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
