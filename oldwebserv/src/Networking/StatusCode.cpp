#include "StatusCode.hpp"

std::string					StatusCode::get_description(int code)
{
	for (int i = 0; i < NB_OF_STATUS_CODE; i++) {
		if (code == _status[i].code) 
		{
			return std::string(itostr(code) + SPACE + _status[i].message);
		}
	}
	return get_description(500);
}

const StatusCode::status		StatusCode::_status[] = {
	//Informational responses (100–199)
	{100, "Continue"},
	{101, "Switching Protocol"},
	{102, "Processing"},
	{103, "Early Hints"},
	//Successful responses (200–299)
	{200, "OK"},
	{201, "Created"},
	{202, "Accepted"},
	{203, "Non-Authoritative Information"},
	{204, "No Content"},
	{205, "Reset Content"},
	{206, "Partial Content"},
	{207, "Multi-Stauts"},
	{208, "Already Reported"},
	{226, "IM Used"},
	//Redirects (300–399)
	{300, "Multiple Choise"},
	{301, "Moved Permanently"},
	{302, "Found"},
	{303, "See Other"},
	{304, "Not Modified"},
	{305, "Use Proxy"}, //deprecated
	{306, "unused"},
	{307, "Temporary Redirect"},
	{308, "Permanent Redirect"},
	//Client errors (400–499)
	{400, "Bad Request"},
	{401, "Unauthorized"},
	{402, "Payment Required"},
	{403, "Forbidden"},
	{404, "Not found"},
	{405, "Method Not Allowed"},
	{406, "Not Acceptable"},
	{407, "Proxy Authentication Required"},
	{408, "Request Timeout"},
	{409, "Conflict"},
	{410, "Gone"},
	{411, "Length Required"},
	{412, "Precondition Failed"},
	{413, "Payload Too Large"},
	{414, "URI Too Long"},
	{415, "Unsupported Media Type"},
	{416, "Range Not Statisfiable"},
	{417, "Exceptation Failed"},
	{418, "I'm a teapot"},
	{421, "Misdirected Request"},
	{422, "Unprocessable Entity"},
	{423, "Locked"},
	{424, "Failed Dependency"},
	{425, "Too early"},
	{426, "Upgrade Required"},
	{428, "Precondition Required"},
	{429, "Too many requests"},
	{431, "Request Header Fields Too Large"},
	{451, "Unavailable For Legal Reasons"},
	//Server errors (500–599)
	{500, "Internal Server Error"},
	{501, "Not Implemented"},
	{502, "Bad Gateway"},
	{503, "Service Unavailable"},
	{504, "Gateway Timeout"},
	{505, "HTTP Version Not Supported"},
	{506, "Variant Also Negotiates"},
	{507, "Insufficient Storage"},
	{508, "Loop Detected"},
	{510, "Not Extended"},
	{511, "Network Authentication Required"},
};
