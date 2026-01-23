#pragma once
#include <iostream>
#include <ctime>



namespace core {
struct TimeComponet {
	std::string weekday_arr[7];
	std::string month_arr[12];

	int day_count_arr[12];

	const size_t DAY_COUNT_ARR_LEN = 12;
	const size_t WEEKDAY_COUNT = 7;
	const size_t MONTH_COUNT = 12;
	const short LEAP_FACTOR = 4;

	int day = 0;
	int month = 0;
	int year = 0;
	int weekday = 0;
	int hour = 0;
	int minute = 0;
	int second = 0;

	TimeComponet();
	
	void calculate_from_current_time();
	void calculate_from_time_t(time_t time);
	int find_leap_year_count(int start_year, int end_year);
	int month_to_int(const std::string& conv_month);
	int weekday_to_int(const std::string& conv_day);
	void display_time_info();
	void set_minute_and_hour(const std::string& str);
	int get_day_count();
	int get_day_count(int month);
	int get_starting_weekday();
	int get_starting_weekday(int month, int year);

};

}
