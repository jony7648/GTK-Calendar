#pragma once
#include <iostream>
#include <ctime>



namespace core {
class TimeComponet {
private:
	std::string weekday_arr[7];
	std::string month_arr[12];
	std::string long_mth_names[12];

	int day_count_arr[12];
	
	int day = 0;
	int month = 0;
	int year = 0;
	int weekday = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;

	int menu_month = 0;
	int menu_year = 0;

public:
	const size_t DAY_COUNT_ARR_LEN = 12;
	const size_t WEEKDAY_COUNT = 7;
	const size_t MONTH_COUNT = 12;
	const short LEAP_FACTOR = 4;

	TimeComponet();
	
	void calculate_from_current_time();
	void calculate_from_time_t(time_t time);
	int find_leap_year_count(int start_year, int end_year);
	int month_to_int(const std::string& conv_month);
	int weekday_to_int(const std::string& conv_day);
	void display_time_info();
	void set_minute_and_hour(const std::string& str);
	void advance_menu_month(int cycle_count);
	void reset_menu_time();

	int get_day_count();
	int get_day_count(int month);
	int get_starting_weekday();
	int get_starting_weekday(int month, int year);
	const std::string& get_short_month_name(int month);
	const std::string& get_long_month_name(int month);
	void get_full_day_str(int day, int month, int year, std::string& output_str);
	std::string* get_weekday_arr();

	int get_menu_month();
	int get_menu_year();
};

}
