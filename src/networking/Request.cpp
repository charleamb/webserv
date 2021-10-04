#include "Request.hpp"

Request::Request() :
status(Waiting),
query_string(),
host(std::string("")),
buffer_size(-1),
buffer(std::string("")),
method(std::string("")),
path(std::string("")),
http_version(std::string("")),
headers(),
body(std::string(""))
{}

Request             &Request::operator=(Request &x)
{
    Request tmp(x);
    swap(tmp);
    return *this;
}

void    Request::swap(Request &x)
{
    char buffer[sizeof(Request)];

    memcpy(buffer, &x, sizeof(Request));
    memcpy(reinterpret_cast<char*>(&x), reinterpret_cast<char*>(this), sizeof(Request));
    memcpy(buffer, reinterpret_cast<char*>(this), sizeof(Request));
}

Request::Request(Request &x)
: status(x.status),
query_string(x.query_string),
host(x.host),
buffer_size(x.buffer_size),
method(x.method),
path(x.path),
http_version(x.http_version),
headers(x.headers),
body(x.body)
{}

void      Request::parse()
{
    while (buffer.find(CRLF) != std::string::npos) 
    {
        if (status == Error || status == Complete)
            break ;
        if (status == Waiting)
            parse_start_line();
        else if (status == StartLineParsed)
            parse_headers();
        else if (status == HeadersParsed)
            parse_body();
        else if (status == BodyParsed)
            check_request();
        //printvalue();
    } 
}

void      Request::check_request() 
{
    if (buffer.find_first_not_of(CRLF) == std::string::npos)
        status = Complete;
    else
        status = Error;
    if (headers.count("Host"))
        host = headers["Host"];
}

char	hexatodec(std::string code)
{
	char	hexa[] = "0123456789ABCDEF";
	int		decode;

	for (int i = 0; i < 17; i++)
		if (code[0] == hexa[i])
			decode = i * 16;
	decode = decode;
	for (int i = 0; i < 17; i++)
		if (code[1] == hexa[i])
			decode = decode + i;
	return decode;
}

void	Request::Querydecoding()
{
//	std::string test = "?parameter=%22Click+on+%27URL+Decode%27%21%22";
//	const char	*str = test.c_str();
	const char	*str = query_string.c_str();
	std::string	query, code;
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == '+')
			query.push_back(' ');
		else if (str[i] == '%')
		{
			if (str[i + 1] && (isdigit(str[i + 1]) || isupper(str[i + 1])))
			{
				i++;
				code.push_back(str[i]);
				if (str[i + 1] && (isdigit(str[i + 1]) || isupper(str[i + 1])))
				{
					i++;
					code.push_back(str[i]);
					query.push_back(hexatodec(code));
				}
			}
		}
		else
			query.push_back(str[i]);
		code.clear();
	}
	query_string = query;
}

void      Request::parse_start_line()
{
    std::string                 line;
    std::vector<std::string>    elems;

    size_t nl = buffer.find(CRLF);
    if (nl == std::string::npos)
    {
        status = Waiting;
    }
    else
    {
        line = buffer.substr(0, nl);
        elems = ft_split(line);
        if (elems.size() == 3)
        {
            method = elems[0];
            size_t query = elems[1].find("?");
            if (query != std::string::npos)
            {
                path = elems[1].substr(0, query);
                query_string = elems[1].substr(query + 1);
				Querydecoding();
            }
            else
                path = elems[1];
            http_version = elems[2];
            buffer.erase(0, line.length() + std::strlen(CRLF));
            status = StartLineParsed;
        }
        else
            status = Error;
    }
}

