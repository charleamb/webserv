#include "RequestInterpreter.hpp"

//	CONSTRUCTOR / DESTRUCTOR #############################################################################################
RequestInterpreter::RequestInterpreter() {
	std::cout << "its working" << std::endl;
}

RequestInterpreter::RequestInterpreter(Client *client, std::map<int, Response *> *CGI_output, std::map<int, Response *> *CGI_input, fd_set *masterfd, int *fdmax)
: _request(client->request),
// _config(),
// _location(),
// _response()
// : _request(),
_config(client->find_config()),
_location(client->find_location(_config->locations, _request->path)),
_masterfd(masterfd),
_cgi_output(CGI_output),
_cgi_input(CGI_input),
_fd_max(fdmax)
{
	// client->get_request()->printvalue();
	// std::cout << "route " << _location->route << std::endl;
	// _request = client->request;
	// std::cout << "before interpret" << std::endl;
	convert_path(); 					//convert /salut to /var/www/html/salut
	std::cout << "real_path : " << _request->real_path << " avec " << _location << " et " << (_location ? *_location->root : "") << '\n';
	interpret();						//TODO: Simplifier parce que pas tres clair

	/*if (_request->headers.count("Connection") && _request->headers["Connection"] == "keep-alive")
		_response.insert_header("Connection", "keep-alive");
	else
		_response.insert_header("Connection", "close");*/

	_response.format(&_request->headers);
	std::cout << "after formattnig : " << _response.code << '\n';
	client->responses.push(_response);

	if (_response.status != Ready)
	{
		(*_cgi_output)[_cgi_output_fd] = &client->responses.front();
		if (_response.status == Waiting_CGI_Input)
			(*_cgi_input)[_cgi_input_fd] = &client->responses.front();
	}


	//std::cout << client->responses.front().buffer << std::endl;
	delete client->request;
	client->request = 0;
}

RequestInterpreter &RequestInterpreter::operator=(RequestInterpreter &x)
{
	RequestInterpreter tmp(x);
	swap(x);
	return *this;
}

void			RequestInterpreter::swap(RequestInterpreter &x)
{
	char buffer[sizeof(RequestInterpreter)];

	memcpy(buffer, reinterpret_cast<char*>(&x), sizeof(RequestInterpreter));
	memcpy(reinterpret_cast<char*>(&x), reinterpret_cast<char*>(this), sizeof(RequestInterpreter));
	memcpy(reinterpret_cast<char*>(this), buffer, sizeof(RequestInterpreter));
}
RequestInterpreter::~RequestInterpreter() {}


//	FUNCTIONS ###########################################################################################################

void 	RequestInterpreter::interpret(void)
{
	try {
		valid_syntax();
		is_method_allowed();
		is_authorized();
		is_allowed_body_size();
		call_methods();

		if (_location && _location->redirect)
		{
			if (_location->redirect->first == 301 || _location->redirect->first == 302)
				throw RequestInterpreterException(_location->redirect->first, _location->redirect->second);
		}
	}
	catch (const RequestInterpreterException &e){
		std::cout << "catch:\n";
		std::cerr << e.what() << std::endl;
		std::cout << e.get_code() << std::endl;
		if (e.get_code() == 301 || e.get_code() == 302)
		{
			std::cout << "Redirection catch\n";
			_response.code = e.get_code();
			_response.insert_header("Location", e.what());
		}
		generate_error_page(e.get_code());
	}
}

void		RequestInterpreter::generate_error_page(int code)
{	
	std::string _path_to_error_page = PATH_TO_ERROR_PAGE_TEMPLATE;
	_response.code = code;
	if (_config->error_pages.count(code))
	{
		_path_to_error_page = _config->error_pages[code];
	}
	std::cout << _path_to_error_page << std::endl;
	_response.set_content(_path_to_error_page);
	if (_path_to_error_page.compare(PATH_TO_ERROR_PAGE_TEMPLATE) == 0)
	{
		_response.body = replace(_response.body, "$1", std::string(itostr(code)));
		_response.body = replace(_response.body, "$2", StatusCode::get_description(code));
	}
}

void		RequestInterpreter::valid_syntax(void) const
{
	if (_request->http_version.compare("HTTP/1.1") != 0)
	{
		throw RequestInterpreterException(400);
	}
}

void		RequestInterpreter::is_method_allowed(void)
{
	if (!_location
	|| (_request->method.compare("GET") == 0 && !(_location->methods & GET))
	|| (_request->method.compare("POST") == 0 && !(_location->methods & POST))
	|| (_request->method.compare("DELETE") == 0 && !(_location->methods & DELETE)))
	{
		_response.insert_header("Allow", _location->limit_except);
		throw RequestInterpreterException(405);
	}
}

