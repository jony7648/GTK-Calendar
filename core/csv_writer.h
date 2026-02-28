#pragma once
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>

namespace core {
class CsvWriter {
public:
	using CsvMap = std::unordered_map<std::string, std::string>;
	CsvWriter(const std::string& file_path, const std::vector<std::string>& header_vec);
	CsvWriter(const CsvWriter&) = delete;
	~CsvWriter();

	void read_csv(std::vector<CsvMap>& output_vec);
	void write_csv(std::vector<CsvMap>& csv_map);
	void set_seek_pos(unsigned int seek_pos); 


private:
	std::string _file_path;
	std::fstream _file_stream;
	std::vector<std::string> _header_vec;
	void parse_csv_line(std::vector<CsvMap>& data_vec, std::string csv_line, std::vector<int>& remove_locations);
};
}
