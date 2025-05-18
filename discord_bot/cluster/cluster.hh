#ifndef __CLUSTER_HH__
#define __CLUSTER_HH__
#include <string>
#include <cstdlib>
#include <cstdint>
#include <sstream>
#include <memory>
#include <fstream>
#include <algorithm>
#include <unordered_set>

#include <dpp/dpp.h>
#include <cluster/command.hh>

namespace cluster
{
	class config {
	public:
		uint64_t serverID;
		std::string token;
		std::unordered_set<std::string> swear_words = {};
	};

	class discord_bot {
	public:
		explicit discord_bot() = default;
		explicit discord_bot(config configuration);
		~discord_bot();
	public:
		const discord_bot* initialize();
	public:
		void ready_t(const dpp::ready_t& ctx);
		void interaction_create_t(const dpp::interaction_create_t& ctx);
	public:
		void send_command(commands::command_t& cmd);
	public:
		bool is_swearwords(const std::string& content);
		bool is_command(const std::string& content);
	private:
		std::pair<dpp::cluster*, config> m_cluster = {};
	};
}
#endif __CLUSTER_HH__ // !__CLUSTER_HH__