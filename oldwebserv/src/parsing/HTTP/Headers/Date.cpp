//
// Created by alena on 25/06/2021.
//
#include "Date.hpp"
#include <cstring>

struct 	tm		from_date1(tuple<int, tuple<slice, int, slice>, tuple<slice, slice, slice> > date)
{
	struct tm	d;

	bzero(&d, sizeof(struct tm));
	d.tm_hour = date.third.first.to_int();
	d.tm_min = date.third.second.to_int();
	d.tm_sec = date.third.third.to_int();
	d.tm_mday = date.second.first.to_int();
	d.tm_wday = date.first;
	d.tm_mon = date.second.second;
	d.tm_year = date.second.third.to_int() - 1900;
	return d;
}

struct 	tm		from_date2(tuple<int, tuple<slice, int, slice>, tuple<slice, slice, slice> > date)
{
	struct tm	d;

	bzero(&d, sizeof(struct tm));
	d.tm_hour = date.third.first.to_int();
	d.tm_min = date.third.second.to_int();
	d.tm_sec = date.third.third.to_int();
	d.tm_mday = date.second.first.to_int();
	d.tm_wday = date.first;
	d.tm_mon = date.second.second;
	int y = date.second.third.to_int();
	if (y < 100)
		y += 2000;
	if (y > 2021)
		y -= 100;
	d.tm_year = y - 1900;
	return d;
}

struct 	tm		from_date3(tuple<int, tuple< int, slice>, tuple<slice, slice, slice>, slice > date)
{
	struct tm	d;

	bzero(&d, sizeof(struct tm));
	d.tm_hour = date.third.first.to_int();
	d.tm_min = date.third.second.to_int();
	d.tm_sec = date.third.third.to_int();
	d.tm_mday = date.second.second.to_int();
	d.tm_wday = date.first;
	d.tm_mon = date.second.first;
	d.tm_year = date.fourth.to_int() - 1900;
	return d;
}

std::string		now()
{
	time_t 	t = time(0);
	struct tm d = *gmtime(&t);
	char buff[64];
	return std::string(buff, strftime(buff, 64, "%a, %m %b %Y %X GMT", &d));
}
