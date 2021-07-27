# include "utils.hpp"

bool	is_file(const std::string path) {
	return !access(path.c_str(), F_OK);
}

bool	is_perm(const std::string path) {
	return !access(path.c_str(), W_OK & X_OK); //if can write and can execute then can delete
}