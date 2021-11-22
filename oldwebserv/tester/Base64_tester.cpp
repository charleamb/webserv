#include "gtest/gtest.h"
#include "../src/utils/Base64.cpp"

TEST(Base64Test, Decode) {
    std::string encoded("GEhlbGxvIFdvcmxk"); //SGVsbG8gV29ybGQhCg

    std::cout << "encoded = " << encoded << std::endl;

    std::string decoded = ft::Base64::decode(encoded);
    std::cout << "decoded = " << decoded << std::endl;
}

// TEST(Base64Test, Encode) {
//     std::string clear("Hello World!");

//     std::cout << "clear = " << clear << std::endl;

//     std::string encoded = ft::Base64::encode(clear);
//     std::cout << "encoded = " << encoded << std::endl;
// }