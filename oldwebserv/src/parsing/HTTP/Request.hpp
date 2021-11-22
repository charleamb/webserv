//
// Created by alena on 14/06/2021.
//

#ifndef WEBSERV_REQUEST_HPP
#define WEBSERV_REQUEST_HPP

#include "parser/export.hpp"
#include "Tokens.hpp"

#include "Request/RequestLine.hpp"


#include "HTTP/Headers/Headers.hpp"

#include "Config/Module/Redirect.hpp"
#include <map>

/*
 * Request
 */
class Request
{
private:
	methods::s_method						_method;
	Target									_target;
	Version									_version;
	std::map<std::string, Header>			_headers;

public:
	Request();
 
	Request(methods::s_method method, Target target, Version version);

	void	header(const Header& header);

	//TODO parse redirect variables
	std::string			redirect_uri(const redirect& base);

	Result<std::string>	get_header(const std::string& name);
	methods::s_method	get_method();
	Target				get_target();

	//TODO check end of body


	friend std::ostream &operator<<(std::ostream & stream, const Request &req);
};

/*
 * RequestParser
 */
class RequestParser: public Parser<Request>
{
public:
	RequestParser();

	result_type operator()(const slice& input);
};

/*
 * RequestHandler
 */
namespace status
{
	enum Status
			{
		Incomplete,		// Waiting for the request to make sense
		Waiting,		// Waiting on full body
		Complete,		// Ready to respond
		Error			// Error occured: Early close, Parse error
	};
}

namespace transfer
{
	enum TransferType
			{
		Unset,			// None yet
		Identity,		// Content-Length
		Chunked			// Transfer-Encoding
	};
}

//TODO mettre dans le cpp
class RequestHandler
{
private:
	transfer::TransferType	_transfer_type;
	status::Status			_status;
	Result<Request>			_req;
	std::vector<char>		_body;

	status::Status		parse();

public:
	RequestHandler();

	status::Status	update(const char *buff, size_t read);

	void		reset();
};


#endif //WEBSERV_REQUEST_HPP
