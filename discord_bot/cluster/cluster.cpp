#include <cluster/cluster.hh>
#include <cluster/utility.hh>
#include <memory.hh>
namespace cluster
{
	discord_bot::discord_bot(config configuration)
	{
		// assigned the sid and token
		m_cluster.second = configuration;

		m_cluster.first = new (std::nothrow) dpp::cluster(m_cluster.second.token, dpp::i_all_intents);
		if (m_cluster.first != NULL) {
			using namespace utility;
			std::ifstream ifs("vendor/swearwords.bin", std::ios::binary);
			if (ifs.is_open()) {
				ifs.seekg(0, std::ios::end);
				std::unique_ptr<uint8_t[]> buffer(new uint8_t[ifs.tellg()]);
				ifs.seekg(0, std::ios::beg);
				uintmax_t size = ifs.tellg();
				ifs.read(reinterpret_cast<char*>(buffer.get()), size);
				ifs.close();
				binary_reader br = binary_reader(buffer.get(), size);
				br.reverse();
				uint32_t amount = br.get_uint();
				for (uint32_t i = 0; i < amount; i++) {
					std::string str = br.get_string();
					m_cluster.second.swear_words.insert(str);
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
			std::cerr << "bot cannot be create." << std::endl;
			return nullptr;
		}

		bot->on_interaction_create([&](const dpp::interaction_create_t& ctx) {
			this->interaction_create_t(ctx);
			});

		bot->on_ready([&](const dpp::ready_t& ctx) {
			this->ready_t(ctx);
			});

		bot->start(dpp::st_wait);

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
				"Coding"
			));

	}
	void discord_bot::interaction_create_t(const dpp::interaction_create_t& ctx)
	{
		dpp::cluster* bot = m_cluster.first;
		const std::string& content = ctx.command.msg.content;
		auto& context = ctx.command;
		
		// if message from the bot, did nothing
		if (context.msg.author.id == bot->me.id) {
			return;
		}

		// get some basic from the content sender
		dpp::user user = context.usr;
		dpp::guild_member member = context.member;
		dpp::snowflake channelID = context.channel_id;

		if (content.length() != 0x0) {
			if (is_swearwords(content)) {
				// delete message if swear words are found using
				bot->message_delete(
					context.msg.id,
					context.msg.channel_id
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
			if (m_cluster.second.swear_words.find(word) != m_cluster.second.swear_words.end()) {
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