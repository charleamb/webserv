#ifndef RESPONSEHEADER_HPP
# define RESPONSEHEADER_HPP
# include <utility>
# include <vector>
# include "../ft_networking.hpp"
# include <ctime>
# include <sys/time.h>
# include <sys/stat.h>
# define NB_OF_RESPONSE_HEADERS 10
namespace ft {
	class ResponseHeader {
		private:
		std::map<std::string, std::string>			_fields;
		std::string get_mime_type(const std::string &file);
		public:
			ResponseHeader();
			~ResponseHeader();
			ResponseHeader &operator=(ResponseHeader &x);
			ResponseHeader(ResponseHeader &x);
			
			std::string								make_headers(void);
			std::map<std::string, std::string>		&get_fields(void);
			void									set_field(std::string first, std::string second);
			void									set_date(void);
			void									set_server(void);
			void									set_content_encoding(void);
			// void									set_content_language(void);
			void									set_content_length(size_t length);
			void									set_content_location(const std::string &new_location);
			void									set_content_type(const std::string &file);
			void									set_last_modified(void);
			void									set_location(const std::string &new_resource);
			void									set_transfer_encoding(void);
			//MEMBER FUNCTIONS
	};//end of ResponseHeader class
}//end of namespace
#endif