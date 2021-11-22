#include "Response.hpp"

// ft::Response::Response(const Request &req): _status(status::Waiting), _code(200), _body(""), _offset(0), _buffer("") {
// 	//TODO: ????
// }

ft::Response::Response() : _status(status::Waiting), _code(200), _body("") , _offset(0), _buffer(""){
	//TODO: ???
}

ft::Response &ft::Response::operator=(ft::Response &x) {
	_status = x.get_status();
	_code = x.get_code();
	_headers = x.get_headers();
	_body = x.get_body();
	_offset = x._offset;
	_buffer = x.get_buffer();
	return *this;
}

ft::Response::~Response() {
	std::cout << "Response destructor called" << std::endl;
}


//GETTERS
const std::string					&ft::Response::get_buffer(void) const {
	return _buffer;
}

const int							&ft::Response::get_offset(void) const {
	return _offset;
}

const status::Status				&ft::Response::get_status(void) const {
	return _status;
}

int 								ft::Response::get_code(void) const {
	return _code;
}

const std::string					&ft::Response::get_body(void) const {
	return _body;
}

ft::ResponseHeader					&ft::Response::get_headers(void) {
	return _headers;
}

//SETTERS
void								ft::Response::set_offset(int offset) {
	_offset = offset;
}

void								ft::Response::set_status(const status::Status &status) {
	_status = status;
}

const status::Status 				&ft::Response::update_status(void) {
	if (_offset == -1)
		_status = status::Error;
	else if (_offset == 0)
		_status = status::Waiting;
	else if (_offset < (int)_buffer.size())
		_status = status::Incomplete;
	else
		_status = status::Complete;
	return _status;
}


void	ft::Response::write_startline(void) {
	std::string status = ft::Status::get_message(_code);

	_buffer = std::string(std::string("HTTP/1.1") + SPACE + status + CRLF);
}

void	ft::Response::write_response(void) {
	write_startline();
	_buffer += _headers.make_headers();
	_buffer += _body;
}

// void	ft::Response::delete_method(const Request &req) {
// 	std::string _path = "/Users/gofmane/42/index.html";
// 	//if request to delete something that will break our server
// 	//	_code = 405;
// 	if (is_file(_path)) {
// 		if (is_perm(_path)) {
// 			if (remove(_path.c_str()) < 0)
// 				std::cerr << "Error when removing file" << std::endl;
// 			_code = 204; //No content
// 		}
// 		else
// 			_code = 403; //Forbidden
// 	}
// 	else
// 		_code = 404; //Not found
// 	// if (_code != 204)
// 		// _body = read_error_page(path_to_error_page);
// 	_headers.set_field("Content-Length", std::to_string(_body.size()));
// 	write_response();
// }

std::string ft::Response::read_error_page(std::string path) {
	std::ofstream		file;
	std::stringstream	buffer;

	_headers.set_field("Content-Type", "text/html");
	if (is_file(path))
	{
		file.open(path.c_str(), std::ifstream::in);
		if (!file.is_open()) {
			std::cerr << "Could not open error page" << std::endl;
			return ("<!DOCTYPE html><html><body><h1>Oh no!</h1><p>We lost our error page :(.</p></body></html>");
		}
		buffer << file.rdbuf();
		file.close();
		return buffer.str();
	}
	else {
		std::cerr << "Could not find error page "<< std::endl;
		return ("<!DOCTYPE html><html><body><h1>Oh no!</h1><p>We lost our error page :(.</p></body></html>");
	}
}

ft::Response::Response(Response &x) {
	_status = x.get_status();
	_code = x.get_code();
	_headers = x.get_headers();
	_body = x.get_body();
	_offset = x._offset;
	_buffer = x.get_buffer();

}
// void	ft::Response::post_method(const Request &req) {

	//if resource sucessfully created,
	// return 201 (Created) + Location header field + status of the request while referring to the new resouce
	// set location
// 	If one or more resources has been created on the origin server as a
//    result of successfully processing a POST request, the origin server
//    SHOULD send a 201 (Created) response containing a Location header
//    field that provides an identifier for the primary resource created
//    (Section 7.1.2) and a representation that describes the status of the
//    request while referring to the new resource(s).
//    If the result of processing a POST would be equivalent to a
//    representation of an existing resource, an origin server MAY redirect
//    the user agent to that resource by sending a 303 (See Other) response
//    with the existing resource's identifier in the Location field.
	//_body = ""
	//upload file
	//if path exits
	//	if has perm
	//		takes body and go modify the file
	//	else
	//		_code = 403;
	//else
	//	_code = 404
	//if (_code > 400)
	// _body = read_error_page(path_to_error_page);
// }


// void	ft::Response::get_method(const Request &req) {
// 	_body = "";

// }

// void	ft::Response::write_content(void) {

// }

// void	ft::Response::read_content(void) {

// }