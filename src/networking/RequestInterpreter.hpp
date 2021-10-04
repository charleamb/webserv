#ifndef REQUESTINTERPRETER_HPP
# define REQUESTINTERPRETER_HPP

# include "Client.hpp"
# include "CGI.hpp"
# include "Response.hpp"
# include "../config/ConfigServer.hpp"
// # include <ctype.h>
# include "../utils/utils.hpp"
# include "../utils/Base64.hpp"

# define PATH_TO_ERROR_PAGE_TEMPLATE "../src/errors/error_page_template.html"
# define PHP_CGI_PATH "/usr/bin/php-cgi"
# include <sys/wait.h>
# include <dirent.h>

class RequestInterpreter {	
	private:
		Request		*_request;
		ConfigServer		*_config;
		const Location		*_location;
		fd_set				*_masterfd;
		Response			_response;
		std::string			_path_to_error_page;
		int					_cgi_output_fd;
		int					_cgi_input_fd;
		int					*_fd_max;
		std::map<int, Response *> *_cgi_output;
		std::map<int, Response *> *_cgi_input;

		void				valid_syntax(void) const;
		void				is_method_allowed(void);
		void				is_authorized(void);
		void				is_allowed_body_size(void) const;
		void				generate_error_page(int code);
		void				set_default_error_page(void);	
		bool				authenticate(const std::string &authorizationField) const;
		void				call_post();
		void				call_get();
		void				call_delete();
		void				call_methods(void);
		void				directory_listing(void);
		void				php_cgi();

		void				convert_path();


		void				get_file(const std::string &path);
	public:
		
		void				interpret(void);

		RequestInterpreter(Client *client, std::map<int, Response *> *CGI_output, std::map<int, Response *> *CGI_input, fd_set *masterfd, int *fdmax);
		RequestInterpreter &operator=(RequestInterpreter &x);
		RequestInterpreter();
		void swap(RequestInterpreter &x);
		~RequestInterpreter();
	
	class RequestInterpreterException : public std::exception {
	private:
		int						_code;
		std::string				_msg;
	public:
		RequestInterpreterException(int code) throw();
		RequestInterpreterException(int code, const std::string str) throw();
		RequestInterpreterException(const std::string &msg) throw();
		int get_code(void) const;
		virtual ~RequestInterpreterException() throw();
		virtual const char *what() const throw();
	};

};
#endif
