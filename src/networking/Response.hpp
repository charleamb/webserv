#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
# include <map>
# include <fstream>
# include <sys/stat.h>
# include <iostream>

# include "../config/ConfigServer.hpp"
# include "../utils/utils.hpp"
# include "ResponseHeader.hpp"
# include "StatusCode.hpp"
//# include "Request.hpp"

# define DEFAULT_ERROR_PAGES "errors/"

class Request;

enum Response_Status { Ready, Waiting_CGI_Input, Waiting_CGI_Output };

class Response {
	friend class StatusCode;
	friend class ResponseHeader;
	
	private:
		std::string			_status_msg;
		ResponseHeader		_headers;

	public:
		int					offset;
		std::string			buffer;
		int					size;
		enum Response_Status status;
		int					code;
		std::string			body;

		std::string			request_body;
		pid_t				cgi_child;
		int					cgi_input_offset;

		~Response();
		Response();
		Response(int code);
		Response &operator=(const Response &response);
		Response(const Response &response);

		void 			print(void);
		void 			set_content(const std::string &filename);
		void			set_default_error_page(void);
		void			format(std::map<std::string, std::string> *map);
		ResponseHeader  &get_headers(void);
		void			insert_header(const std::string &key, const std::string &value);
		bool			has_body();
};
#endif
