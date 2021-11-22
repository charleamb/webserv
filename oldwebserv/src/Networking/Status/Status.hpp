#ifndef STATUS_HPP
#define STATUS_HPP

# include "../ft_networking.hpp"
# include <string>

namespace ft {
	class Status {
		private:
		Status();
		~Status();

		struct Code {
			int						code;
			std::string				message;
		};
		static const Code			_status[];
		public:

		static std::string			get_message(int code);
	}; // end of Status class
}; // end of namespace
#endif