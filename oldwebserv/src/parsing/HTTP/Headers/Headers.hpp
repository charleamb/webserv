//
// Created by alena on 24/06/2021.
//

#ifndef WEBSERV_HEADERS_HPP
#define WEBSERV_HEADERS_HPP

#include "parser/combinators.hpp"
#include "Header.hpp"
#include "HeaderField.hpp"
#include "ContentLength.hpp"
#include "Date.hpp"

static const Alt<ContentLength, Date, HeaderField>	HEADERS = alt(ContentLength(), Date(), HEADER_FIELD);

//TODO more headers...

#endif //WEBSERV_HEADERS_HPP
