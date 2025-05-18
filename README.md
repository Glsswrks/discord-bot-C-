# Discord-Bot pure C++
A simple C++ **Discord-Bot** project using **DPP (Discord++)** Library.

### Features:

* [x] Blocks over 2,000 swear words by default.
* [x] Supports a custom prefix command.
* [x] Discord bot presence.
* [x] Clean and readable code.
* [x] No memory leak with my default code.
* [x] Content interaction event handler.
* [ ] Slash Command event handler.
* [ ] Button action event handler.

### Feature Details:
Here's a breakdown of how each things works:

* **Swear-Words:** The bot will deserialize the words that store in **"Vendor/swearwords.bin"** path directory, once the constructor being create. Every content that send by User will be check within the event handler. Once it's found one or more of the swear-words being use inside the Container that we stored, it will automatically delete the message. (SUGGESTION: Limit the amount of swear-words for each user).
* **Custom Command:** I decided to use a custom prefix command instead of **Discord** Slashcommand. Because i've seen that most of the discord-bot inside the server i've joined before, they are using custom prefix command instead. And it's also easy to manage than the slashcommand itself. But it just my preference. You can implement the Slashcommand Event Handler on your own by calling the slashcommand inside the **Initialize** Function.
* **Bot-Presence:** Is discord bot status just our **Discord Profile**, You can custom base on your need, inside **"ready_t()"** Function. And i didn't implement anything else inside that function. Just the discord bot presence, You can implement more of your need.

### Thought:
This is the project i create just for fun, and i just create a basic structor for y'all to start with. I also name each function base on it's used. You can understand and use easily. There's many more things you have to do like, add more command, and a checker for swear word that being use more than 3-4 time, you can mute them or something. so basically, just implement what you need Lolz.

### Library Credit:
* **Nlohmann-JSON:** (https://github.com/nlohmann/json.git)
* **DPP, (Discord++):** (https://github.com/brainboxdotcc/DPP.git)
* **Swear Words:** (https://github.com/zacanger/profane-words.git)
* **Binary-Reader-Writer:** (https://github.com/Glsswrks/binary-write-and-read.git)
