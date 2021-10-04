#ifndef RESPONSEHEADER_HPP
# define RESPONSEHEADER_HPP

# include <map>
# include <string>
# include <ctime>
# include <sys/time.h>
# include <sys/stat.h>

# include "../utils/utils.hpp"

# define NB_MIME_TYPES 61

class ResponseHeader {
	private:
		struct mime_type {
			std::string		extension;
			std::string		type;
		};

		std::map<std::string, std::string>	_fields;
		static const mime_type				_mime[];
	
	void								set_date(void);
	public:
		std::map<std::string, std::string>	&get_fields(void);
		static const std::string			get_mime_type(const std::string &extension);
		void								set_content_type(void);
		void								set_last_modified(const std::string &path);
		void								set_content_type(const std::string &filename);
		void								set_content_length(size_t length);
		std::string							format(void);
		ResponseHeader();
		ResponseHeader(const ResponseHeader &ref);
		~ResponseHeader();
};
#endif
