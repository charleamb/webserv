#include "utils.hpp"

std::string	replace(std::string &string, const std::string &value_to_find, const std::string &value_to_replace) {
	size_t pos = 0;

	while ((pos = string.find(value_to_find, pos)) != std::string::npos)
	{
		string.replace(pos, value_to_find.length(), value_to_replace);
		pos += value_to_replace.length();
	}
	return string;
}

void    replace_once(std::string &newPath, const std::string &to_remove, const std::string &to_add)
{
    if (newPath.find(to_remove.c_str(), 0, to_remove.size()) != std::string::npos)
	{
		newPath.erase(0, to_remove.size());
		newPath.insert(0, std::string(to_add));
	}
}

bool		is_capitalized(const std::string &word) {
	for (size_t i = 0; i < word.length(); i++) {
		if (!isupper(static_cast<unsigned char>(word[i])))
				return false;
	}
	return true;
}

bool is_digits(const std::string &str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}