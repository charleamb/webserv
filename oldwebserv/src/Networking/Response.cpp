# include "Response.hpp"

Response::Response()
: _status_msg(std::string("")),
_headers(),
offset(0),
buffer(),
size(0),
status(Ready),
code(200),
body(std::string("")),
cgi_input_offset(0)
{}

Response::Response(int code)
: _headers(),
_status_msg(),
code(code),
body(""),
offset(0),
buffer(),
size(0),
status(Ready),
cgi_input_offset(0)
{}

Response::Response(const Response &response)
: _status_msg(response._status_msg),
_headers(response._headers),
offset(response.offset),
buffer(response.buffer),
size(response.size),
status(response.status),
code(response.code),
body(response.body),
request_body(response.request_body),
cgi_input_offset(response.cgi_input_offset)
{}

Response &Response::operator=(const Response &ref) //TODO: voir si on l'utilise sinon remplacer par un copie + swap
{
	code = ref.code;
	_status_msg = ref._status_msg;
	_headers = ref._headers;
	body = ref.body;
	offset = ref.offset;
	buffer = ref.buffer;
	size = ref.size;
	status = ref.status;
	cgi_input_offset = ref.cgi_input_offset;
	request_body = ref.request_body;
	return *this;
}

Response::~Response() {
	//clear ResponseHeader
	//clear body ?
	//clear buffer ?
}

bool			Response::has_body(void) {
	return	body.empty();
}

void			Response::insert_header(const std::string &key, const std::string &value) {
	_headers.get_fields().insert(std::make_pair(key, value));
}

ResponseHeader		&Response::get_headers(void) {
	return _headers;
}

void			Response::format(std::map<std::string, std::string> *map) {
	//TODO: voir si c'est du transfer encoding
	// et si oui, ajouter le header
	
	if (map && map->count("Connection") && (*map)["Connection"] == "keep-alive")
		insert_header("Connection", "keep-alive");
	else if (map)
		insert_header("Connection", "close");

	//TODO: ajouter le header content-length si y a un body
	if (body.size())
		_headers.get_fields()["Content-Length"] = itostr(static_cast<int>(body.size()));

	std::string status = StatusCode::get_description(code);
	buffer = std::string(std::string("HTTP/1.1") + SPACE + status + CRLF);
	buffer +=  _headers.format();
	buffer += body;
}


void		Response::set_default_error_page(void)
{
	code = 500;
	body = "<!DOCTYPE html><html><body><h1>500</h1><p>Internal Server Error</p></body></html>";
	
	//TODO: set content type
	get_headers().get_fields()["Content-Type"] = "text/html";
}

void						Response::set_content(const std::string &filename)
{
	std::ifstream		file(filename, std::ios::binary);
	char				buf[2048];
	std::stringstream	ss;

	if (file.is_open())
	{
		ss << file.rdbuf();
		body = ss.str();
		/*std::ofstream output("output.png", std::ios::binary)
		output << ss.str();
		output.close();
		*/
		file.close();
		if (body.size())
			get_headers().set_content_type(filename);
	}
	else
		set_default_error_page();
}

void Response::print(void) {
	std::cout << "\tCode :" << code << '\n';
	std::cout << "\tMessage : " << _status_msg << '\n';
	for (std::map<std::string, std::string>::iterator it = _headers.get_fields().begin() ; it != _headers.get_fields().end(); it++)
	{
		std::cout << '\t' << it->first << ": " << it->second << '\n';
	}
	std::stringstream ss;
	ss << body;
	std::string line;
	std::cout << '\n';
	while (ss)
	{
		std::getline(ss, line);
		std::cout << '\t' << line << '\n';
	}
}

// void						Response::write_response(std::map<int, std::string> *error_pages) {
// 	std::string status = Status::get_description(_code);
// 	buffer = std::string(std::string("HTTP/1.1") + SPACE + status + CRLF);

// 	if (_code >= 300)
// 	{
// 		std::string name, size_file;

// 		if (error_pages && error_pages->count(_code))
// 		{
// 			name = (*error_pages)[_code];
// 			std::cout << "error page : " << name << '\n';
// 		}
// 		else
// 		{
// 			name = IntToStr(_code);
// 			name.append(".html");
// 			name = DEFAULT_ERROR_PAGES + name;
// 		}
		
// 		get_file(name);
// 	}
// 	if (!get_header().get_fields().count("Content-Length"))
// 		get_header().get_fields()["Content-Length"] = IntToStr(_body.size());
// 	if (!get_header().get_fields().count("Content-Type"))
// 		get_header().get_fields()["Content-Length"] = "application/octet-stream";
// 	buffer +=  _headers.format() + CRLF;
// 	buffer += _body + CRLF;
// 	std::cout << "body : " << '\"' << _body << '\"' << '\n';

// 	size = buffer.size();
// 	ready = true;
// }