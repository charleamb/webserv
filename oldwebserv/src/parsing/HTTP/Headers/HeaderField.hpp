//
// Created by alena on 23/06/2021.
//

#ifndef WEBSERV_HEADERFIELD_HPP
#define WEBSERV_HEADERFIELD_HPP

#include "Parser.hpp"
#include "Tokens.hpp"
#include "Header.hpp"

/*
 * FieldName = token = 1 * tchar = "!" | "#" | "$" | "%" | "&" | "'" | "*"| "+" | "-" | "." | "^" | "_" | "`" | "|" | "~" | DIGIT | ALPHA
 */

static const OneOf charset = OneOf("!#$%&'*+-.^_`|~");

class FieldName : public Parser<slice>
{
public:
	FieldName() { }
	result_type operator()(const slice &input)
	{
		return take_with(alt(charset, digit, alpha))(input);
	}
};

/*
 * FieldValue  = *(field-vchar [ 1*( SP / HTAB ) field-vchar ])
 */

class FieldValue : public Parser<slice>
{
public:
	FieldValue() { }
	result_type operator()(const slice &input)
	{
		static const Alt<Match, HexChar> vchar = alt(Match(isprint), HexChar('\x80', '\xFF'));
		ParserResult<slice> res = take_with(sequence(vchar, opt(sequence(take_with(OneOf(" \t")), vchar))))(input);
		return res;
	}
};

/*
 * HeaderField = FieldName ":" OWS FieldValue OWS
 */

class HeaderField : public Parser<Header>
{
public:

	HeaderField() { }
	result_type operator()(const slice &input)
	{
		ParserResult<Header> res = map(separated(FieldName(), Char(':'),
		preceded(ows, terminated(FieldValue(), ows))), Header::Other)(input);
		return res;
	}
};

const HeaderField HEADER_FIELD = HeaderField();

#endif //WEBSERV_HEADERFIELD_HPP
