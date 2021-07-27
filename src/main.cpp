
#include "Networking/Server/Server.hpp"
#include "parsing/Config/config.hpp"

#include <fstream>
#include <sstream>

double now_double(void)
{
	struct timespec tv;

	if(clock_gettime(CLOCK_REALTIME, &tv) != 0) return 0;

	return (tv.tv_sec + (tv.tv_nsec / 1000000000.0));
}

int main(int ac, char **av)
{
	(void)ac;
	std::ifstream t(av[1]);
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string		str = buffer.str();
	slice cfg(str);
	ParserResult<std::vector<config::ServerConfig> >	res = ConfigParser()(cfg);
	if (res.is_err()) {
		std::cerr << res.unwrap_err() << std::endl;
	} else if (res.left().size) {
		std::cerr << "Configuration error: at " << res.left().take(16) << std::endl;
	} else {
		for (std::vector<config::ServerConfig>::iterator it = res.unwrap().begin(); it != res.unwrap().end(); it++) {
			std::cout << *it << std::endl;
		}
	}

	//RequestHandler	handler;

	//std::string		body = "GET / HTTP/1.1\r\nHost: localhost\r\nContent-Length: 47\r\n\r\nsome random bullshit except I'm not supposed to";
	//handler.update(body.c_str(), body.length());

	try {
		// std::vector<uint16_t> ports;
		// ports.push_back(3000);
		ft::Server server(res.unwrap());

		 std::cout << "Server started" << std::endl;
	} catch (std::exception &e) {
		std::cerr << "Server error: " << e.what() << std::endl;
	}
}
