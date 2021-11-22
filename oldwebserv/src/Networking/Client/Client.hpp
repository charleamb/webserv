# ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../Sockets/Socket.hpp"
# include <string>
# include "parsing/HTTP/Request.hpp"
# include <queue>
# include "../Response/Response.hpp"
namespace ft {
	class Client : public Socket {
		private:
		Client();
		Client &operator=(const Client &x);
		Client(const Client &x);
		std::queue<Response>		_responses;
		//TODO: _request et response vont dans private:

		public:
		virtual ~Client();
		RequestHandler	_req;
		std::queue<Response>			&get_responses();
		Response						&get_front_response();
		Client(int fd, sockaddr_in address);
	}; //end of Client class
} //end of namespace
#endif