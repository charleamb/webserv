//
// Created by alena on 17/06/2021.
//

#ifndef WEBSERV_ERROR_HPP
#define WEBSERV_ERROR_HPP

#include <vector>
#include <string>
#include <iostream>
#include "Constants.hpp"
#include "slice.hpp"
#include "utils.hpp"

template<typename Data = Empty>
class Error
{
public:
	enum ErrorKind
	{
		KIND_ERROR,
		KIND_FAILURE
	};

private:
	Data										_data;
	ErrorKind									_kind;
	std::string 								_msg;
	std::vector<std::pair<slice, std::string> >	_stack;

	typedef std::vector<std::pair<slice, std::string> >::const_iterator iterator;

public:

	Error(): _kind(KIND_ERROR), _msg("No info") { }
	explicit Error(std::string msg, ErrorKind kind = KIND_ERROR): _kind(kind), _msg(msg), _stack() { }
	explicit Error(std::string msg, Data data, ErrorKind kind = KIND_ERROR): _data(data), _kind(kind), _msg(msg), _stack() { }

	Error(const Error& other)
	{
		if (&other == this)
			return ;
		this->_kind = other._kind;
		this->_msg = other._msg;
		this->_stack = other._stack;
	}

	Data	content() const {
		return this->_data;
	}

	ErrorKind	kind() const
	{
		return this->_kind;
	}

	static Error Failure(std::string msg)
	{
		return Error(msg, KIND_FAILURE);
	}

	/*
	 * Insert a new element in the stacktrace
	 */
	Error		at(const slice &where, std::string info)
	{
		this->_stack.push_back(std::make_pair(where, info));
		return *this;
	}

	friend std::ostream	&operator<<(std::ostream &stream, const Error &err)
	{
		stream << YELLOW << err._msg << NC << std::endl;
#if LOG_LEVEL > 0
		err.trace();
#endif
		return stream;
	}

	void 		trace() const
	{
		for (Error::iterator it = _stack.begin(); it != _stack.end(); it++) {
			std::cerr << RED << "\tat: " << it->first.take(10) << " (" << it->second << ")" << NC << std::endl;
		}
	}

	void 		to_failure()
	{
		_kind = KIND_FAILURE;
	}
};

Error<Empty>	error(std::string msg);
Error<Empty>	failure(std::string msg);

#endif //WEBSERV_ERROR_HPP
