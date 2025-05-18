#include <iostream>
#include <cluster/cluster.hh>
#include <memory.hh>
#include <nlohmann/json.hpp>
#include <fstream>

int main(int argc, char* argv[])
{	
	using namespace cluster;
	discord_bot* bot = new discord_bot({ 0x0, "TOKEN" });
	bot->initialize();

	while (true);

	return EXIT_SUCCESS;
}