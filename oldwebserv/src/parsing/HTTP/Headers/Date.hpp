//
// Created by alena on 25/06/2021.
//

#ifndef WEBSERV_DATE_HPP
#define WEBSERV_DATE_HPP

#include "Result.hpp"
#include "Parser.hpp"
#include "Header.hpp"
#include "Tokens.hpp"

const TakeWhile	TWO_DIGIT = TakeWhile(std::isdigit, 2, 2);
const TakeWhile	FOUR_DIGIT = TakeWhile(std::isdigit, 4, 4);

/*
 * Wkday = "Mon" | "Tue" | "Wed" | "Thu" | "Fri" | "Sat" | "Sun"
 */
class Wkday : public Parser<int>
{
public:
	Wkday() { }
	result_type operator()(const slice &input)
	{
		static const std::string DAYS[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
		ParserResult<slice> res = alt(Tag("Mon"), Tag("Tue"), Tag("Wed"), Tag("Thu"), Tag("Fri"), Tag("Sat"), Tag("Sun"))(input);
		if (res.is_err())
			return res.convert<int>();
		std::string	m = res.unwrap().to_string();
		for (int i = 0; i < 7; i++)
			if (DAYS[i] == m)
				return result_type::ok(res.left(), i);
		return result_type::err(res.left(), failure("Unknown weekday"));
	}
};

/*
 * Weekday = "Monday" | "Tuesday" | "Wednesday" | "Thursday" | "Friday" | "Saturday" | "Sunday"
 */
class Weekday : public Parser<int>
{
public:
	Weekday() { }
	result_type operator()(const slice &input)
	{
		static const std::string DAYS[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
		ParserResult<slice> res = alt(Tag("Monday"), Tag("Tuesday"), Tag("Wednesday"), Tag("Thursday"), Tag("Friday"), Tag("Saturday"), Tag("Sunday"))(input);
		if (res.is_err())
			return res.convert<int>();
		std::string	m = res.unwrap().to_string();
		for (int i = 0; i < 7; i++)
			if (DAYS[i] == m)
				return result_type::ok(res.left(), i);
		return result_type::err(res.left(), failure("Unknown weekday"));
	}
};

/*
 * Month = "Jan" | "Feb" | "Mar" | "Apr" | "May" | "Jun" | "Jul" | "Aug" | "Sep" | "Oct" | "Nov" | "Dec"
 */
class Month : public Parser<int>
{

public:
	Month() { }
	result_type operator()(const slice &input)
	{
		static const std::string MONTHS[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
		ParserResult<slice>	res = alt(Tag("Jan"), Tag("Feb"), Tag("Mar"), Tag("Apr"), Tag("May"),
				   Tag("Jun"), Tag("Jul"), alt(Tag("Aug"), Tag("Sep"),
				   Tag("Oct"), Tag("Nov"), Tag("Dec")))(input);
		if (res.is_err())
			return res.convert<int>();
		std::string	m = res.unwrap().to_string();
		for (int i = 0; i < 12; i++)
			if (MONTHS[i] == m)
				return result_type::ok(res.left(), i);
		return result_type::err(res.left(), failure("Unknown month"));
	}
};

/*
 * Time = 2DIGIT ":" 2DIGIT ":" 2DIGIT
 */
class Time : public Parser<tuple<slice, slice, slice> >
{
public:
	Time() { }
	result_type operator()(const slice &input)
	{
		return sequence(terminated(TWO_DIGIT, Char(':')), terminated(TWO_DIGIT, Char(':')), TWO_DIGIT)(input);
	}
};
/*
 * Date1 = 2DIGIT SP month SP 4DIGIT
 */
class Date1 : public Parser<tuple<slice, int, slice> >
{
public:
	Date1() { }
	result_type operator()(const slice &input)
	{
		return sequence(terminated(TWO_DIGIT, single_space), terminated(Month(), single_space), FOUR_DIGIT)(input);
	}
};

/*
 * Date2 =  2DIGIT "-" month "-" 2DIGIT
 */
class Date2 : public Parser<tuple<slice, int, slice> >
{
public:
	Date2() { }
	result_type operator()(const slice &input)
	{
		return sequence(terminated(TWO_DIGIT, Char('-')), Month(), preceded(Char('-'), FOUR_DIGIT))(input);
	}
};

/*
 *  Date3 = Month SP ( 2DIGIT | ( SP 1DIGIT ))
 */
class Date3 : public Parser<tuple<int, slice> >
{
public:
	Date3() { }
	result_type operator()(const slice &input)
	{
		return sequence(terminated(Month(), single_space), alt(TWO_DIGIT, preceded(single_space, as_slice(digit))))(input);
	}
};

struct 	tm	from_date1(tuple<int, tuple<slice, int, slice>, tuple<slice, slice, slice> > date);
struct 	tm	from_date2(tuple<int, tuple<slice, int, slice>, tuple<slice, slice, slice> > date);
struct 	tm	from_date3(tuple<int, tuple< int, slice>, tuple<slice, slice, slice>, slice > date);
std::string	now();

/*
 * Rfc1123Date = wkday "," SP Date1 SP Time SP "GMT"
 */
class Rfc1123Date : public Parser<struct tm>
{
public:
	Rfc1123Date() { }
	result_type operator()(const slice &input)
	{
		return map(sequence(terminated(Wkday(), Char(',')), preceded(single_space, Date1()),
			preceded(single_space, terminated(Time(), Tag(" GMT")))), from_date1)(input);
	}
};

/*
 *  Rfc850Date  = weekday "," SP Date2 SP Time SP "GMT"
 */
class Rfc850Date : public Parser<struct tm>
{
public:
	Rfc850Date() { }
	result_type operator()(const slice &input)
	{
		return map(sequence(terminated(Weekday(), Char(',')), preceded(single_space, Date2()),
					preceded(single_space, terminated(Time(), Tag(" GMT")))), from_date2)(input);
	}
};

/*
 *  AsctimeDate = wkday SP date3 SP time SP 4DIGIT
 */
class AsctimeDate : public Parser<struct tm>
{
public:
	AsctimeDate() { }
	result_type operator()(const slice &input)
	{
		return map(sequence(terminated(Wkday(), Char(',')), preceded(single_space, Date3()), preceded(single_space, Time()),
							preceded(single_space, FOUR_DIGIT)), from_date3)(input);
	}
};

/*
 * Date  = "Date" ":" HTTP-date
 * HTTP-date = Rfc1123Date | Rfc850Date | AsctimeDate
 */
class Date : public Parser<Header>
{
public:
	Date() { }
	result_type operator()(const slice &input)
	{
		result_type res = map(separated(Tag("Date"), terminated(preceded(ows, Char(':')), ows),
		terminated(as_slice(alt(Rfc1123Date(), Rfc850Date(), AsctimeDate())), ows)), Header::Date)(input);
		return res;
	}
};

#endif //WEBSERV_DATE_HPP
