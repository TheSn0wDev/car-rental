#include "../includes/common.h"
#include <functional>
#include <cstdlib>
#include <cerrno>
#include <cstring>
using namespace std;

pair<string, map<string, string>> parseInput(const string &input) {
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

void help(const CommandArgs &args) {
    for (const auto &[name, command]: commands) {
        std::cout << name << ":\n";
        for (const auto &[tag, description]: command.argsInfos) {
            std::cout << "  -" << tag << ": " << description << "\n";
        }
    }
}

void quit(const CommandArgs &args) {
    if (currentUser != nullptr) {
        std::cout << "Bye " << currentUser->name << "!\n";
    } else {
        std::cout << "Goodbye!\n";
    }
    exit(0);
}

void login(const CommandArgs &args) {
    if (auto const user = database.getUserByName(args.at("u")).value_or(nullptr)) {
        hash<string> constexpr hasher;

        if (string const hashedInputPassword = to_string(hasher(args.at("p")));
            user->password != hashedInputPassword) {
            cout << "Invalid username or password\n";
        } else {
            currentUser = user;
            cout << "Welcome " << user->name << "!\n";
        }
    } else {
        cout << "Invalid username or password\n";
    }
}

void logout(const CommandArgs &args) {
    cout << "Bye " << currentUser->name << "!\n";
    currentUser = nullptr;
}

void registerUser(const CommandArgs &args) {
    User *newUser = database.createUser(args.at("u"), args.at("p"));

    currentUser = newUser;
    cout << "User created, logged in as " << newUser->name << "\n";
}

void list(const CommandArgs &args) {
    if (const auto& cars = database.getCars(); cars.empty()) {
        cout << "No cars available at the moment" << endl;
    } else {
        dumpCarList(cars);
    }
}

void returnCar(const CommandArgs &args) {

}

void addCar(const CommandArgs &args) {
    database.createCar(args.at("d"));
    cout << "Car added\n";
}

void removeCar(const CommandArgs &args) {
    char *end;
    errno = 0;
    long long const id = strtoll(args.at("c").c_str(), &end, 10);

    if (end == args.at("c").c_str() || *end != '\0' || errno != 0) {
        cout << "Invalid car id\n";
        return;
    }

    if (Car const *car = database.getCarByID(id).value_or(nullptr); car == nullptr) {
        std::cout << "Car not found\n";
    } else {
        database.removeCarByID(id);
        std::cout << "Car removed\n";
    }
}

void rent(const CommandArgs &args) {

}

void owned(const CommandArgs &args) {
    auto cars = database.getCars();
    vector<Car> ownedCars;

    copy_if(cars.begin(), cars.end(), back_inserter(ownedCars), [](const Car &car) {
        return currentUser->id == car.ownerId;
    });

    dumpCarList(ownedCars);
}
