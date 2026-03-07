#include <fstream>
#include <algorithm>
#include "csv_writer.h"
#include "util.h"
#include "error.h"
#include "file_util.h"

const std::string NO_FILE_MESSAGE = "FILE COULD NOT BE OPENED";

const char ESCAPE_ARR[] = {'"', '\\'};
const size_t ESCAPE_ARR_LEN = sizeof(ESCAPE_ARR) / sizeof(ESCAPE_ARR[0]);


void line_to_csv_line(std::string& line, char split_char) {
	char curr_char = ' ';
	char check_char = ' ';

	size_t strlen = line.length();

	bool wrap_in_quotes = false;

	for (int i=0; i<strlen; i++) {
		curr_char = line.at(i);	

		if (curr_char == split_char) {
			wrap_in_quotes = true;
			continue;
		}

		for (int j=0; j<ESCAPE_ARR_LEN; j++) {
			check_char = ESCAPE_ARR[j];

			// if an escape char is found insert a backslash then skip
			// the following iteration due to the longer strlen
			if (curr_char == check_char) {
				line.insert(i,1,'\\');
				strlen++;
				i++;
			}
		}
	}

	if (wrap_in_quotes) {
		line.insert(0,1,'\"');
		line.push_back('\"');
	}
}

void remove_chars_from_str(std::string& src_str, std::vector<int>& location_vec) {
	int location = 0;
	int remove_count = 0;

	for (int location : location_vec) {
		if (location < 0) {
			break;
		}


		src_str.erase(location - remove_count,1);
		remove_count++;
	}
}

