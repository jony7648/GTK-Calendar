#include <iostream>
#include <vector>
#include <print>
#include <ctime>
#include "util.h"
#include "time_componet.h"

enum CTIME_INDEX {
	CTIME_WEEKDAY,
	CTIME_MONTH,
	CTIME_DAY,
	CTIME_TIME,
	CTIME_YEAR,
};

void readable_time_to_vector(std::vector<std::string>& time_vector) {
	time_t current_time;
	time(&current_time);


	std::string readable_time = std::ctime(&current_time);
	util::str_strip(readable_time);
	std::cout << readable_time << "\n";
	util::str_split(time_vector, readable_time, ' ');
}

namespace core {
TimeComponet::TimeComponet() {
	weekday_arr[0] = "Sun";
	weekday_arr[1] = "Mon";
	weekday_arr[2] = "Tue";
	weekday_arr[3] = "Wed";
	weekday_arr[4] = "Thu";
	weekday_arr[5] = "Fri";
	weekday_arr[6] = "Sat";

	month_arr[0] = "Jan";
	month_arr[1] = "Feb";
	month_arr[2] = "Mar";
	month_arr[3] = "Apr";
	month_arr[4] = "May";
	month_arr[5] = "Jun";
	month_arr[6] = "Jul";
	month_arr[7] = "Aug";
	month_arr[8] = "Sep";
	month_arr[9] = "Oct";
	month_arr[10] = "Nov";
	month_arr[11] = "Dec";


	long_mth_names[0] = "Janurary";
	long_mth_names[1] = "February";
	long_mth_names[2] = "March";
	long_mth_names[3] = "April";
	long_mth_names[4] = "May";
	long_mth_names[5] = "June";
	long_mth_names[6] = "July";
	long_mth_names[7] = "August";
	long_mth_names[8] = "September";
	long_mth_names[9] = "October";
	long_mth_names[10] = "November";
	long_mth_names[11] = "December";


	day_count_arr[0] = 31;
	day_count_arr[1] = 28;
	day_count_arr[2] = 31;
	day_count_arr[3] = 30;
	day_count_arr[4] = 31;
	day_count_arr[5] = 30;
	day_count_arr[6] = 31;
	day_count_arr[7] = 31;
	day_count_arr[8] = 30;
	day_count_arr[9] = 31;
	day_count_arr[10] = 30;
	day_count_arr[11] = 31;

	calculate_from_current_time();
	reset_menu_time();
}

int TimeComponet::find_leap_year_count(int start_year, int end_year) {
	int next_leap = start_year + (start_year % LEAP_FACTOR);
	int leap_count = (end_year - next_leap + LEAP_FACTOR) / LEAP_FACTOR;

	return leap_count;
	
}

int TimeComponet::month_to_int(const std::string& conv_month) {
	int month_index = 0;

	for (int i=0; i<MONTH_COUNT; i++) {
		const std::string& month = month_arr[i];	

		if (conv_month.compare(month) == 0) {
			month_index = i;
		}
	}

	return month_index;
}

int TimeComponet::weekday_to_int(const std::string& conv_day) {
	int day_index = 0;

	for (int i=0; i<WEEKDAY_COUNT; i++) {
		const std::string& day = weekday_arr[i];	

		if (conv_day.compare(day) == 0) {
			day_index = i;
		}
	}

	
	return day_index;
}

void TimeComponet::display_time_info() {
	std::println(
	"Day: {}\nMonth: {}\nYear: {}\nWeekday: {}\nHour: {}\nMinute: {}\nSecond: {}", 
		day, month, year, weekday, hour, minute, second
	);
}

void TimeComponet::set_minute_and_hour(const std::string& time_str) {

}

void TimeComponet::calculate_from_current_time() {
	std::vector<std::string> time_vector;

	readable_time_to_vector(time_vector);

	int attrib_value = 0;

	for (int i=0; i<time_vector.size(); i++) {
		const std::string& str = time_vector.at(i);
		switch (i) {
			case CTIME_DAY:
				this->day = std::stoi(str);
				break;
			case CTIME_MONTH:
				this->month = month_to_int(str);
				break;
			case CTIME_WEEKDAY:
				this->weekday = weekday_to_int(str);
				break;
			case CTIME_TIME:
				set_minute_and_hour(str);
				break;
			case CTIME_YEAR:
				this->year = std::stoi(str);
				break;

		}
	}
}

int TimeComponet::get_day_count() {
	//this method will return the amount of days in a month
	if (month >= MONTH_COUNT || month < 0) {
		std::cout << "CRITICAL ERROR: MONTH IS OUT OF RANGE!!!" << "\n";
		return 0;
	}
	return day_count_arr[month];
}

int TimeComponet::get_day_count(int month) {
	//this method will return the amount of days in a month
	if (month >= MONTH_COUNT || month < 0) {
		std::cout << "CRITICAL ERROR: MONTH IS OUT OF RANGE!!!" << "\n";
		return 0;
	}

	return day_count_arr[month];
}

int TimeComponet::get_starting_weekday() {
	return weekday;
}

void TimeComponet::advance_menu_month(int cycle_count) {
	if (menu_month + cycle_count < 0) {
		menu_year--;
	}
	
	if (menu_month + cycle_count == MONTH_COUNT) {
		menu_year++;
	}

	//menu_year = menu_year + (menu_month + cycle_count) / (MONTH_COUNT);
	menu_month = util::cycle_through_bounds(menu_month, cycle_count, 0, MONTH_COUNT);
}

void TimeComponet::reset_menu_time() {
	menu_month = month;
	menu_year = year;
}

const std::string& TimeComponet::get_long_month_name(int month) {
	return long_mth_names[month];
}

const std::string& TimeComponet::get_short_month_name(int month) {
	return month_arr[month];
}


int TimeComponet::get_menu_month() {
	return menu_month;
}

int TimeComponet::get_menu_year() {
	return menu_year;
}

std::string* TimeComponet::get_weekday_arr() {
	return weekday_arr;
}

void TimeComponet::get_full_day_str(int day, int month, int year, std::string& output_str) {
	const std::string& RD_ENDING = "rd";			
	const std::string& TH_ENDING = "th";			
	const std::string& ST_ENDING = "st";			
	const std::string& ND_ENDING = "nd";			

	int day_last_num = day % 10;

	std::string day_suffix = "";

	if (day > 3 and day < 21) {
		day_suffix = TH_ENDING;
	}
	else if (day_last_num > 4) {
		day_suffix = TH_ENDING;
	}
	else if (day_last_num == 3) {
		day_suffix = RD_ENDING;
	}
	else if (day_last_num == 2) {
		day_suffix = ND_ENDING;
	}
	else {
		day_suffix = ST_ENDING;	
	}

	const std::string& month_str = long_mth_names[month];
	const std::string& day_str = std::to_string(day) + day_suffix;

	output_str = month_str + " " + day_str + ", " + std::to_string(year);
}

int TimeComponet::get_starting_weekday(int target_month, int target_year) {
	//this meathod will return the weekday that the month starts on

	int years_ahead = target_year - year;
	int months_ahead = years_ahead * MONTH_COUNT + target_month - month;
	int days_to_cycle = -(day-1); //converts to index format and sets it so it will loop back to day zero
	int leap_year_count = 0;
	int cycle_dir = 1;

	//std::cout << "Starting days to cycle: " << days_to_cycle << "\n";
	int current_month = 0;

	if (months_ahead < 0) {
		cycle_dir = -1;
	}


	//calculate the amount of days to cycle (without leap years)
	for (int i=0; i<abs(months_ahead); i++) {
		current_month = util::cycle_through_bounds(month, i*cycle_dir, 0, MONTH_COUNT);
		
		if (cycle_dir < 0 ) {
			//if cycle_dir < 0, decrement current_month to access
			//the day count of the previous month
			current_month = util::cycle_through_bounds(current_month, -1, 0, MONTH_COUNT);
		}

		days_to_cycle += day_count_arr[current_month] * cycle_dir;
	}

	//account for leap years
	leap_year_count = find_leap_year_count(year, target_year);
	days_to_cycle += leap_year_count;

	//if the target month is before march don't account for 
	//febuary's leap year
	if (target_month < 2 and leap_year_count > 0) {
		days_to_cycle--;
	}


	int result = util::cycle_through_bounds(weekday, days_to_cycle, 0, WEEKDAY_COUNT);

	/*
	std::println(
		"Weekday: {}\nDays to cycle: {}\nMonths Ahead: {}\nResult: {}",
		weekday, days_to_cycle, months_ahead, result
	);
	*/

	return result;
}
}
