//
// Created by alena on 15/07/2021.
//

#ifndef WEBSERV_METHODS_HPP
#define WEBSERV_METHODS_HPP

#include "slice.hpp"

namespace methods
{
	enum	s_method
	{
		GET = 1,
		POST = 2,
		DELETE = 4
	};

	s_method	as_get(slice input);
	s_method	as_post(slice input);
	s_method	as_delete(slice input);

	struct Methods
	{
	private:
		unsigned int	_raw;
	public:
		Methods(s_method method): _raw(method) { }

		static Methods	all()
		{
			return Methods(GET).add_method(POST).add_method(DELETE);
		}

		Methods	&add_method(s_method method) { _raw |= method; return *this; }

		bool	has(s_method method) const { return (_raw & method) != 0; }

		friend std::ostream	&operator<<(std::ostream& stream, const Methods& m)
		{
			if (m.has(GET)) {stream << "GET "; }
			if (m.has(POST)) {stream << "POST "; }
			if (m.has(DELETE)) {stream << "DELETE "; }
			return stream;
		}
	};
}

#endif //WEBSERV_METHODS_HPP
