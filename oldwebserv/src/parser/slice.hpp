//
// Created by alena on 17/06/2021.
//

#ifndef WEBSERV_SLICE_HPP
#define WEBSERV_SLICE_HPP

#include <string>
#include <cstddef>
#include <ostream>
#include <iostream>

/*
 * Allows to convert std::string in to slice, for practicality reasons such as preventing deep copy
 */

struct slice
{
public:
	const char	*p;
	size_t		size;

	slice(): p(0), size(0) { }
	slice(const char *str, size_t sz): p(str), size(sz) { }
	slice(const void *data, size_t sz): p(reinterpret_cast<const char *>(data)), size(sz) { }
	slice(const std::string &str): p(str.data()), size(str.length()) { }
	slice(const slice &other): p(other.p), size(other.size) { }

/*
 * Returns a portion of this slice that's at most n bytes
 * "hello world".take(5) => "hello"
 */
	slice	take(size_t n) const
	{
		return slice(p, std::min(n, size));
	}
/*
 * Returns the portion of this slice starting at byte n
 * "hello world".from(5) => " world"
 */
	slice	from(size_t n) const
	{
		return slice(p + n, size - n);
	}
/*
 * convert to slice in to string
 */
	std::string 	to_string()
	{
		return std::string(p, size);
	}

	static std::string	to_string_static(slice input)
	{
		return input.to_string();
	}

	std::string		to_string() const
	{
		return std::string(p, size);
	}

	int				to_int(int base = 10) const
	{
		static const std::string	CHARSET = "0123456789abcdef";
		int 	ret = 0;

		for (size_t i = 0; i < size; i++)
		{
			ret *= base;
			ret += CHARSET.find(std::tolower(p[i]));
		}
		return ret;
	}
/*
 * Checks to see if this slice contains the given string
 * "hello world".contains("hello") => 0
 */
	int		contains(const std::string &other) const
	{
		size_t	len = other.length();

		if (len > size)
			return -1;
		const char *s = other.c_str();
		const char *self = p;
		while (*s && *self && len--)
		{
			if (*s != *self)
				return -1;
			s++;
			self++;
		}
		return 0;
	}
/*
 * contains, but case insensitive
 */
	int		icontains(const std::string &other) const
	{
		size_t	len = other.length();

		if (len > size)
			return -1;
		const char *s = other.c_str();
		const char *self = p;
		while (*s && *self && len--) {
			if (std::tolower(*s) != std::tolower(*self))
				return -1;
			s++;
			self++;
		}
		return 0;
	}

	bool 		operator==(const char *str) const
	{
		std::string 	s(str);

		if (s.length() != this->size)
			return false;
		return !this->icontains(s);
	}

	bool		operator==(const slice &other)
	{
		return other.p == p && other.size == size;
	}

	bool		operator!=(const slice &other)
	{
		return !operator==(other);
	}

	friend		std::ostream	&operator<<(std::ostream &stream, const slice &slice)
	{
		stream.write(slice.p, slice.size);
		return stream;
	}
};

#endif //WEBSERV_SLICE_HPP
