#ifndef UTILS_H
#define UTILS_H
#include "./database.h"
#include <string>
#include <functional>
#include <map>
#include <iostream>
#include <sstream>

using CommandArgs = std::map<std::string, std::string>;
typedef struct {
    std::function<void(const CommandArgs&)> execute;
    // defined if the used must be logged or not
    bool needAuth;
    // associate every argument with a description
    std::vector<std::pair<std::string, std::string>> argsInfos;
} Command;

// global database instance
extern Database database;
extern User* currentUser;

// functions
std::pair<std::string, std::map<std::string, std::string>> parseInput(const std::string& input);
void help(const CommandArgs& args);
void quit(const CommandArgs& args);
void login(const CommandArgs& args);
void logout(const CommandArgs& args);
void registerUser(const CommandArgs& args);

// commands definition
const std::map<std::string, Command> commands = {
    {"help", {help, false, {}}},
    {"quit", {quit, false, {}}},
    {"login", {login, false, {{"u", "the username"}, {"p", "the password"}}}},
    {"logout", {logout, true, {}}},
    {"register", {registerUser, false, {{"u", "set the username"}, {"p", "set the password"}}}},
};

#endif //UTILS_H
