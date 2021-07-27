#ifndef SOCKET_HPP
# define SOCKET_HPP

# include "ft_sockets.hpp"

namespace ft {
	class Socket {
		private:
		struct sockaddr_in	_address;
		int					_fd;

		public:
		Socket();
		Socket(int fd, const sockaddr_in &address);
		virtual ~Socket();

		// GETTERS / SETTERS
		int					get_fd(void) const;
		struct sockaddr_in	&get_address(void);
		void				set_address(uint16_t port, const std::string &host);


		void				create_fd();
		void				make_non_blocking(void);

		// SOCKETEXCEPTION CLASS
		class SocketException : public std::exception {
			private:
			std::string		_msg;

			public:
			SocketException(const std::string &msg) throw();
			virtual ~SocketException() throw();
			virtual const char *what() const throw();
		}; //end of socket exception class
	}; //end of socket class
}
#endif