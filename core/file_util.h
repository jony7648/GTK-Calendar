#pragma once
#include "error.h"

#include <iostream>
#include <vector>

namespace file_util {
	core::Error get_file_contents(std::ifstream& file_stream, std::vector<std::string>& line_vector);
	core::Error get_file_contents(std::fstream& file_stream, std::vector<std::string>& line_vector);
	void get_files_in_dir(std::vector<std::string>& path_vector, const std::string& dir_path);
	void write_line(std::fstream& file_stream, const std::string& line);
	void set_stream_start_line(std::ifstream& stream, int line);
	void set_stream_start_line(std::fstream& stream, int line);
	bool file_exists(const std::string& path);
}


