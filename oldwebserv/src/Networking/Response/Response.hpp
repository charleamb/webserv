#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "../../parsing/HTTP/Request.hpp"
# include "../Status/Status.hpp"
# include "../Utils/utils.hpp"
# include <string>
# include <fstream>
# include <sstream>
# include "ResponseHeader.hpp"
namespace ft {
	class Response {
	private:
		status::Status						_status;
		int									_code;
		ResponseHeader						_headers;
		std::string							_body;

		void								write_startline(void);
		void								write_headers(void);
		void								write_response(void);
	public:
		int									_offset;
		std::string							_buffer;

		~Response();
		Response();
		Response(Response &x);
		Response &operator=(Response &x);
		// Response(const Request &req);

		const std::string					&get_buffer(void) const;
		const int							&get_offset(void) const;
		const status::Status				&get_status(void) const;
		const std::string					&get_body(void) const;
		ResponseHeader						&get_headers(void);
		int									get_code(void) const;

		void								set_offset(int offset);
		void								set_headers(void);
		void								set_status(const status::Status &status);
		const status::Status 				&update_status(void);

		// void								delete_method(const Request &req);
		// void								post_method(const Request &req);
		// void								get_method(const Request &req);


		std::string 						read_error_page(std::string path);
	}; // end of Response class
} // end of namespace
#endif