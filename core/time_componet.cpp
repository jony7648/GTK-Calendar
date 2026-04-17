#include <iostream>
#include <vector>
#include <ctime>
#include "util.h"
#include "time_componet.h"
#include <glib.h>

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
	util::str_split(readable_time, ' ', time_vector);
}

namespace core {
TimeComponet::TimeComponet() {
	_weekday_arr[0] = "Sun";
	_weekday_arr[1] = "Mon";
	_weekday_arr[2] = "Tue";
	_weekday_arr[3] = "Wed";
	_weekday_arr[4] = "Thu";
	_weekday_arr[5] = "Fri";
	_weekday_arr[6] = "Sat";

	_month_arr[0] = "Jan";
	_month_arr[1] = "Feb";
	_month_arr[2] = "Mar";
	_month_arr[3] = "Apr";
	_month_arr[4] = "May";
	_month_arr[5] = "Jun";
	_month_arr[6] = "Jul";
	_month_arr[7] = "Aug";
	_month_arr[8] = "Sep";
	_month_arr[9] = "Oct";
	_month_arr[10] = "Nov";
	_month_arr[11] = "Dec";


	_long_mth_names[0] = "Janurary";
	_long_mth_names[1] = "February";
	_long_mth_names[2] = "March";
	_long_mth_names[3] = "April";
	_long_mth_names[4] = "May";
	_long_mth_names[5] = "June";
	_long_mth_names[6] = "July";
	_long_mth_names[7] = "August";
	_long_mth_names[8] = "September";
	_long_mth_names[9] = "October";
	_long_mth_names[10] = "November";
	_long_mth_names[11] = "December";


	_day_count_arr[0] = 31;
	_day_count_arr[1] = 28;
	_day_count_arr[2] = 31;
	_day_count_arr[3] = 30;
	_day_count_arr[4] = 31;
	_day_count_arr[5] = 30;
	_day_count_arr[6] = 31;
	_day_count_arr[7] = 31;
	_day_count_arr[8] = 30;
	_day_count_arr[9] = 31;
	_day_count_arr[10] = 30;
	_day_count_arr[11] = 31;

	calculate_from_current_time();
	reset_menu_time();

	sig_handler.set_parent_object(this);
	sig_handler.add_signal(S_MenuDateChanged);
}

int TimeComponet::find_leap_year_count(int start_year, int end_year) const {
	int next_leap = start_year + (start_year % LEAP_FACTOR);
	int leap_count = (end_year - next_leap + LEAP_FACTOR) / LEAP_FACTOR;

	return leap_count;
}

bool TimeComponet::is_leap_month(int month, int year) const {
	if (year % LEAP_FACTOR == 0) {
		return true;	
	}

	return false;
}

int TimeComponet::month_to_int(const std::string& conv_month) const {
	int month_index = 0;

	for (int i=0; i<MONTH_COUNT; i++) {
		const std::string& month = _month_arr[i];	

		if (conv_month.compare(month) == 0) {
			month_index = i;
		}
	}

	return month_index;
}

int TimeComponet::weekday_to_int(const std::string& conv_day) const {
	int day_index = 0;

	for (int i=0; i<WEEKDAY_COUNT; i++) {
		const std::string& day = _weekday_arr[i];	

		if (conv_day.compare(day) == 0) {
			day_index = i;
		}
	}

	
	return day_index;
}

void TimeComponet::display_time_info() const {
	g_print (
	"Day: %d\nMonth: %d\nYear: %d\nWeekday: %d\nHour: %d\nMinute: %d\nSecond: %d", 
		_date.day, _date.month, _date.year, _weekday, _time.hour, _time.minute, _time.minute 
	);

	std::cout << _month_arr[3] << " this is month\n";	
}

void TimeComponet::calculate_from_current_time() {
	std::vector<std::string> time_vector;

	readable_time_to_vector(time_vector);


	int attrib_value = 0;

	for (int i=0; i<time_vector.size(); i++) {
		const std::string& str = time_vector.at(i);
		// do a specific action based on the current iteration
		switch (i) {
			case CTIME_DAY:
				this->_date.day = std::stoi(str);
				break;
			case CTIME_MONTH:
				this->_date.month = month_to_int(str);
				break;
			case CTIME_WEEKDAY:
				this->_weekday = weekday_to_int(str);
				break;
			case CTIME_TIME:
				break;
			case CTIME_YEAR:
				this->_date.year = std::stoi(str);
				break;
		}
	}
}

int TimeComponet::get_day_count() const {
	//this method will return the amount of days in a month
	if (_date.month >= MONTH_COUNT || _date.month < 0) {
		std::cout << "CRITICAL ERROR: MONTH IS OUT OF RANGE!!!" << "\n";
		return 0;
	}

	return _day_count_arr[_date.month];
}

int TimeComponet::get_day_count(int month) const {
	//this method will return the amount of days in a month
	if (month >= MONTH_COUNT || month < 0) {
		std::cout << "CRITICAL ERROR: MONTH IS OUT OF RANGE!!!" << "\n";
		return 0;
	}

	return _day_count_arr[month];
}

int TimeComponet::get_starting_weekday() const {
	return _weekday;
}

void TimeComponet::advance_menu_month(int cycle_count) {
	int year_cycle_count = 0;


	if (cycle_count > 0) {
		year_cycle_count = (_menu_date.month + cycle_count) / MONTH_COUNT;
	}
	else if (cycle_count < 0) {
		year_cycle_count = -(
			(TimeComponet::MONTH_COUNT - _menu_date.month - cycle_count - 1) /
			MONTH_COUNT
		);
	}
	else {
		return;
	}


	_menu_date.year = util::cycle_through_bounds(_menu_date.year, year_cycle_count, MIN_YEAR, MAX_YEAR);

	//menu_year = menu_year + (menu_month + cycle_count) / (MONTH_COUNT);
	_menu_date.month = util::cycle_through_bounds(_menu_date.month, cycle_count, 0, MONTH_COUNT);

	SigMenuDateChanged sig_data = {
		.date=_menu_date
	};

	sig_handler.emit_data(S_MenuDateChanged, &sig_data);
}

void TimeComponet::reset_menu_time() {
	_menu_date.month = _date.month;
	_menu_date.year = _date.year;
}

void TimeComponet::set_menu_day(int day) {
	_menu_date.day = day;

	SigMenuDateChanged sig_data = {
		.date=_menu_date
	};

	sig_handler.emit_data(S_MenuDateChanged, &sig_data);

}

void TimeComponet::set_menu_month(int month) {
	_menu_date.month = month;

	SigMenuDateChanged sig_data = {
		.date=_menu_date
	};

	sig_handler.emit_data(S_MenuDateChanged, &sig_data);

}

void TimeComponet::set_menu_year(int year) {
	_menu_date.year = year;
	
	SigMenuDateChanged sig_data = {
		.date=_menu_date
	};

	sig_handler.emit_data(S_MenuDateChanged, &sig_data);

}

void TimeComponet::set_menu_date(Date& date) {
	this->_menu_date.day = date.day;
	this->_menu_date.month = date.month;
	this->_menu_date.year = date.year;
}

const std::string& TimeComponet::get_long_month_name(int month) const {
	//use moduls here as it allows us to limit the month without
	//performing a check each time the month is changed
	return _long_mth_names[month % MONTH_COUNT];
}

const std::string& TimeComponet::get_short_month_name(int month) const {
	return _month_arr[month % MONTH_COUNT];
}

const std::string& TimeComponet::get_weekday_name(int weekday) const {
	return _weekday_arr[weekday % WEEKDAY_COUNT];
}

const core::Date& TimeComponet::get_date() const {
	return _date;
}

int TimeComponet::get_day() const {
	return _date.day;
}

int TimeComponet::get_month() const {
	return _date.month;
}

int TimeComponet::get_year() const {
	return _date.year;
}

const Date& TimeComponet::get_menu_date() const {
	return _menu_date;		
}

int TimeComponet::get_menu_day() const {
	return _menu_date.day;
}

int TimeComponet::get_menu_month() const {
	return _menu_date.month;
}

int TimeComponet::get_menu_year() const {
	return _menu_date.year;
}



const std::string* TimeComponet::get_weekday_arr() const {
	return _weekday_arr;
}

void TimeComponet::get_month_year_str(const core::Date& date, std::string& output_str) const {
	output_str = _long_mth_names[date.month] +  " " + std::to_string(date.year);
}

void TimeComponet::get_full_day_str(core::Date& date, std::string& output_str) const {
	const std::string& RD_ENDING = "rd";			
	const std::string& TH_ENDING = "th";			
	const std::string& ST_ENDING = "st";			
	const std::string& ND_ENDING = "nd";			

	int day_last_num = date.day % 10;

	std::string day_suffix = "";

	if (date.day > 3 and date.day < 21) {
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

	const std::string& month_str = _long_mth_names[date.month];
	const std::string& day_str = std::to_string(date.day) + day_suffix;

	output_str = month_str + " " + day_str + ", " + std::to_string(date.year);
}

int TimeComponet::get_starting_weekday(int target_month, int target_year) const {
	//this meathod will return the weekday that the month starts on

	int years_ahead = target_year - _date.year;
	int months_ahead = years_ahead * MONTH_COUNT + target_month - _date.month;
	int days_to_cycle = -(_date.day-1); //converts to index format and sets it so it will loop back to day zero
	int leap_year_count = 0;
	int cycle_dir = 1;

	//std::cout << "Starting days to cycle: " << days_to_cycle << "\n";
	int current_month = 0;

	if (months_ahead < 0) {
		cycle_dir = -1;
	}


	//calculate the amount of days to cycle (without leap years)
	for (int i=0; i<abs(months_ahead); i++) {
		current_month = util::cycle_through_bounds(_date.month, i*cycle_dir, 0, MONTH_COUNT);
		
		if (cycle_dir < 0 ) {
			//if cycle_dir < 0, decrement current_month to access
			//the day count of the previous month
			current_month = util::cycle_through_bounds(current_month, -1, 0, MONTH_COUNT);
		}

		days_to_cycle += _day_count_arr[current_month] * cycle_dir;
	}

	//account for leap years
	leap_year_count = find_leap_year_count(_date.year, target_year);
	days_to_cycle += leap_year_count;

	//if the target month is before march don't account for 
	//febuary's leap year
	if (target_month < 2 and leap_year_count > 0) {
		days_to_cycle--;
	}


	int result = util::cycle_through_bounds(_weekday, days_to_cycle, 0, WEEKDAY_COUNT);

	return result;
}
}
