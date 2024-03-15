#include "../includes/common.h"
#include <iostream>
#include <map>
#include <vector>
#include <csignal>

auto database = Database();
User *currentUser = nullptr;

void signalHandler(int signum) {
    quit({});
}

bool canExecute(const std::string& command, std::map<std::string, std::string> args) {
    if (commands.find(command) == commands.end()) {
        std::cout << "Command not found" << std::endl;
        return false;
    }
    if (commands.at(command).needAuth && currentUser == nullptr) {
        std::cout << "You must be logged in to execute this command" << std::endl;
        return false;
    }

    bool hasError = false;
    auto argsInfos = commands.at(command).argsInfos;

    for (const auto& [arg, description] : argsInfos) {
        if (args.find(arg) == args.end()) {
            if (!hasError) {
                std::cout << "Missing arguments:" << std::endl;
                hasError = true;
            }
            std::cout << arg << ": " << description << std::endl;
        }
    }

    return !hasError;
}

int main(int argc, char *argv[]) {
    signal(SIGINT, signalHandler);

    while (true) {
        std::string input;
        std::cout << "$> ";
        std::getline(std::cin, input);

        auto [commandName, args] = parseInput(input);

        if (canExecute(commandName, args)) {
            commands.at(commandName).execute(args);
        }
    }
}
