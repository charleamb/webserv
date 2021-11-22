#ifndef LISTENINGSOCKET_HPP
# define LISTENINGSOCKET_HPP

# include "BindingSocket.hpp"

namespace ft {
	class ListeningSocket : public BindingSocket {
		private:
		void	_listen(int socket);

		public:
		ListeningSocket(uint16_t port, const std::string &host);
		~ListeningSocket();
		
	}; // end of ListeningSocket class
}
#endif