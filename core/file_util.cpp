#include <fstream>
#include <filesystem>
#include "file_util.h"

namespace file_util {
void get_file_contents(std::ifstream& file_stream, std::vector<std::string>& line_vector) {
	if (file_stream.is_open() == false) {
		std::cout << "ERROR: file is not open!\n";
	}

	std::string current_line = "";

	while (getline(file_stream, current_line)) {
		line_vector.push_back(current_line);
	}


}

void get_files_in_dir(std::vector<std::string>& path_vector, const std::string& dir_path) {
	namespace fs = std::filesystem;
	

	for (auto& entry : fs::directory_iterator(dir_path)) {
		if (entry.is_directory()) {
			continue;
		}

		path_vector.push_back(entry.path());	
	}
}
}
