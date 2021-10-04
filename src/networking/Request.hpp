#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "Client.hpp"

#include <map>
#include <string>
#include "../utils/utils.hpp"
#define GET 1
#define POST 2
#define DELETE 3
#define UNKNOWN 0
class Client;

enum Status {
		Waiting,
		StartLineParsed,
		HeadersParsed,
		BodyParsed,
		Error,
		Complete,
};

class Request
{
public:

    //TODO: Add request timer

	void									check_request();
	void									parse_start_line();
	void									parse_headers();
	void									parse_body();
	void									parse_in_chunk();
	void									parse();
	Status									status;
	std::string								query_string;
	std::string								host;
    // std::string                    			content_type;
	size_t									buffer_size;
	std::string								buffer;
	std::string								method;
	std::string								path;
	std::string								real_path;
	std::string								http_version;
	std::map<std::string , std::string>		headers;
	std::string								body;

	std::string								&get_content_type();
    std::pair<bool, std::string>			env_querystring() const ;
    std::pair<bool, std::string>			env_method() const;
    std::pair<bool, std::string>			env_content_type();
    // std::pair<bool, std::string>			env_content_length();

	int     								get_content_length() const;

	void parse(Client &client, char *buf);
	void Querydecoding();
    Request();
	void swap(Request &x);
	Request(Request &x);
	Request &operator=(Request &x);
	// void	putvalue(Request request);
	void	printvalue();
    ~Request();
};

#endif
