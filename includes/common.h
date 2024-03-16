#ifndef UTILS_H
#define UTILS_H
#include "./database.h"
#include "./globals.h"
#include <string>
#include <functional>
#include <map>
#include <iostream>
#include <sstream>

using CommandArgs = std::map<std::string, std::string>;

typedef struct {
    std::function<void(const CommandArgs &)> execute;
    // defined if the used must be logged or not
    bool needAuth;
    // associate every argument with a description
    std::vector<std::pair<std::string, std::string> > argsInfos;
} Command;


std::pair<std::string, std::map<std::string, std::string> > parseInput(const std::string &input);

void dumpCarList(const std::vector<Car> &cars);

std::optional<long long> parseLong(const std::string &input);

std::string timestampToReadable(const std::chrono::system_clock::time_point &timestamp);

void help(const CommandArgs &args);

void quit(const CommandArgs &args);

void login(const CommandArgs &args);

void logout(const CommandArgs &args);

void registerUser(const CommandArgs &args);

void list(const CommandArgs &args);

void returnCar(const CommandArgs &args);

void addCar(const CommandArgs &args);

void removeCar(const CommandArgs &args);

void owned(const CommandArgs &args);

void rent(const CommandArgs &args);

// commands definition
const std::map<std::string, Command> commands = {
    {"help", {help, false, {}}},
    {"quit", {quit, false, {}}},
    {"login", {login, false, {{"u", "the username"}, {"p", "the password"}}}},
    {"logout", {logout, true, {}}},
    {"register", {registerUser, false, {{"u", "set the username"}, {"p", "set the password"}}}},
    {"list", {list, false, {}}},
    {"rent", {rent, true, {{"c", "the car id"}, {"d", "duration in days"}}}},
    {"return", {returnCar, true, {{"c", "the car id"}}}},
    {"add", {addCar, true, {{"d", "the car description"}}}},
    {"remove", {removeCar, true, {{"c", "the car id"}}}},
    {"owned", {owned, true, {}}}
};

#endif //UTILS_H
