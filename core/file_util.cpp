#include <fstream>
#include <filesystem>
#include "file_util.h"
#include "error.h"

namespace file_util {
void set_stream_start_line(std::ifstream& stream, int line) {
	std::string dummy_str;

	for (int i=0; i<line; i++) {
		getline(stream, dummy_str);
	}
}

void set_stream_start_line(std::fstream& stream, int line) {
	std::string dummy_str;

	for (int i=0; i<line; i++) {
		getline(stream, dummy_str);
	}
}

core::Error get_file_contents(std::ifstream& file_stream, std::vector<std::string>& line_vector) {
	if (file_stream.is_open() == false) {
		std::cout << "ERROR: file is not open!\n";
		return core::Error(core::ErrorType::FileOpenFail);
	}

	//reset the position back to the start
	file_stream.seekg(0, std::ios::beg);

	std::string current_line = "";

	while (getline(file_stream, current_line)) {
		line_vector.push_back(current_line);
	}

	return core::Error(core::ErrorType::Clear);
}

core::Error get_file_contents(std::fstream& file_stream, std::vector<std::string>& line_vector) {
	if (file_stream.is_open() == false) {
		std::cout << "ERROR: file is not open!\n";
		return core::Error(core::ErrorType::FileOpenFail);
	}

	//reset the position back to the start
	file_stream.seekg(0, std::ios::beg);

	std::string current_line = "";


	while (getline(file_stream, current_line)) {
		std::cout << current_line.length() << "\n";
		line_vector.push_back(current_line);
	}

	return core::Error(core::ErrorType::Clear);
}

void get_files_in_dir(std::vector<std::string>& path_vector, const std::string& dir_path) {
	namespace fs = std::filesystem;


	for (auto& entry : fs::directory_iterator(dir_path)) {
		if (entry.is_directory()) {
			continue;
		}

		path_vector.push_back(entry.path().string());
	}
}

std::string get_parent_dir_from_file(const std::string& path) {
	std::string parent_dir = path;
	char curr_char = ' ';

	for (int i=path.length()-1; i>=0; i--) {
		if (curr_char == '/') {
			parent_dir = parent_dir.substr(0, i-1);
			break;
		}
	}

	return parent_dir;
}

bool file_exists(const std::string& path) {
	return std::filesystem::is_regular_file(path);
}

void write_line(std::fstream& file_stream, const std::string& line) {
	if (!file_stream.is_open()) {
		return;
	}

	file_stream << line << "\n";
}
}
