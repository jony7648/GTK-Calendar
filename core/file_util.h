#pragma once
#include <iostream>
#include <vector>

namespace file_util {
extern void get_file_contents(std::ifstream& file_stream, std::vector<std::string>& line_vector);
extern void get_files_in_dir(std::vector<std::string>& path_vector, const std::string& dir_path);
}


