//
// Created by alena on 15/07/2021.
//

#include "Scheme.hpp"

slice	from_char_slice(const slice&input, ParserResult<tuple<char, slice> >& res)
{
	return input.take(1 + res.unwrap().second.size);
}
int scheme_charset(int c) { return isalnum(c) || c == '+' || c == '-' || c == '.'; }

/*
 * scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
 */
Scheme::Scheme(){ }
Scheme::result_type	Scheme::operator()(const slice &input)
{
	return sequence(alpha, TakeWhile(scheme_charset, true))(input)
			.map(input, from_char_slice);
}

//TODO REMOVE FROM ?