// bool		RequestInterpreter::authenticate(const std::string &auth_field) const
// {
// 	size_t type = auth_field.find("Basic");

// 	if (type != std::string::npos)
// 	{
// 		int credentials = auth_field.find_first_not_of("\t ", type + 6);
// 		std::string encoded_credentials = auth_field.substr(credentials);
// 		std::string decoded_credentials = ft::Base64::decode(encoded_credentials);

// 		//todo check if it matches with auth_basic
// 		//TODO: chercher dans le AuthUserFile
// 	}
// 	return false;
// }

void		RequestInterpreter::is_authorized(void)
{

	if (!_location->auth_basic.empty())
	{
		_response.insert_header("WWW-Authenticate", "Basic realm=\"Login required\"");
		throw RequestInterpreterException(401);
	}
}

void	  RequestInterpreter::is_allowed_body_size(void) const
{
	if (_config->client_max_body_size > 0 && _request->body.size() > size_t(_config->client_max_body_size))
	{
		throw RequestInterpreterException(413);
	}
}

//TODO: remplacer call_methods par un tableau static de int et de pointeur sur fonction
void	RequestInterpreter::call_methods(void)
{
	if (_request->method.compare("GET") == 0)
	{
		call_get();
	}
	else if (_request->method.compare("POST") == 0)
	{
		call_post();
	}
	else if (_request->method.compare("DELETE") == 0)
	{
		call_delete();
	}
	else
	{
		throw RequestInterpreterException(501);
	}
}

void	RequestInterpreter::call_delete()
{
	if (unlink(_request->real_path.c_str()))
	{
		throw RequestInterpreterException(500);
	}
	else
	{
		_response.code = _response.has_body() ? 200 : 204;
	}
}

void			RequestInterpreter::directory_listing(void)
{
	std::cout << "Directory listing\n";
	Response response;
	std::string tmp;
	/*if (client.get_cursor())
		client.set_cursor(path);*/
	tmp = \
	"<html> \
	<head><title>Index of " + _request->path + "</title></head>\n\
	<body bgcolor=\"white\">\n\
	<h1> Index of " + _request->path + "</h1><hr><pre><a href=\"../\">../</a>\n";

	struct dirent *entry;
	DIR *dir = opendir(_request->real_path.c_str());
	if (!dir)
		throw RequestInterpreterException(404);
	while ((entry = readdir(dir)))
	{
		if (std::string(entry->d_name) != ".." && std::string(entry->d_name) != ".")
		{
			tmp += "<a href=\"" + std::string(entry->d_name) + (is_directory(strToPath(_request->real_path + "/" + std::string(entry->d_name))) ? "/" : "")
			+ "\">" + std::string(entry->d_name) + (is_directory(strToPath(_request->real_path + "/" + std::string(entry->d_name))) ? "/" : "") + "</a>\n";
		}
	}
	closedir(dir);
	tmp += "\n</pre><hr></body> \n\
	</html>\n";
	_response.body = tmp;
	_response.insert_header("Content-Type", "text/html");
}

void	RequestInterpreter::call_get(void)
{
	struct stat tmp;
	std::cout << "GET : " << _request->real_path << '\n';
	stat(_request->real_path.c_str(), &tmp);
	if (tmp.st_mode & S_IFDIR) //TODO : directory listing
	{
		if (_request->real_path[_request->real_path.size() - 1] != '/') //Redirection si dossier mais sans / à la fin
		{
			std::cout << "host request : " << _request->headers["Host"] << '\n';
			throw RequestInterpreterException(301, "http://" + _request->headers["Host"] + _request->path + "/");
		}

		if (_location)
		{
			if (_location->listing)
				directory_listing();
			else if (_location->index)
				get_file(strToPath(_request->real_path + "/" + *(_location->index)));
			else
				throw RequestInterpreterException(404);
		}
		else
			throw RequestInterpreterException(404);
	}
	else
		get_file(_request->real_path);
}

void	RequestInterpreter::get_file(const std::string &path)
{
	if (!file_exists(path))
	{
		throw RequestInterpreterException(404);
	}
	else if (CGI_check_php(path))
	{
		if (file_is_executable(path))
		{
			php_cgi();
		}
		else
		{
			throw RequestInterpreterException(403);
		}
	}
	else
	{
		if (file_is_readable(path))
		{
			_response.set_content(path);
		}
		else
		{
			throw RequestInterpreterException(403);
		}
	}
}

