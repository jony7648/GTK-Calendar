#include <fstream>
#include <algorithm>
#include "csv_writer.h"
#include "util.h"

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
	_file_stream.open(file_path, std::ios::app | std::ios::in);
	_header_vec = header_vec;
}

CsvWriter::~CsvWriter() {
	_file_stream.close();
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



void CsvWriter::write_csv(std::vector<CsvMap>& data_vec) {
	_file_stream.seekg(0);

	std::string current_line = "";

	getline(_file_stream, current_line);

	std::string csv_line = "";
	std::string csv_sub_line = "";
	std::string output_csv_str = "";

	for (CsvMap& data_vec : data_vec) {
		csv_line = "";
		for (const std::string& header : _header_vec) {
			csv_sub_line = data_vec[header];

			line_to_csv_line(csv_sub_line, ',');

			csv_line += csv_sub_line + ",";
		}	

		csv_line.pop_back();
		output_csv_str += csv_line + "\n";
	}
	
	_file_stream << output_csv_str;
		
}

void CsvWriter::read_csv(std::vector<CsvMap>& data_vec) {
	set_seek_pos(1);

	std::vector<std::string> line_vec;
	std::string current_line;

	std::vector<int> remove_locations(20, -1);

	while (getline(_file_stream, current_line)) {
		std::fill(remove_locations.begin(), remove_locations.end(), -1);
		parse_csv_line(data_vec, current_line, remove_locations);
	}
}

void CsvWriter::set_seek_pos(unsigned int seek_pos) {
	_file_stream.seekg(0);
	std::string throw_away = "";

	for (int i=0; i<seek_pos; i++) {
		getline(_file_stream, throw_away);
	}
}
}
