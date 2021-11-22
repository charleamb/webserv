#include "Client.hpp"


// CONSTRUCTORS / DESTRUCTOR ##################################################################################################

Client::~Client() {
if (request)
	delete request;
}

Client::Client()
: _fd(-1),
_server(0),
connected(false),
responses(),
request()
{
//	request = new Request();
}

Client::Client(const Client &ref) :
_fd(ref._fd),
_server(ref._server),
request(ref.request),
responses(ref.responses),
connected(ref.connected)
{}

Client::Client(int fd, Server *server) :
_fd(fd),
_server(server),
connected(false),
responses(),
request()
{
//	request = new Request();
}


//	FUNCTIONS #################################################################################################################	

ConfigServer *Client::find_config(void) const
{
	return _server->find_hostname(request->host);
}

/*
**	finds the location object whose route is the most specific match to the requested_route
**	if requested_route = /bin/lol/
**	between two locations "/" and "/bin/" find_location returns "/bin/"
*/

const Location				*Client::find_location(const std::vector<Location> &locations, const std::string &requested_route) const
{
	//return find_config()->has_location(requested_route);



	//ISSUE: me donne la location "/salut" alors que ma requête est "/"
	size_t				old_matching_score;
	const Location		*matched_location;

	old_matching_score = 0;
	matched_location = &locations[0];

	for (size_t i = 0; i < locations.size(); i++) {
		if (requested_route.find(locations[i].route) != std::string::npos) {
			size_t matching_score = locations[i].route.size();
			if (matching_score > old_matching_score) {
				old_matching_score = matching_score;
				matched_location = &locations[i];
			}
		}
		else if (old_matching_score < 2 && locations[i].route[0] == '~') {
			std::cout << "TODO: regex parsing" << std::endl;
		}
	}
	return matched_location;
}

// GETTERS / SETTERS ##############################################################################################################


const Response				*Client::get_response(void) const
{
	return &(responses.front());
}

const std::queue<Response>	&Client::get_responses(void) const
{
	return responses;
}

Request						*Client::get_request(void) const
{
	return request;
}

void						Client::set_server(Server *server)
{
	_server = server;
}

const Server 				*Client::get_server(void) const
{
	return _server;
}
