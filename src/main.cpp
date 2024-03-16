#include "../includes/common.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
using namespace std;

auto database = Database();
User *currentUser = nullptr;

void signalHandler(int signum) {
    quit({});
}

bool canExecute(const string &command, map<string, string> args) {
    if (commands.find(command) == commands.end()) {
        cout << "Command not found" << endl;
        return false;
    }
    if (commands.at(command).needAuth && currentUser == nullptr) {
        cout << "You must be logged in to execute this command" << endl;
        return false;
    }

    bool hasError = false;
    auto argsInfos = commands.at(command).argsInfos;

    for (const auto &[arg, description]: argsInfos) {
        if (args.find(arg) == args.end()) {
            if (!hasError) {
                cout << "Missing arguments:" << endl;
                hasError = true;
            }
            cout << arg << ": " << description << endl;
        }
    }

    return !hasError;
}

int main() {
    cout << "Welcome to car rental !" << endl;
    // init sig handler
    signal(SIGINT, signalHandler);

    while (true) {
        string input;
        cout << "$> ";
        getline(cin, input);

        if (auto [commandName, args] = parseInput(input); canExecute(commandName, args)) {
            commands.at(commandName).execute(args);
        }
    }
}
