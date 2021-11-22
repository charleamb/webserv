//
// Created by alena on 24/06/2021.
//

#ifndef WEBSERV_CONTENTLENGTH_HPP
#define WEBSERV_CONTENTLENGTH_HPP

#include "Header.hpp"

/*
 * Content-Length = "Content-Length" ":" 1*DIGIT
 */

class ContentLength : public Parser<Header>
{
public:
	ContentLength() { }
	result_type operator()(const slice &input)
	{
		return map(separated(
				terminated(Tag("Content-Length"), Char(':')), ows,
				terminated(TakeWhile(std::isdigit), ows)), Header::ContentLength)(input);
	}
};

#endif //WEBSERV_CONTENTLENGTH_HPP