void      Request::parse_headers() {
    std::string                 line;
    size_t                      start;
    size_t                      end;
    size_t                      nl;
    
    nl = buffer.find(CRLF);
    line = buffer.substr(0, nl);
    if (line.empty())
    {
        if (headers.count("Content-Length") || headers.count("Transfer-Encoding"))
        {
            status = HeadersParsed;
        }
        else
            status = Complete;
    }
    else
    {
        start = line.find_first_not_of(" \t");
        end = line.find_first_of(": \t");
        std::string key = line.substr(start, end - start);
        std::cout << "key : " << key << " in " << buffer <<'\n';

        start = line.find_first_not_of(": \t", end);
        end = line.find_first_of(" \t", start);
        std::string value = line.substr(start, end - start);
        headers.insert(std::make_pair(key, value));
    }
    buffer.erase(0, line.length() + std::strlen(CRLF));
}

void      Request::parse_in_chunk() {
    if (buffer_size < 0)         //parsing length of line
    {
        size_t nl = buffer.find(CRLF);
        // if (nl == std::string::npos)
        //     return Error;
        std::string n = buffer.substr(0, nl);
        if (!is_digits(n))
        {
            status = Error;
        }
        else
        {
            buffer_size = strtoi(n);
            buffer.erase(0, nl + std::strlen(CRLF));
        }
    }
    else if (buffer_size > 0)                                 //parsing content line
    {
        std::stringstream ss;
        ss.clear();
        ss << std::hex << buffer;
        body += buffer.substr(0, buffer_size);
        buffer.erase(0, buffer_size + std::strlen(CRLF));
        buffer_size = -1;
    }
    else
        status = Complete;
}

void      Request::parse_body() {
    if (headers.count("Transfer-Encoding"))
    {
        if (headers["Transfer-Encoding"].compare("chunked") == 0) //TODO: transfer-encoding: deflate zip chunked
            parse_in_chunk();
    }
    else if (headers.count("Content-Length"))
    {
        int length = strtoi(headers["Content-Length"]) - static_cast<int>(body.size());
        if (length < 0)
        {
            status = Error;
        }
        else
        {
            body += buffer.substr(0, length);
            buffer.erase(0, length);
            if (body.size() == strtoi(headers["Content-Length"]))
                status = BodyParsed;
        }
    }
    else
        status = Error;             //close connection
}

// Request::putvalue(Request req)
// {
// 	type = req.type;
// 	path = req.path;
// 	http_version = req.http_version;
// 	query_string = req.query_string;
// 	body = req.body;
// 	host = req.host;
// 	UserAgent = req.UserAgent;
// 	Accept = req.Accept;
// 	AcceptLanguage = req.AcceptLanguage;
// 	AcceptEncoding = req.AcceptEncoding;
// 	connection = req.connection;
// 	map = req.map;
// 	complete = req.complete; 
// }

void		Request::printvalue()
{
    std::cout << "-------------- REQUEST --------------" << std::endl;
    std::cout << method << " " << path << " " << query_string << " " << http_version << std::endl;
    for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
        std::cout << it->first << " " << it->second << std::endl;
    std::cout << body << std::endl;
    std::cout << "------------------------------------" << std::endl;
}

Request::~Request()
{}

std::pair<bool, std::string>    Request::env_querystring() const
{
    if (method.compare("POST") == 0) //DELETE a une query string ?
        return (std::make_pair(false, std::string()));
    std::string result = "QUERY_STRING=" + query_string;
    return std::make_pair(true, result);
}

// std::string               &Request::get_content_type()
// {
//     // if (headers.count("Content-Type"))
//         return headers["Content-Type"];
//     // return 
// }

std::pair<bool, std::string>    Request::env_method() const
{
    return std::make_pair(true, std::string("REQUEST_METHOD=" + method));
}

// std::pair<bool, std::string>    Request::env_content_type()
// {
//     if (method.compare("POST") != 0)
//         return std::make_pair(false, std::string());
//     std::string result = "CONTENT_TYPE=" + content_type;
//     return std::make_pair(true, result);
// }

// std::pair<bool, std::string>    Request::env_content_length()
// {
//     std::string length;
//     std::string result;
//     if (method.compare("POST") != 0)
//         return std::make_pair(false, std::string());
//     length = strtoi(headers["Content-Length"]);
//     result = "CONTENT_LENGTH=" + length;
//     return std::make_pair(true, result);
// }

