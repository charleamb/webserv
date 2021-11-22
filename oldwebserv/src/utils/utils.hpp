#ifndef UTILS_HPP
# define UTILS_HPP

# include <sstream>
# include <fstream>
# include <unistd.h>
# include <string>
# include <fcntl.h>
# include <sys/stat.h>
# include <iostream>
# include <vector>

# define BUFFER_SIZE 1024
# define CRLF "\r\n"

std::vector<std::string> ft_split(std::string str, std::string charset = "");
size_t              length_split(const char *str, const std::string charset = "");

bool                is_inside_root(std::string path, std::string root);
std::string         splitToPath(std::vector<std::string> split);
std::string         strToPath(std::string str);


bool                str_is_digit(const std::string &tmp);
void                delete_comments(std::string &tmp);
std::string         message_error(std::string msg, int ligne);
bool		        file_is_readable(const std::string &path);
std::string         combine_split(std::vector<std::string> split, int index);
std::string         get_extension(std::string str);
bool		        file_is_executable(const std::string &path);
bool		        file_exists(const std::string &path);
std::string	        read_file(const char *filename);
std::string 	    itostr(int n);
std::string	        replace(std::string &string, const std::string &value_to_find, const std::string &value_to_replace);
void                replace_once(std::string &newPath, const std::string &to_remove, const std::string &to_add);
bool		        file_exists(const std::string &path);
int			        strtoi(const std::string &s);
bool                is_digits(const std::string &str);
bool                is_directory(const std::string &str);

template<typename T>
bool find_in_vector(const std::vector<T> &vector, const T &value)
{
	for (size_t i = 0; i < vector.size(); i++)
		if (vector[i] == value)
			return true;
	return false;
}

#endif
