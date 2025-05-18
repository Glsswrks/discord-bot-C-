#ifndef __UTILITY_HH__
#define __UTILITY_HH__
#include <string>
#include <vector>
#include <sstream>
#include <cstdint>
#include <algorithm>
namespace utils
{
    std::vector<std::string> split(const std::string& str, char delimiter);

	std::string tolower(const std::string& content);
	std::string toupper(const std::string& content);

	constexpr uint32_t quick_hash(const char* str, size_t hash = 2166136261u) {
		return (*str) ? quick_hash(str + 1, (hash ^ *str) * 16777619u) : hash;
	}

	constexpr uint32_t operator ""_qh(const char* str, size_t) {
		return quick_hash(str);
	}
}
#endif // !__UTILITY_HH__
