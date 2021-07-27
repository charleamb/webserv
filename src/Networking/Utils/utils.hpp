#ifndef UTILS_HPP
# define UTILS_HPP

# include <unistd.h>
# include <string>

bool	is_file(const std::string path);
bool	is_perm(const std::string path);
#endif