#include "Location.hpp"

Location::Location() :
    methods(),
    route(std::string("")),
    redirect(),
    root(),
    listing(false),
    index(),
    upload_route(),
    auth_basic(""),
    limit_except(std::string(""))
{}

Location::~Location()
{
    /*if (redirect)
        delete redirect;
    if (root)
        delete root;
    if (index)
        delete index;
    if (upload_route)
        delete upload_route;
    */
    clear();
}

Location &Location::operator=(const Location &ref)
{
	clear();
    methods = ref.methods;
    route = ref.route;
    redirect = ref.redirect ? new std::pair<int, std::string>(*ref.redirect) : 0;
    root = ref.root ? new std::string(*ref.root) : 0;
    listing = ref.listing;
    index = ref.index ? new std::string(*ref.index) : 0;
    upload_route = ref.upload_route ? new std::string(*ref.upload_route) : 0;
    cgi_extension = ref.cgi_extension;
    auth_basic = ref.auth_basic;
    limit_except = ref.limit_except;
    return *this;
}

void Location::print(void)
{
    std::cout << "\t\t\t\troute : " << route <<'\n';
    std::cout << "\t\t\t\tmethods: " <<(methods & GET_d ? "GET" : "") << " " << (methods & POST_d ? "POST" : "") << " " << (methods & DELETE_d ? "DELETE" : "") << '\n';
    std::cout << "\t\t\t\tredirect : " << (redirect ? itostr(redirect->first) + " " + redirect->second : "") << '\n';
    std::cout << "\t\t\t\troot : " << (root ? *root : "") << '\n';
    std::cout << "\t\t\t\tlisting : " << listing << '\n';
    std::cout << "\t\t\t\tindex : " << (index ? *index : "") << '\n';
    std::cout << "\t\t\t\tupload_route : " << (upload_route ? *upload_route : "") << '\n';
    std::cout << "\t\t\t\tCGI-extension : ";
    for (size_t i = 0; i < cgi_extension.size(); i++)
        std::cout << cgi_extension[i] << " ";
    std::cout << '\n';
}

void Location::clear(void)
{
    if (root)
        delete root;
    if (index)
        delete index;
    if (upload_route)
        delete upload_route;
    if (redirect)
        delete redirect;
}

Location::Location(const Location &ref)
:
methods(ref.methods),
route(ref.route),
redirect(ref.redirect ? new std::pair<int, std::string>(*ref.redirect) : 0),
root(ref.root ? new std::string(*ref.root) : 0),
listing(ref.listing),
index(ref.index ? new std::string(*ref.index) : 0),
upload_route(ref.upload_route ? new std::string(*ref.upload_route) : 0),
cgi_extension(ref.cgi_extension)
{}
