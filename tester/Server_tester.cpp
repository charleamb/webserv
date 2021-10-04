#include "gtest/gtest.h"
#include "../src/networking/Server.hpp"


TEST(ServerFunctionTests, match_wildcard) {
    Server test;
    std::string startingWildcardHost = "*.example.org";
    std::string endingWildcardHost = "www.example.*";
    std::string hostField = "www.example.org";

    EXPECT_EQ(test.match_wildcard(startingWildcardHost, hostField), 12);
    EXPECT_EQ(test.match_wildcard(endingWildcardHost, hostField), -12);
}

TEST(ServerFunctionTests, find_hostname) {
    Server test;
    std::string startingWildcardHost = "*.example.org";
    std::string endingWildcardHost = "www.example.*";
    std::vector<std::string> tests;

    tests.push_back(startingWildcardHost);
    tests.push_back(endingWildcardHost);

    std::string hostField = "www.example.org";

    size_t index = 0;
    int oldMatchLevel = 0;
    for (size_t i = 0; i < tests.size(); i++) {
        int matchLevel = test.match_wildcard(tests[i], hostField);
	    if (matchLevel > 0 && matchLevel > oldMatchLevel || (oldMatchLevel < 0 && matchLevel < oldMatchLevel)) {
		    index = i;
		    oldMatchLevel = matchLevel;
	    }
    }
    EXPECT_EQ(oldMatchLevel, 12);
    EXPECT_EQ(index, 0);
}