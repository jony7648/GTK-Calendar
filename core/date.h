#pragma once
namespace core {
struct Date {
	bool operator==(Date& date);
	int day = 0, month = 0, year = 0;
};

struct Time {
	int second = 0, minute = 0, hour = 0;
};
}

