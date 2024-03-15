#include "database.h"
#include <iostream>

Database::Database() {
    this->idGenerator = UniqueIDGenerator();
}

User *Database::createUser(const std::string& name, const std::string& password) {
    std::hash<std::string> constexpr hasher;
    std::string const hashedPassword = std::to_string(hasher(password));

    this->users.push_back({
        idGenerator.generateID(),
        name,
        {},
        {},
        hashedPassword
    });

    return &this->users.back();
}

void Database::listCars() {
    std::vector<Car> availableCars;

    // filter cars that are not rented
    std::copy_if(this->cars.begin(), this->cars.end(), std::back_inserter(availableCars), [](const Car& car) {
        return !car.rented;
    });

    if (availableCars.empty()) {
        std::cout << "No cars available" << std::endl;
    } else {
        // todo: print available cars
    }
}

std::optional<User*> Database::getUserByName(const std::string& name) {
    for (auto& user : this->users) {
        if (user.name == name) {
            return &user;
        }
    }

    return std::nullopt;
}

Database::~Database() {
    std::cout << "Database destroyed" << std::endl;
}


