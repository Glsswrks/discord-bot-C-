#include <cluster/cluster.hh>
#include <cluster/utility.hh>
#include <memory.hh>
namespace cluster
{
	discord_bot::discord_bot(config configuration)
	{
		// assigned the sid and token
		m_cluster.second = configuration;

		m_cluster.first = new dpp::cluster(m_cluster.second.token, dpp::i_all_intents);

		if (m_cluster.first != NULL) {
			using namespace utility;
			std::ifstream ifs("vendor/swearwords.bin", std::ios::binary);
			if (ifs.is_open()) {
				ifs.seekg(0, std::ios::end);
				uintmax_t size = ifs.tellg();
				ifs.seekg(0, std::ios::beg);
				std::unique_ptr<uint8_t[]> buffer = std::make_unique<uint8_t[]>(size);
				ifs.read(reinterpret_cast<char*>(buffer.get()), size);
				ifs.close();
				binary_reader br = binary_reader(buffer.get(), size);
				br.reverse();
				uint32_t amount = br.get_uint();
				for (uint32_t i = 0; i < amount; i++) {
					std::string str = br.get_string();
					m_cluster.second.insert(str);
				}
				br.cleanup();
			} else {
				std::cout << "cannot open swearwords.bin" << std::endl;
			}
		}
	}
	discord_bot::~discord_bot()
	{
		if (m_cluster.first != nullptr) {
			delete m_cluster.first;
			m_cluster.first = nullptr;
		}
	}
	const discord_bot* discord_bot::initialize()
	{
		dpp::cluster* bot = m_cluster.first;
		if (bot == NULL) {
			std::cout << "bot cannot be create." << std::endl;
			return nullptr;
		}

		bot->on_message_create([&](const dpp::message_create_t& ctx) {
			this->message_create_t(ctx);
			});

		bot->on_ready([&](const dpp::ready_t& ctx) {
			this->ready_t(ctx);
			});

		bot->start();

		return this;
	}
	void discord_bot::ready_t(const dpp::ready_t& ctx)
	{
		// assigned a new local variable for short typing
		dpp::cluster* bot = m_cluster.first;

		// set your discord bot status, however you like
		if (dpp::run_once<struct register_bot>())
			bot->set_presence(dpp::presence(
				dpp::presence_status::ps_idle,
				dpp::activity_type::at_custom,
				"Playing Visual Studio 2022"
			));

	}
	void discord_bot::message_create_t(const dpp::message_create_t& ctx)
	{
		if (ctx.msg.author.id == m_cluster.first->me.id)
			return;
		dpp::cluster* bot = m_cluster.first;
		const std::string& content = ctx.msg.content;
		auto& context = ctx.msg;

		// get some basic from the content sender
		dpp::user user = context.author;
		dpp::guild_member member = context.member;
		dpp::snowflake channelID = context.channel_id;

		if (content.size() != 0x0) {
			if (is_swearwords(content)) {
				// delete message if swear words are found using
				bot->message_delete(
					context.id,
					context.channel_id
				);
				return;
			} if (is_command(content)) {
				using namespace utils;
				std::vector<std::string> args = utils::split(content.substr(1), ' ');
				std::string command = utils::tolower(args.at(0));
				commands::command_t cmd;
				cmd.name = command;
				cmd.ctx = ctx;
				cmd.options = std::vector<std::string>(args.begin() + 1, args.end());
				send_command(cmd);
			} else {
				// TODO: implement your own stuff to do with all the message
			}
		}
	}
	void discord_bot::send_command(commands::command_t& cmd)
	{
		using namespace utils;
		std::string command = cmd.name;
		uint32_t hash = quick_hash(command.c_str());

		//TODO: implement your own command here
		switch (hash) {
		case "ping"_qh: {
			cmd.ctx.reply("Pong!");
			break;
		} default:
			break;
		}
	}
	bool discord_bot::is_swearwords(const std::string& content)
	{
		std::istringstream iss(content);
		std::string word;
		while (iss >> word) {
			if (m_cluster.second.swearwords().find(word) != m_cluster.second.swearwords().end()) {
				return true;
			}
		}
		return false;
	}
	bool discord_bot::is_command(const std::string& content)
	{
		// command prefix and checker
		// our prefix is '!' and we check if content not just prefix
		// to prevent compiler throwing an exception
		return (content.size() >= 2 && content.at(0) == '!') ? true : false;
	}
}
