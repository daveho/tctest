#include <cctype>
#include <stdexcept>
#include "demofn_cplusplus.h"

void convert_to_lower(std::string &s) {
	for (unsigned i = 1; i < s.size(); ++i) {
		if (s[i] == 'J')
			throw std::runtime_error("J causes exception for some reason!");
		s[i] = tolower(s[i]);
	}
}
