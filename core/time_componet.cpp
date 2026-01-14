#include <iostream>
#include <vector>
#include <print>
#include <ctime>
#include "util.h"
#include "time_componet.h"

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
	enum {
		WEEKDAY_INDEX,
		MONTH_INDEX,
		DAY_INDEX,
		TIME_INDEX,
		YEAR_INDEX,
	};


	time_t current_time;
	time(&current_time);

	std::string readable_time = std::ctime(&current_time);
	std::vector<std::string> time_vector;


	util::str_split(time_vector, readable_time, ' ');

	int attrib_value = 0;

	for (int i=0; i<time_vector.size(); i++) {
		const std::string& str = time_vector.at(i);
		switch (i) {
			case DAY_INDEX:
				this->day = std::stoi(str);
				break;
			case MONTH_INDEX:
				this->month = month_to_int(str);
				break;
			case WEEKDAY_INDEX:
				this->weekday = weekday_to_int(str);
				break;
			case TIME_INDEX:
				set_minute_and_hour(str);
				break;
			case YEAR_INDEX:
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



int get_starting_weekday() {
	//this meathod will return the weekday that the month starts on
	//NEEDS TO BE IMPLEMENTED
	return 0;
		
}
}
