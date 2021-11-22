#include "ResponseHeader.hpp"

ft::ResponseHeader::ResponseHeader() {

}

ft::ResponseHeader::~ResponseHeader() {
	_fields.clear();
}

ft::ResponseHeader &ft::ResponseHeader::operator=(ResponseHeader &x) {
	for (std::map<std::string, std::string>::iterator it = x.get_fields().begin(); it != x.get_fields().end(); it++) {
		set_field(it->first, it->second);
	}
	return *this;
}

ft::ResponseHeader::ResponseHeader(ResponseHeader &x) {
	for (std::map<std::string, std::string>::iterator it = x.get_fields().begin(); it != x.get_fields().end(); it++) {
		set_field(it->first, it->second);
	}
}

std::map<std::string, std::string>	&ft::ResponseHeader::get_fields(void) {
	return _fields;
}

void		ft::ResponseHeader::set_field(std::string first, std::string second) {
	_fields.insert(std::make_pair(first, second));
}

std::string ft::ResponseHeader::make_headers(void) {
	std::string buffer;
	for (std::map<std::string, std::string>::iterator it = _fields.begin(); it != _fields.end(); it++) {
		buffer += std::string(it->first + ":" + it->second + CRLF);
	}
	buffer += "\n";
	return buffer;
}

void		ft::ResponseHeader::set_date(void) {

	struct timeval		clock;
	struct tm			*time;
	char				date[100];

	gettimeofday(&clock, NULL);

	time = gmtime(&clock.tv_sec);
	strftime(date, 100, "%a, %d %b %Y %T %Z", time);
	set_field("Date", std::string(date));
}

void		ft::ResponseHeader::set_server(void) {
	set_field("Server", "Webserv/1.2.3 (Unix)");
}


void		ft::ResponseHeader::set_content_encoding(void) {
	// chunked, compress, deflate, gzip, identity.
	std::string type;//TODO: get the type of encoding
	set_field("Content-Encoding", type);
}

// void		ft::ResponseHeader::set_content_language(void) {
// 	std::string language;
// 	set_field("Content-Language", language);
// }

void		ft::ResponseHeader::set_content_length(size_t length) {
	set_field("Content-Length", std::to_string(length));
}

void		ft::ResponseHeader::set_content_location(const std::string &new_location) {
	set_field("Content-Location", new_location);
}

std::string ft::ResponseHeader::get_mime_type(const std::string &file) {
	size_t found = file.find_last_of(".");
	if (found != std::string::npos) {
		std::string extension = file.substr(found + 1, file.size() - found - 1);
		if (extension.compare("html") == 0 || extension.compare("css") == 0 || extension.compare("js") == 0 )
			return std::string("text/" + extension);
		else if (file.compare("jpg") == 0 || file.compare("jpeg") || file.compare("png") == 0)
			return std::string("image/" + extension);
	}
	return std::string("text/plain"); 
}
void		ft::ResponseHeader::set_content_type(const std::string &file) {
	//media type
	std::string type = get_mime_type(file);
	set_field("Content-Type", type);
}

void		ft::ResponseHeader::set_last_modified(void) {
	struct tm			*time;
	char				date[100];
	struct stat			buff;
	//TODO: replace by path var

	stat("/Users/gofmane/42/index.html", &buff);
	time = std::gmtime(&buff.st_mtimespec.tv_sec);
	std::strftime(date, 100 , "%a, %d %b %Y %T %Z", time);
	set_field("Last-Modified", std::string(date));
}

void		ft::ResponseHeader::set_location(const std::string &redirection) {
	//Used in redirection, or when a new resource has been created
	set_field("Location", redirection);
}

void		ft::ResponseHeader::set_transfer_encoding(void) {
	// chunked, compress, deflate, gzip, identity.
}
