#pragma once
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include "error.h"


namespace core {

class CsvWriter {
public:
	using CsvMap = std::unordered_map<std::string, std::string>;
	CsvWriter(const std::string& file_path, const std::vector<std::string>& header_vec);
	CsvWriter(const CsvWriter&) = delete;

	Error read_csv(std::vector<CsvMap>& output_vec);
	Error write_csv(std::vector<CsvMap>& csv_map);
	int find_matching_line(core::CsvWriter::CsvMap& csv_map, unsigned int start_index, unsigned int end_index);
	void replace_line(unsigned int line_num, const std::string line_contents);
	void csv_map_to_str(CsvMap& data, std::string& output_line);

	void set_equivalnce_bounds(int start, int end);

private:
	int _same_check_start = 0;
	int _same_check_end = 0;
	std::string _file_path;
	std::vector<std::string> _header_vec;
	void parse_csv_line(std::vector<CsvMap>& data_vec, std::string csv_line, std::vector<int>& remove_locations);
	CsvMap* check_for_same_line(std::vector<CsvMap>& data_vec, const std::string& current_line);
};
}