namespace core {
CsvWriter::CsvWriter(const std::string& file_path, const std::vector<std::string>& header_vec) {
	_file_path = file_path;	
	_header_vec = header_vec;
}

void CsvWriter::csv_map_to_str(CsvMap& data, std::string& output_line) {
	std::string dummy_str = "";
	output_line = "";

	for (const std::string& header : _header_vec) {
		dummy_str = data[header];
		line_to_csv_line(dummy_str, ',');
		output_line.append(dummy_str + ",");			
	}

	output_line.pop_back();
}

void CsvWriter::parse_csv_line(std::vector<CsvMap>& data_vec, std::string csv_line, std::vector<int>& remove_locations) {
	char curr_char = ' ';
	char split_char = ',';
	char string_char = '"';

	std::string sub_str = "";
	std::string current_header = "";
	
	bool string_mode = false;
	char escape_char = '\\';
	int escape_vec_index = 0;
	bool should_split = false;
	int start_pos = 0, copy_count = 0, copy_offset = 0, value_index = 0;
	size_t line_len = csv_line.length();
	
	data_vec.push_back(CsvMap());
	CsvMap& data_map = data_vec.back();
	
	for (int i=0; i<line_len; i++) {
		current_header = _header_vec[value_index];
		curr_char = csv_line.at(i);
		should_split = false;
	
		if (curr_char == escape_char) {
			remove_locations.at(escape_vec_index) = i - start_pos;
			escape_vec_index++;
			i++;
			continue;
		}
	
		if ((curr_char == split_char && !string_mode) || i == line_len-1) {
			should_split = true;
		}
	
		if (i == line_len-1) {
			copy_offset++;
		}
	
		if (curr_char == string_char) {
			if (!string_mode) {
				string_mode = true;
				start_pos = i+1;
				continue;
			}
	
			string_mode = false;
			copy_offset--; 
		}
	
		if (should_split) {
			copy_count = i - start_pos + copy_offset;
			sub_str = csv_line.substr(start_pos, copy_count);
			remove_chars_from_str(sub_str, remove_locations);
			data_map[current_header] = sub_str;
			value_index++;
			start_pos = i+1;
			copy_offset = 0;
		}
	}
}

CsvWriter::CsvMap* CsvWriter::check_for_same_line(std::vector<CsvMap>& data_vec, const std::string& current_line) {
	bool found_csv_match = false;
	CsvMap* data_ptr = nullptr;

	std::vector<std::string> split_vec;

	util::str_split(current_line, ',', split_vec);

	for (CsvMap& data : data_vec) {

		data_ptr = &data;

		for (int i=_same_check_start; i<_same_check_end; i++) {
			found_csv_match = true;

			const std::string& file_elem = split_vec.at(i);
			const std::string& current_header = _header_vec.at(i);
			const std::string& save_elem = data[current_header];

			std::cout << "Elem " << i << ": " << file_elem << "  "  << save_elem << "\n";

			if (file_elem != save_elem) { 
				std::cout << "Not Same!\n";
				data_ptr = nullptr;
				break;
			}
		}

		//if we found a matching entry return it
		if (data_ptr) {
			break;
		}


	}

	return data_ptr;
}

Error CsvWriter::write_csv(std::vector<CsvMap>& data_vec) {
	//Meathod takes in passed in csv data and saves it to a file

	//rewrite meathod to allow for the replacement for certain line

	const size_t DATA_COUNT = data_vec.size();


	//if there are no enteries don't save anything
	if (DATA_COUNT <= 0) {
		return Error(ErrorType::Clear);
	}

	bool* write_state_arr = nullptr;

	std::ifstream input_stream;
	std::string input_stream_line;
	std::vector<std::string> file_data_vec;
	std::string header_line;

	std::ofstream output_stream;
	
	std::string output_csv_str = "";
	unsigned int line_index = 0;

	CsvMap* matching_data = nullptr;


	input_stream.open(_file_path);
	file_util::set_stream_start_line(input_stream, 1);

	if (!input_stream.is_open()) {
		std::cout << "Error: File stream failed to open for whatever reason\n";
		return Error(ErrorType::FileOpenFail);
	}

	write_state_arr = new bool[DATA_COUNT]{true};

	//populate file_data_vec
	while (getline(input_stream, input_stream_line)) {
		//check to see if there is a data entry that mataches this line
		matching_data = check_for_same_line(data_vec, input_stream_line);

		//if so replace the line
		if (matching_data) {
			csv_map_to_str(*matching_data, input_stream_line);
			write_state_arr[line_index] = false;
		}

		file_data_vec.push_back(input_stream_line);
		line_index++;
	}

	//close the file stream
	input_stream.close();


	//add data enteries that we could not find a match for
	for (int i=0; i<DATA_COUNT; i++) {
		bool state = write_state_arr[i];

		CsvMap& map = data_vec.at(i);

		if (state) {
			csv_map_to_str(map, input_stream_line);
			file_data_vec.push_back(input_stream_line);
		}
	}


	output_stream.open(_file_path);


	//write header line
	util::str_unsplit(_header_vec, header_line, ',');
	output_stream << header_line << "\n";

	for (const std::string& line_str : file_data_vec) {
		output_stream << line_str << "\n";
	}

	output_stream.close();


	delete[] write_state_arr;

	return Error(ErrorType::Clear);
}



Error CsvWriter::read_csv(std::vector<CsvMap>& data_vec) {
	std::ifstream input_stream(_file_path);

	if (!input_stream.is_open()) {
		return Error(ErrorType::FileOpenFail);
	}

	std::vector<std::string> line_vec;
	std::string current_line;

	std::vector<int> remove_locations(20, -1);

	//discard header line
	file_util::set_stream_start_line(input_stream, 1);

	while (getline(input_stream, current_line)) {
		if (current_line.empty()) {
			continue;
		}

		std::fill(remove_locations.begin(), remove_locations.end(), -1);
		parse_csv_line(data_vec, current_line, remove_locations);
	}

	input_stream.close();

	return Error(ErrorType::Clear);
}

void CsvWriter::replace_line(unsigned int target_line_num, const std::string line_contents) {
	//Meathod replaces a specified line number with the line contents string

	std::vector<std::string> line_vec;
	std::string output_line;
	std::string full_output_str = "";

	std::fstream file_stream(_file_path);

	file_util::get_file_contents(file_stream, line_vec);

	//maybe change this line vector line as it may not need to be there
	line_vec.at(target_line_num) = line_contents;

	for (int i=0; i<line_vec.size(); i++) {
		if (i == target_line_num) {
			output_line = line_contents;
		}
		else {
			output_line = line_vec.at(i);
		}

		full_output_str += output_line + "\n";
	}

	file_stream << full_output_str;
}

int CsvWriter::find_matching_line(core::CsvWriter::CsvMap& csv_map, unsigned int start_index, unsigned int end_index) {
	int matching_line = 0;

	std::ifstream file_stream(_file_path);

	if (file_stream.is_open()) {
		return -1;
	}

	file_stream.seekg(1);


	std::string current_line;

	std::vector<std::vector<std::string>> line_split_vec;

	unsigned int vec_index = 0;

	while (getline(file_stream, current_line)) {
		util::str_split(current_line, ',', line_split_vec.at(vec_index));
		vec_index++;
	}

	for (int i=0; i<line_split_vec.size(); i++) {
		if (matching_line != -1) {
			break;
		}

		std::vector<std::string>& split_vec = line_split_vec.at(i);

		for (int j=start_index; i<end_index; j++) {
			if (j >= _header_vec.size()) {
				std::cout << "Failed to find matching CSV line! index goes out of header vec bounds for file path \"" << _file_path << "\"\n";
				return -1;
			}

			if (j >= split_vec.size()) {
				std::cout << "Failed to find matching CSV line! index goes out of line split vec bounds for file path \"" << _file_path << "\"\n";
				return -1;
			}

			const std::string& split_str = split_vec.at(j);
			const std::string& current_header = _header_vec.at(j);
			const std::string& data_value = csv_map[current_header];

			if (data_value == split_str) {
				matching_line = j;
				break;
			}
		}
	}

	return matching_line;
}

void CsvWriter::set_equivalnce_bounds(int start_index, int end_index) {
	_same_check_start = start_index;
	_same_check_end = end_index;
}
}
