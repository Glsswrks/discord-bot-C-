#ifndef __COMMAND_HH__
#define __COMMAND_HH__
#include <string>
#include <vector>
#include <stdint.h>
#include <dpp/dpp.h>
namespace cluster::commands
{
	class command_t {
	public:
		std::string name;
		std::vector<std::string> options;
		dpp::interaction_create_t ctx;
	};
}
#endif // !__COMMAND_HH__
