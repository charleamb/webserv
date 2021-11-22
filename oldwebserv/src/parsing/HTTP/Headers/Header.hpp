//
// Created by alena on 24/06/2021.
//

#ifndef WEBSERV_HEADER_HPP
#define WEBSERV_HEADER_HPP

#include "slice.hpp"
#include "Error.hpp"

class Header
{
public:
	enum HeaderKind {
		DATE,
		CONTENT_LENGTH,
		HOST,
		OTHER
	};

private:
	slice		_name;
	slice		_value;
	HeaderKind	_kind;

public:
	Header(slice name, slice value, HeaderKind kind = OTHER): _name(name), _value(value), _kind(kind) { }

	Header static ContentLength(tuple<slice, slice> data) { return Header(data.first, data.second, CONTENT_LENGTH); }
	Header static Date(tuple<slice, slice> data) { return Header(data.first, data.second, DATE); }
	Header static Other(tuple<slice, slice> data) { return Header(data.first, data.second, OTHER); }

	std::string	value() { return this->_value.to_string(); }
	std::string	value() const { return this->_value.to_string(); }

	std::string	name() { return this->_name.to_string(); }
	std::string	name() const { return this->_name.to_string(); }

	HeaderKind	kind() { return this->_kind; }

	friend std::ostream &operator<<(std::ostream& stream, const Header& header)
	{
		stream << "(" << header._kind << ") " << header._name.to_string() << " : " << header._value.to_string();
		return stream;
	}
};

#endif //WEBSERV_HEADERS_HPP