void			RequestInterpreter::call_post(void)
{
	std::cout << "POST : " << _request->real_path << '\n';
	if (CGI_check_php(_request->real_path))
	{
		std::cout << "php CGI checked : " << _location << " et " <<find_in_vector(_location->cgi_extension, std::string("php")) <<'\n';
		//TODO: executer script sur cgi
		if (_location && find_in_vector(_location->cgi_extension, std::string("php")) && !access(_request->real_path.c_str(), F_OK | X_OK)) //Si permission d'executer
			php_cgi();
	}
	else if (_location && _location->upload_route) // multipart/form-data a verifier
	{
		std::string filename = _request->real_path.substr(_request->real_path.find_last_of('/') + 1);
		if (filename.size() < 1)
			throw RequestInterpreterException(404); //erreur nom fichier incorrect : Not Found
		std::ofstream file;
		file.open(std::string(*_location->upload_route + filename).c_str());
		if (!file.is_open())
		{
			std::cerr << "Could not open file\n";
			throw RequestInterpreterException(500); //erreur : Internal Error
		}
		//creation de fichier
		if (!_location)
		{
			std::cerr << "No location\n";
			throw RequestInterpreterException(403); //renvoyer une erreur car seule une location peut accepter des uploads : Not Authorized
		}
		file << _request->body;
		file.close();
		std::cout << "\tRessource created :" << std::string(*_location->upload_route + filename).c_str() << '\n';

		throw RequestInterpreterException(201);
		//Completer la reponse : Created
	}
	else
	{
		throw RequestInterpreterException(403); 
		//tentative de créer un fichier là où il n'y a pas le droit : Not Authorized
	}
}

void			RequestInterpreter::php_cgi()
{
	std::cout << "Execute php CGI\n";
	std::vector<std::string> tmp = CGI_getenv(_request->real_path, _request);
	std::vector<char *> CGI_env;
	for (int i = 0; i < tmp.size(); i++)
		CGI_env.push_back(const_cast<char *>(tmp[i].c_str()));

	std::vector<char *> param;
	std::string cgi_path = PHP_CGI_PATH;
	param.push_back(const_cast<char *>(cgi_path.c_str()));
	//param.push_back(const_cast<char *>(path.c_str()));
	param.push_back(0);

	std::stringstream fd;
	int pipe_output[2];
	int pipe_input[2];

	pid_t child_pid = 0;
	int child_status = 0;

	pipe(pipe_output);
	pipe(pipe_input);

	_cgi_output_fd = pipe_output[0];
	_cgi_input_fd = pipe_input[1];


	if (!(child_pid = fork()))
	{
		std::string tmp;

		if (dup2(pipe_input[0], STDIN_FILENO) == -1)
			std::cerr << "Erreur dup2 input\n";

		if (dup2(pipe_output[1], STDOUT_FILENO) == -1)
			std::cerr << "Erreur dup2 output\n";

		close(pipe_input[0]);
		close(pipe_input[1]); //ferme écriture de l'input child
		close(pipe_output[0]); //ferme lecture de l'output child
		close(pipe_output[1]);
		
		CGI_env.push_back(0);
		execve(param[0], param.data(), CGI_env.data());
		std::cerr << "Fail execution\n";
		std::exit(0);
	}
	close(pipe_input[0]);
	close(pipe_output[1]); //ferme écriture de l'output cgi
	std::cout << "\tWaiting for child process CGI " << child_pid << "...\n";
	_response.cgi_child = child_pid;

	if (_request->body.size())
	{
		_response.request_body = _request->body;
		if (_cgi_input_fd > *_fd_max)
			*_fd_max = _cgi_input_fd;
		FD_SET(_cgi_input_fd, _masterfd);
		_response.status = Waiting_CGI_Input;
	}
	else
	{
		_response.status = Waiting_CGI_Output;
		close(_cgi_input_fd);
	}
	
	FD_SET(pipe_output[0], _masterfd);
	*_fd_max = pipe_output[0] > *_fd_max ? pipe_output[0] : *_fd_max;
	_response.code = 200;
}

void				RequestInterpreter::convert_path()
{
	_request->real_path = _request->path;
	if (_location && _location->root)
		replace_once(_request->real_path, _location->route, *(_location->root));
	else if(_config->root.size() > 1)
		_request->real_path.insert(0, _config->root.substr(0, _config->root.size() - 1));
}

RequestInterpreter::RequestInterpreterException::RequestInterpreterException(const std::string &msg) throw() : _msg(msg) {}

RequestInterpreter::RequestInterpreterException::RequestInterpreterException(int code, const std::string str) throw() : _code(code), _msg(str) {}

RequestInterpreter::RequestInterpreterException::RequestInterpreterException(int code) throw() : _code(code) {}

RequestInterpreter::RequestInterpreterException::~RequestInterpreterException() throw() {}

int	RequestInterpreter::RequestInterpreterException::get_code(void) const { return _code; }

const char *RequestInterpreter::RequestInterpreterException::what(void) const throw() {
	return _msg.c_str();
}
