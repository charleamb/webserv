#ifndef BASE64_HPP
# define BASE64_HPP
# include <string>
# include <iostream>
# include <vector>
# include <stdio.h>
# define NB_BITS_IN_BYTE 8
namespace ft {
    class Base64 {
        public:
        static bool                                  is_base64(const unsigned char c);
        static const std::string                    decode(std::string &encoded_string);
        // static std::vector<int>                    charToBin(unsigned char c);
        // static const std::string                encode(std::string &clear_string);
        // static std::vector<int>                stringToBinary(std::string &s);
    };
}
#endif