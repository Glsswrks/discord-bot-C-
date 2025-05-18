#include <cluster/utility.hh>
namespace utils
{
	std::vector<std::string> split(const std::string& str, char delimiter)
	{
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string item;
        while (std::getline(ss, item, delimiter)) {
            tokens.push_back(item);
        }
        return tokens;
	}
    std::string tolower(const std::string& content)
    {
		std::string result = content;
		std::transform(result.begin(), result.end(), result.begin(), ::tolower);
		return result;
    }
    std::string toupper(const std::string& content)
    {
        std::string result = content;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }
}