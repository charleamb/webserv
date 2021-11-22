//
// Created by alena on 08/07/2021.
//

#include "Location.hpp"
#include "parsing/Config/config.hpp"

/*
 * LocationConfig
 */
LocationConfig::LocationConfig():
	_path(""),
	_methods(methods::GET),
	_auto_index(true),
	_upload(false),
	_body_size(SIZE_UNSET),
	_root("")
	{ }

void	LocationConfig::use(LocationConfig *ptr) { LocationConfig::active = ptr; }

/*
 * GETTER
 */
LocationConfig	*LocationConfig::path(slice path)
{
	active->set_path(path);
	return active;
}

LocationConfig	*LocationConfig::root(slice root)
{
	active->set_root(root);
	return active;
}

LocationConfig	*LocationConfig::body_size(size_t size)
{
	active->set_size(size);
	return active;
}

LocationConfig	*LocationConfig::auto_index(bool auto_index)
{
	active->set_autoindex(auto_index);
	return active;
}

LocationConfig	*LocationConfig::upload(bool upload)
{
	active->set_upload(upload);
	return active;
}

LocationConfig	*LocationConfig::methods(methods::Methods methods)
{
	active->set_methods(methods);
	return active;
}

LocationConfig	*LocationConfig::index(slice index)
{
	active->set_index(index);
	return active;
}

LocationConfig	*LocationConfig::redirection(redirect ret)
{
	active->set_redirect(ret);
	return active;
}

LocationConfig	*LocationConfig::dump(slice unused)
{
	(void)unused;
	return active;
}

/*
 * SETTER
 */
LocationConfig	&LocationConfig::set_path(slice path)
{
	this->_path = path.to_string();
	return *this;
}

LocationConfig	&LocationConfig::set_size(size_t size)
{
	this->_body_size = size;
	return *this;
}

LocationConfig	&LocationConfig::set_root(slice root)
{
	this->_root = root.to_string();
	return *this;
}

LocationConfig	&LocationConfig::set_autoindex(bool index)
{
	this->_auto_index = index;
	return *this;
}

LocationConfig	&LocationConfig::set_upload(bool upload)
{
	this->_upload = upload;
	return *this;
}

LocationConfig	&LocationConfig::set_methods(methods::Methods methods)
{
	this->_methods = methods;
	return *this;
}

LocationConfig	&LocationConfig::set_index(slice index)
{
	this->_index = index.to_string();
	return *this;
}

LocationConfig	&LocationConfig::set_redirect(redirect ret)
{
	this->_redirect = ret;
	return *this;
}

std::ostream& operator<<(std::ostream& stream, const LocationConfig& cfg)
{
	stream << MAGENTA << "Location : " << cfg._path << NC << std::endl
		   << BLUE << "root : " << cfg._root << std::endl
		   << "methods : "<< cfg._methods << std::endl
		   << "autoindex : " << (cfg._auto_index ? "on" : "off") << std::endl
		   << "upload : " << (cfg._upload ? "on" : "off") << std::endl
		   << "body_size : ";
	if (cfg._body_size == SIZE_UNSET)
		stream << "unset" << std::endl;
	else
		stream << cfg._body_size << std::endl;
	stream << "index : " << cfg._index << std::endl
		   << "Redirect : " << cfg._redirect.status << " | " << cfg._redirect.uri << NC << std::endl;
	return stream;
}

/*
 * LocationContent
 */
LocationContent::LocationContent() { }

LocationContent::result_type	LocationContent::operator()(const slice &input)
{
	LocationConfig	cfg;
	LocationConfig::use(&cfg);

	ParserResult<slice>	res = as_slice(many(alt(
			as_slice(map(directive(ClientMaxBodySize()), LocationConfig::body_size)),
			as_slice(map(directive(Root()), LocationConfig::root)),
			as_slice(map(directive(config::Methods()), LocationConfig::methods)),
			as_slice(map(directive(AutoIndex()), LocationConfig::auto_index)),
			as_slice(map(directive(Upload()), LocationConfig::upload)),
			as_slice(map(directive(Index()), LocationConfig::index)),
			as_slice(map(directive(Redirect()), LocationConfig::redirection)),
			alt(
					as_slice(map(take_with(newline), LocationConfig::dump)),
					as_slice(map(comment, LocationConfig::dump))
			)
	)))(input);
	return res.map(cfg);
}

/*
 * Head
 */
Head::Head() { }

Head::result_type Head::operator()(const slice &input)
{
	return preceded(sequence(Tag("Location"), rws), take_until_match(rws))(input);
}

/*
 * Location = location RWS path RWS { . . . }
 */
Location::Location() { }

Location::result_type Location::operator()(const slice &input)
{
	ParserResult<tuple<slice, LocationConfig> >	res = bloc(Head(), LocationContent())(input);
	if (res.is_err())
		return res.convert<LocationConfig>();
	return res.map(res.unwrap().second.set_path(res.unwrap().first));
}
