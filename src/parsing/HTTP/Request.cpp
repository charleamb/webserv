//
// Created by alena on 14/07/2021.
//

#include "Request.hpp"

/*
 * Request
 */
Request::Request() { };

Request::Request(methods::s_method method, Target target, Version version): _method(method), _target(target), _version(version) { }

void	Request::header(const Header& header)
{
	this->_headers.insert(std::make_pair(header.name(), header));
}

std::string	Request::redirect_uri(const redirect &base) { return base.uri; }

Result<std::string>		Request::get_header(const std::string &name)
{
	std::map<std::string, Header>::iterator it = this->_headers.find(name);
	if (it == this->_headers.end())
		return Result<std::string>::err(DefaultError("Unknown cgi"));
	return Result<std::string>::ok(it->second.value());
}

methods::s_method Request::get_method() { return _method; }

Target Request::get_target() { return _target; }

std::ostream &operator<<(std::ostream & stream, const Request &req)
{
	stream << req._method << " " << req._target << " HTTP/" << req._version;
	return stream;
}

/*
 * RequestParser
 */
RequestParser::RequestParser() { }

RequestParser::result_type	RequestParser::operator()(const slice& input)
{
	ParserResult<tuple<methods::s_method, Target, Version> > line = RequestLine()(input);
	Request		req;
	if (line.is_ok())
	{
		tuple<methods::s_method, Target, Version>	info = line.unwrap();
		req = Request(info.first, info.second, info.third);
	}
	else {
		return result_type::err(line.left(), error("Failed to parse request line"));
	}
	ParserResult<std::vector<Header> >	headers = many(terminated(HEADERS, newline))(line.left());
	std::cout << "****Parsing****" << std::endl << req << std::endl;
	if (headers.is_ok())
	{
		std::vector<Header>	&v = headers.unwrap();
		for (std::vector<Header>::iterator it = v.begin(); it != v.end(); it++)
		{
			req.header(*it);
			std::cout << *it << std::endl;
		}
	}
	else
		std::cerr << headers.unwrap_err() << std::endl;
	return Newline()(headers.left()).map(req);
	// TODO if this is not here, either the request is incomplete or ill-formatted
}

/*
 * RequestHandler
 */

RequestHandler::RequestHandler() : _transfer_type(transfer::Unset), _status(status::Incomplete), _req(Result<Request>::err(DefaultError(""))) {
	_body.reserve(512);
}

status::Status	RequestHandler::update(const char *buff, size_t read)
{
	//TODO check content length / transfer encoding and store it
	if (_status == status::Complete) // TODO handle zero size transfer (connection closed) if chunked transfer. else error
		return _status;
	size_t offset = _body.size();
	_body.reserve(offset + read);
	std::vector<char>::iterator off = _body.begin() + offset;
	_body.insert(off, buff, buff + read);
	switch (_status) {
		case status::Incomplete:
			return parse();
		default:
			break;
	}
	return _status;
}

void		RequestHandler::reset()
{
	// TODO if the request is complete, keep the rest of the body instead of trashing it
	_body.clear();
	_transfer_type = transfer::Unset;
	_status = status::Incomplete;
	_req = Result<Request>::err(DefaultError(""));
}

/*
 * Private fonction
 */
status::Status		RequestHandler::parse()
{
	ParserResult<Request>	req = RequestParser()(slice(_body.data(), _body.size()));
	if (req.is_ok())
	{
		Request	r = req.unwrap();
		_req = Result<Request>::ok(r);
		std::cout << "parsed: " << (req.left().p - _body.data()) << std::endl;
		std::vector<char>::iterator		end = _body.begin() + (req.left().p - _body.data());
		_body.erase(_body.begin(), end);
		std::cout << "left: [" << slice(_body.data(), req.left().size) << "]" << std::endl;
	}
	return _status;
}
