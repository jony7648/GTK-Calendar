#include "csv_writer.h"
#include "util.h"
#include "file_util.h"

int extract_csv_data(std::vector<core::CsvWriter::DataMap>& data_map_vector, std::vector<std::string> file_contents) {
	const std::string& header_line = file_contents.at(0);
	size_t strlen = 0;
	uint16_t copy_count = 0, copy_offset = 0, sub_str_start = 0;
	std::string sub_str;

	char sub_str_char = ' ';
	char split_char = ',';


	int cat_value_index = 0, data_index = 0;

	std::vector<std::string> header_vec(4);
	size_t header_vec_size = header_vec.size();

	int error_value = 0;


	util::str_split(header_vec, header_line, ',');

	//parse through csv
	for (int i=1; i<file_contents.size(); i++) {
		const std::string& current_line = file_contents.at(i);
		data_index = i-1;
		cat_value_index = 0;
		sub_str_start = 0;
		copy_offset = 0;

		strlen = current_line.length();

		data_map_vector.push_back(core::CsvWriter::DataMap());
		core::CsvWriter::DataMap& data_map = data_map_vector.at(data_index);
		
		for (int j=0; j<strlen; j++) {
			if (cat_value_index >= header_vec_size) {
				std::cout << "Too many elements in csv line " << i+1 << "\n";
				cat_value_index++;
				error_value = 1;
				break;
			}

			if (j == strlen - 1) {
				copy_offset = 1;
			}

			const std::string& category = header_vec.at(cat_value_index);
			sub_str_char = current_line[j];
			
			if (sub_str_char == split_char || j == strlen - 1) {
				copy_count = j - sub_str_start + copy_offset;

				sub_str = current_line.substr(sub_str_start, copy_count);
				data_map[category] = sub_str;
				sub_str_start = j+1;
				cat_value_index++;
			}
		}
	}

	return error_value;
}

namespace core {
CsvWriter::CsvWriter(const std::string& file_path) {
	_file_stream.open("test.save", std::ios::app | std::ios::in);
	_file_path = file_path;
}

CsvWriter::~CsvWriter() {
	_file_stream.close();
}

void CsvWriter::save_map(const DataMap& data_map) {
	std::string data_str = "";
	short cut_count = 1;

	for (const auto& [key, value] : data_map) {
		data_str += value + ",";
	}

	data_str = data_str.substr(0, data_str.length() - cut_count);

	std::fstream test("test.save");

	file_util::write_line(test, data_str);
}

void CsvWriter::get_csv_data(std::vector<DataMap>& data_map_vector) {
	std::vector<std::string> file_contents;

	file_util::get_file_contents(_file_stream, file_contents);

	extract_csv_data(data_map_vector, file_contents);
}
}
