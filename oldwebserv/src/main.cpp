#include "../webserv.hpp"

Webserver	webserv("global");

void DisconnectSignal(int a) {
//	std::cout << "Signal handling\n";
	(void) a;
	webserv.g_signal = true;
//	g_signal;
//	std::exit(0);
}

int		main(int ac, char **av) {

	std::signal(SIGINT, DisconnectSignal);
	std::signal(SIGQUIT, DisconnectSignal);
	std::signal(SIGTERM, DisconnectSignal); //Signal handling

	if (ac > 2)
	{
		std::cerr << "Usage: ./webserv [path-to-config-file]" << std::endl;
		return EXIT_FAILURE;
	}
	try
	{
		//Webserver webserv(av[1]);;
		webserv.config(av[1]);
		webserv.run();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return EXIT_SUCCESS;
}
