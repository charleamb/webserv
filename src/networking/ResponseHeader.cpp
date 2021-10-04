#include "ResponseHeader.hpp"

//	CONSTRUCTORS / DESTRUCTOR ############################################################################################

ResponseHeader::ResponseHeader()
: _fields()
{
	_fields.insert(std::make_pair("Server", "Webserv/1.2.3 (Unix)"));
	// _fields.insert(std::nake_pair("Connection", ));
	set_date();
}

ResponseHeader::ResponseHeader(const ResponseHeader &ref)
: _fields(ref._fields)
{}

ResponseHeader::~ResponseHeader()
{
	_fields.clear();
}

// FORMATTING ############################################################################################################

std::string			ResponseHeader::format(void) 
{
	std::string		buffer;

	for (std::map<std::string, std::string>::iterator it = _fields.begin(); it != _fields.end(); it++) {
		buffer += std::string(it->first + ": " + it->second + CRLF);
	}

	buffer += std::string(CRLF);
	return buffer;
}

// GETTERS & SETTERS #####################################################################################################

std::map<std::string, std::string> &ResponseHeader::get_fields(void)
{
	return _fields;
}

void				ResponseHeader::set_content_length(size_t length)
{
	_fields.insert(std::make_pair("Content-Length", itostr(length)));
}

void				ResponseHeader::set_content_type(const std::string &filename)
{
	std::string type = std::string("application/octet-stream");
	size_t found = filename.find_last_of(".");

	if (found != std::string::npos) {
		std::string extension = filename.substr(found + 1, filename.size() - found - 1);
		type = get_mime_type(extension);
	}
	_fields.insert(std::make_pair("Content-Type", type));
}

void				ResponseHeader::set_date(void)
{
	struct timeval		clock;
	struct tm			*time;
	char				date[100];

	gettimeofday(&clock, NULL);

	time = gmtime(&clock.tv_sec);
	strftime(date, 100, "%a, %d %b %Y %T %Z", time);
	_fields.insert(std::make_pair("Date", std::string(date)));
}

void				ResponseHeader::set_last_modified(const std::string &path)
{
	struct tm			*time;
	char				date[100];
	struct stat			buff;

	stat(path.c_str(), &buff);
	time = std::gmtime(&buff.st_mtim.tv_sec);
	std::strftime(date, 100 , "%a, %d %b %Y %T %Z", time);
	_fields.insert(std::make_pair("Last-Modified", std::string(date)));
}

const std::string	ResponseHeader::get_mime_type(const std::string &extension)
{
	for (int i = 0; i < NB_MIME_TYPES; i++) {
		if (extension.compare(_mime[i].extension) == 0)
			return _mime[i].type;
	}
	return std::string("application/octet-stream");
}

const ResponseHeader::mime_type	ResponseHeader::_mime[] =
{
	{"aac", "audio/aac"},
	{"abw", "application/x-abiword"},
	{"arc", "application/x-freearc"},
	{"avi", "video/x-msvideo"},
	{"azw", "application/vnd.amazon.ebook"},
	{"bin", "application/octet-stream"},
	{"bmp", "image/bmp"},
	{"bz", "application/x-bzip"},
	{"bz2", "application/x-bzip2"},
	{"cda", "application/x-cdf"},
	{"csh", "application/x-csh"},
	{"css", "text/css"},
	{"csv", "text/csv"},
	{"doc", "application/msword"},
	{"docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
	{"eot", "application/vnd.ms-fontobject"},
	{"epub", "application/epub+zip"},
	{"gz", "application/gzip"},
	{"gif", "image/gif"},
	{"htm", "text/html"},
	{"html", "text/html"},
	{"ico", "image/vnd.microsoft.icon"},
	{"ics", "text/calendar"},
	{"jar", "application/java-archive"},
	{"jpeg", "image/jpeg"},
	{"jpg", "image/jpeg"},
	{"jsonld", "application/ld+json"},
	{"js", "text/javascript"},
	{"json", "application/json"},
	{"mid", "audio/midi"},
	{"midi", "audio/midi"},
	{"mjs", "text/javascript"},
	{"mp3", "audio/mpeg"},
	{"mp4", "audio/mp4"},
	{"mpeg", "audio/mpeg"},
	{"mpkg", "application/vnd.apple.installer+xml"},
	{"odp", "application/vnd.oasis.opendocument.presentation"},
	{"ods", "application/vnd.oasis.opendocument.spreadsheet"},
	{"odt", "application/vnd.oasis.opendocument.text"},
	{"oga", "audio/ogg"},
	{"ogv", "video/ogg"},
	{"ogx", "application/ogg"},
	{"opus", "audio/opus"},
	{"otf", "font/oft"},
	{"png", "image/png"},
	{"pdf", "application/pdf"},
	{"php", "application/x-httpd-php"},
	{"ppt", "application/vnd.ms-powerpoint"},
	{"pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"},
	{"rar", "application/vnd.rar"},
	{"rtf", "application/rtf"},
	{"sh", "application/x-sh"},
	{"svg", "image/svg+xml"},
	{"swf", "application/x-shockwave-flash"},
	{"tar", "application/x-tar"},
	{"tif", "image/tiff"},
	{"tiff", "image/tiff"},
	{"ts", "video/mp2t"},
	{"ttf", "font/ttf"},
	{"txt", "text/plain"},
	{"xml", "text/xml"},
	{"zip", "application/zip"},
};
