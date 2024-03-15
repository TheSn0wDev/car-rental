#include "../includes/common.h"
using namespace std;

pair<string, map<string, string>> parseInput(const string& input) {
    map<string, string> argsMap;
    istringstream iss(input);
    string buffer;
    vector<string> words;

    while (iss >> buffer) {
        words.push_back(buffer);
    }

    // start at 1 to skip the command name
    for (size_t i = 1; i < words.size(); ++i) {
        if (words[i].size() > 1 && words[i][0] == '-') {
            if (i + 1 < words.size()) {
                argsMap[words[i].substr(1)] = words[i + 1];
                ++i;
            }
        }
    }

    return make_pair(words[0], argsMap);
}

void help(const CommandArgs& args) {
    for (const auto& command : commands) {
        std::cout << command.first << ":\n";
        for (const auto& arg : command.second.argsInfos) {
            std::cout << "  -" << arg.first << ": " << arg.second << "\n";
        }
    }
}

void quit(const CommandArgs& args) {
    std::cout << "Goodbye!\n";
    exit(0);
}

void login(const CommandArgs& args) {
    std::cout << "login\n";
}

void logout(const CommandArgs& args) {
    std::cout << "logout\n";
}

void registerUser(const CommandArgs& args) {
    std::cout << "register\n";
}

void test(const CommandArgs& args) {
    std::cout << "test\n";
}
