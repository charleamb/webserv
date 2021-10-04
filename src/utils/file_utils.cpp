#include "utils.hpp"

bool		file_exists(const std::string &path) {
	if (access(path.c_str(), F_OK) == 0)
		return true;
	return false;	
}

bool		file_is_executable(const std::string &path) {
	if (access(path.c_str(), X_OK) == 0)
		return true;
	return false;	
}

bool		file_is_readable(const std::string &path) {
	if (access(path.c_str(), R_OK) == 0)
		return true;
	return false;	
}

bool is_directory(const std::string &path)
{
    struct stat tmp;
    if (stat(path.c_str(), &tmp) == -1)
    {
        return false;
    }
    return (tmp.st_mode & S_IFDIR);
}
