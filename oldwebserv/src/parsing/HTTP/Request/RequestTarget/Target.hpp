//
// Created by alena on 20/06/2021.
//

#ifndef WEBSERV_TARGET_HPP
#define WEBSERV_TARGET_HPP
#include "Segment.hpp"

struct Target
{

public:
	std::string		scheme;
	std::string		path;
	std::string		query;
	std::string 	decoded_path;
	std::string 	decoded_query;

private:

	static std::string 	decode(std::string input)
	{
		std::string		out;

		out.reserve(input.length());
		for (size_t i = 0; i < input.length(); i++) {
			switch (input[i]) {
				case '%':
					out += static_cast<char>(
							HEXBASE.find(std::toupper(input[i + 1])) << 4 |
							HEXBASE.find(std::toupper(input[i + 2])));
					i += 2;
					break;
				default:
					out += input[i];
			}
		}
		return out;
	}

public:

	Target(): scheme(""), path(""), query(""), decoded_path("") { }
	Target(slice path, slice query): scheme("http"), path(path.to_string()), query(query.to_string()) {
		decoded_path = decode(this->path);
		decoded_query = decode(this->query);
	}

	static Target from(slice path, slice query)
	{
		return Target(path, query);
	}

	friend std::ostream&	operator<<(std::ostream& stream, const Target& target)
	{
		stream << "url encoded: " << target.decoded_path << "?" << target.decoded_query << std::endl;
		stream << target.path;
		if (!target.query.empty())
			stream << "?" << target.query;
		return stream;
	}
};

#endif //WEBSERV_TARGET_HPP
