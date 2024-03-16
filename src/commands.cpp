#include "../includes/common.h"
#include <functional>
#include <cstdlib>
#include <cerrno>
#include <cstring>
using namespace std;

pair<string, map<string, string> > parseInput(const string &input) {
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
        cout << name << ":" << endl;
        for (const auto &[tag, description]: command.argsInfos) {
            cout << "  -" << tag << ": " << description << endl;
        }
    }
}

void quit(const CommandArgs &args) {
    if (currentUser != nullptr) {
        cout << "Bye " << currentUser->name << "!" << endl;
    } else {
        cout << "Goodbye!" << endl;
    }
    exit(0);
}

void login(const CommandArgs &args) {
    if (auto const user = database.getUserByName(args.at("u")).value_or(nullptr)) {
        hash<string> constexpr hasher;

        if (string const hashedInputPassword = to_string(hasher(args.at("p")));
            user->password != hashedInputPassword) {
            cout << "Invalid username or password" << endl;
        } else {
            currentUser = user;
            cout << "Welcome " << user->name << "!" << endl;
        }
    } else {
        cout << "Invalid username or password" << endl;
    }
}

void logout(const CommandArgs &args) {
    cout << "Bye " << currentUser->name << "!" << endl;
    currentUser = nullptr;
}

void registerUser(const CommandArgs &args) {
    User *newUser = database.createUser(args.at("u"), args.at("p"));

    currentUser = newUser;
    cout << "User created, logged in as " << newUser->name << "\n";
}

void list(const CommandArgs &args) {
    if (const auto &cars = database.getCars(); cars.empty()) {
        cout << "No cars available at the moment" << endl;
    } else {
        dumpCarList(cars);
    }
}

void returnCar(const CommandArgs &args) {
    auto const id = parseLong(args.at("c"));

    if (!id.has_value()) {
        cout << "Invalid car id" << endl;
        return;
    }

    if (Car *car = database.getCarByID(id.value()).value_or(nullptr); car == nullptr) {
        cout << "Car not found" << endl;
    } else {
        if (car->renterId != currentUser->id) {
            cout << "You're not the renter of this car" << endl;
        } else {
            car->rented = false;
            car->renterId = nullopt;
            car->until = nullopt;
            cout << "Car returned" << endl;
        }
    }
}

void addCar(const CommandArgs &args) {
    database.createCar(args.at("d"));
    cout << "Car added" << endl;
}

void removeCar(const CommandArgs &args) {
    auto const id = parseLong(args.at("c"));

    if (!id.has_value()) {
        cout << "Invalid car id" << endl;
        return;
    }

    if (Car const *car = database.getCarByID(id.value()).value_or(nullptr); car == nullptr) {
        cout << "Car not found" << endl;
    } else {
        if (car->ownerId != currentUser->id) {
            cout << "This car doesn't belong to you" << endl;
        } else {
            database.removeCarByID(id.value());
            cout << "Car removed" << endl;
        }
    }
}

void rent(const CommandArgs &args) {
    auto const id = parseLong(args.at("c"));
    auto const duration = parseLong(args.at("d"));

    if (!id.has_value()) {
        cout << "Invalid car id" << endl;
        return;
    }
    if (!duration.has_value() || duration.value() <= 0) {
        cout << "Invalid duration" << endl;
        return;
    }
    if (Car *car = database.getCarByID(id.value()).value_or(nullptr); car == nullptr) {
        cout << "Car not found" << endl;
    } else {
        if (car->renterId != nullopt) {
            cout << "Car already rented" << endl;
        } else {
            chrono::system_clock::time_point const now = chrono::system_clock::now();
            chrono::duration<int, ratio<60 * 60 * 24> > const numberOfDays(duration.value());

            car->rented = true;
            car->renterId = currentUser->id;
            car->until = now + numberOfDays;
            cout << "Car rented until " << timestampToReadable(car->until.value()) << endl;
        }
    }
}

void owned(const CommandArgs &args) {
    auto cars = database.getCars();
    vector<Car> ownedCars;

    copy_if(cars.begin(), cars.end(), back_inserter(ownedCars), [](const Car &car) {
        return currentUser->id == car.ownerId;
    });

    dumpCarList(ownedCars);
}
