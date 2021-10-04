#ifndef STATUSCODE_HPP
# define STATUSCODE_HPP

# include <string>
# include <cstdlib>
# include <stdlib.h>
# include "../utils/utils.hpp"

# define NB_OF_STATUS_CODE 64
# define SPACE " "

class StatusCode {
	private:
		struct status {
			int						code;
			std::string				message;
		};
		static const status			_status[];
	public:
		static std::string			get_description(int code);
}; //end of Status class
#endif
