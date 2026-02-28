#pragma once
#include <iostream>
#include <ctime>
#include "date.h"


namespace core {

class TimeComponet {
private:
	std::string _weekday_arr[7];
	std::string _month_arr[12];
	std::string _long_mth_names[12];

	int _day_count_arr[12];
	
	Date _date;
	Time _time;
	Date _menu_date;
	int _weekday = 0;

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

	void set_menu_day(int day);
	void set_menu_month(int month);
	void set_menu_year(int year);
	void set_menu_date(Date& date);

	int get_day_count();
	int get_day_count(int month);
	int get_starting_weekday();
	int get_starting_weekday(int month, int year);
	const std::string& get_short_month_name(int month);
	const std::string& get_long_month_name(int month);
	void get_full_day_str(core::Date& date, std::string& output_str);
	std::string* get_weekday_arr();

	int get_menu_month();
	int get_menu_year();

	const Date& get_menu_date();
	
};

}