// int     Request::get_content_length() const
// {
//     if (!headers.count("Content-Length"))
//         return (-1);
//     return strtoi(headers["Content-Length"]);
// }

// void remove_nl(char *buf, int to_read)
// {
//     if (to_read < 0)
//     {
//         if (std::strlen(buf) >= 1 && buf[std::strlen(buf) - 1] == '\n')
//         {
//             buf[std::strlen(buf) - 1] = 0;
//             if (std::strlen(buf) >= 1 && buf[std::strlen(buf) - 1] == '\r')
//                 buf[std::strlen(buf) - 1] = 0;
//         }
//     }
// }

// void    Request::parse(Client &client, char *buf)
// {
//     std::vector<std::string> split = ft_split(buf);
//     //remove_nl(buf, to_read);
//     if (!body_read && split.size() > 0 && (split[0] == "GET" || split[0] == "POST" || split[0] == "DELETE"))
//     {
//         if (split[0] == "GET")
//             method= 1;
//         else if (split[0] == "POST")
//             method= 2;
//         else if (split[0] == "DELETE")
//             method= 3;
//         if (split[1].find('?', 0) != std::string::npos)
//         {
//             path = split[1].substr(0, split[1].find('?', 0));
//             if (split[1].find('?') != split[1].size() - 1)
//                 query_string = split[1].substr(split[1].find('?', 0) + 1, split[1].size());
            
//         }
//         else
//             path = split[1];
//         http_version = split[2];

//         if (!method|| path[0] != '/' || http_version != "HTTP/1.1")
//         {
//             throw Webserver::WebserverException(400);
//         }

//     }
//     else if (!body_read && split.size() > 0)
//     {
//         if (split[0] == "Host:")
//         {
//             host = combine_split(split, 1);//split[1];
//         }
//         else if (split[0] == "User-Agent:")
//             UserAgent = combine_split(split, 1);
//         else if (split[0] == "Accept:")
//             Accept = combine_split(split, 1);
//         else if (split[0] == "Accept-Language:")
//             AcceptLanguage = combine_split(split, 1);
//         else if (split[0] == "Accept-Encoding:")
//             AcceptEncoding = combine_split(split, 1);
//         else if (split[0] == "Connection:")
//             connection = split.size() == 2 && split[1] == "keep-alive" ? true : false;
//         else if (split[0] == "Transfer-Encoding:")
//             chunked = combine_split(split, 1) == "chunked" ? true : false;
//         else if (split[0][split[0].size() - 1] == ':')
//             map[split[0].substr(0, split[0].size() - 1)] = combine_split(split, 1);
//     }
//     else if (!body_read && !split.size())
//     {
//         if (host.size() <= 0)
//         {
//             throw Webserver::WebserverException(400);
//         }
//         int tmp;
//         std::stringstream ss;
//         ss.clear();
//         if (!chunked && !map.count("Content-Length"))
//         {
//             complete = true;
//             return;
//         }
//         if (!get_Content_Length())
//         {
//             complete = true;
//             return;
//         }
//         body_read = true;
//     }
//     else if (body_read)
//     {
//         if (chunked)
//         {
//             std::stringstream ss;
//             ss.clear();

//             if (to_read < 0)
//             {
//                 ss << std::hex << buf;
//                 ss >> to_read;
//             }
//             else if (to_read > 0)
//             {
//                 if (std::string(buf).size() > to_read)
//                     buf[to_read] = 0;
//                 body += std::string(buf).substr(0, to_read);
//                 to_read -= std::string(buf).substr(0, to_read).size();
//                 to_read = to_read > 0 ? to_read : -1;
//             }
//             else
//             {
//                 complete = true;
//             }
            

//         }
//         else// if (body.size() < get_Content_Length())
//         {
//             body += std::string(buf);
//             if (body.size() >= get_Content_Length())
//             {
//                 body = body.substr(0, get_Content_Length());
//                 complete = true;
//             }
//         }
//     }
//     return;
// }
