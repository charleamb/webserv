#include "gtest/gtest.h"
#include "../src/networking/Client.hpp"
#include "../src/networking/Server.hpp"
#include <string>
#include <vector>
TEST(ClientTest, Constructor) {
	std::map<int, Client>	clients;
	Server					server;

	EXPECT_TRUE(clients.empty());
	Client client(13, &server);
	clients.insert(std::make_pair(13, client));
	EXPECT_EQ(clients.size(), 1);
}


TEST(ClientTest, getMatchingLocation) {

	std::string route1 = "/";
	std::string route2 = "/bin/";
	std::string path = "/bin/lol";

	std::vector<std::string>	tests;
	tests.push_back(route2);

	tests.push_back(route1);
	size_t			oldMatchLevel;
	size_t index = 0;
	oldMatchLevel = 0;

	for (size_t i = 0; i < tests.size(); i++) {
		//on check les string literal locations
		if (path.find(tests[i]) != std::string::npos) {
			size_t matchLevel = tests[i].size();
			if (matchLevel > oldMatchLevel) {
				oldMatchLevel = matchLevel;
				index = i;
			}
		}
		else if (oldMatchLevel < 2 && tests[i][0] == '~') {
			//regex parsing
			std::cout << "TODO: regex parsing" << std::endl;
		}
	}
	EXPECT_EQ(5, oldMatchLevel);
	EXPECT_EQ(0, index);
}