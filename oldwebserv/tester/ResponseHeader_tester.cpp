#include "../src/networking/ResponseHeader.hpp"
#include <string>
#include "gtest/gtest.h"

TEST(ResponseHeaderTest, GetMimeType) {
	std::string type;
	ResponseHeader	header;

	type = header.get_mime_type("json");
	EXPECT_EQ(type, std::string("application/json"));
}