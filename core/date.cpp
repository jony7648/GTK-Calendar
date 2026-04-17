#include "date.h"

bool core::Date::operator==(Date& date) {	
	return day == date.day && month == date.month && year == date.year;
}
