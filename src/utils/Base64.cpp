#include "Base64.hpp"

static const std::string base64_valid_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

bool                ft::Base64::is_base64(const unsigned char c) {
    if (isalnum(c) || c == '+' || c == '/' || c == '=')
        return true;
    return false;
}

const std::string   ft::Base64::decode(std::string &encoded_string) {
    
    int x = 0;
	int j = 0;
	int y = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (encoded_string[y] && encoded_string[y] != '=' && is_base64(encoded_string[y])) 
	{
        if (encoded_string[y] == '=')
            break ;
		char_array_4[x++] = encoded_string[y];
		y++;
		if (x == 4)
		{
			for (x = 0; x < 4; x++)
				char_array_4[x] = base64_valid_chars.find(char_array_4[x]);
			
			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (x = 0; (x < 3); x++)
				ret += char_array_3[x];
			x = 0;
		}
	}
	if (x) //not divisible par 4
	{
		for (j = x; j < 4; j++)
			char_array_4[j] = 0;

		for (j = 0; j < 4; j++)
			char_array_4[j] = base64_valid_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < x - 1); j++)
            ret += char_array_3[j];
	}
	return ret;
}

// std::vector<int>    ft::Base64::stringToBinary(std::string &s) {
//     std::vector<int> bin;

//     for (size_t x = 0; x < s.size(); x++) {
//         for (size_t y = 0; y < 8; y++) {
//             int bit = !!((s[x] << y) & 0x80);
//             bin.push_back(bit);
//         }
//     }
//     return bin;
// }

// std::vector<int>               ft::Base64::charToBin(unsigned char c) {
//     std::vector<int> byte;
//     int bit;

//     for (int i = 0; i < NB_BITS_IN_BYTE; i++) {
//         bit = !!((c << i) & 0x80);
//         byte.push_back(bit);
//     }
//     return byte;
// }

// const std::string   ft::Base64::encode(std::string &clear_string) {
//     const unsigned char *src = reinterpret_cast<unsigned char *>(&clear_string);
//     size_t len = clear_string.size();
//     unsigned char *out, *pos;
//     const unsigned char *end, *in;

//     size_t olen = 4*((len + 2) / 3); /* 3-byte blocks to 4-byte */

//     if (olen < len)
//         return std::string(); /* integer overflow */

//     std::string outStr;
//     outStr.resize(olen);
//     out = (unsigned char*)&outStr[0];

//     end = src + len;
//     in = src;
//     pos = out;
//     while (end - in >= 3) {
//         *pos++ = base64_valid_chars[in[0] >> 2];
//         *pos++ = base64_valid_chars[((in[0] & 0x03) << 4) | (in[1] >> 4)];
//         *pos++ = base64_valid_chars[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
//         *pos++ = base64_valid_chars[in[2] & 0x3f];
//         in += 3;
//     }

//     if (end - in) {
//         *pos++ = base64_valid_chars[in[0] >> 2];
//         if (end - in == 1) {
//             *pos++ = base64_valid_chars[(in[0] & 0x03) << 4];
//             *pos++ = '=';
//         }
//         else {
//             *pos++ = base64_valid_chars[((in[0] & 0x03) << 4) |
//                 (in[1] >> 4)];
//             *pos++ = base64_valid_chars[(in[1] & 0x0f) << 2];
//         }
//         *pos++ = '=';
//     }
//     return outStr;
// }