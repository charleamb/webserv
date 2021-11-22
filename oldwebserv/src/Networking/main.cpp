# include <ctime>
# include <sys/time.h>
# include <iostream>
# include <string>
int main() {
	std::string file = "test.css";
	size_t found = file.find_last_of(".");
	if (found != std::string::npos) {
		std::string extension = file.substr(found + 1, file.size() - found - 1);
		if (extension.compare("html") == 0 || extension.compare("css") == 0 || extension.compare("js") == 0 )
			std::cout << std::string("text/" + extension) << std::endl;
		std::cout << found << std::endl;
		std::cout << file.size() - found << std::endl;
		// if (file.compare(found + 1, ))
	}

}