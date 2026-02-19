#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

namespace core {
class CsvWriter {
private:
	std::string _file_path;
	std::fstream _file_stream;
public:
	using DataMap = std::map<std::string, std::string>;

	CsvWriter(const std::string& file_path);
	CsvWriter(const CsvWriter&) = delete;
	~CsvWriter();


	void save_map(const DataMap& data_map);
	void get_csv_data(std::vector<DataMap>& data_map_vector);

};
}
