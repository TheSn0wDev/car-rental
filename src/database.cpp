#include "database.h"
#include <iostream>
#include <functional>
#include <fstream>
#include "cereal/archives/binary.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/chrono.hpp"
#include "cereal/types/optional.hpp"

const std::string Database::FILENAME = "database.bin";

Database::Database() {
    if (std::ifstream is(FILENAME, std::ios::binary); is) {
        cereal::BinaryInputArchive archive(is);
        archive(this->users, this->cars);
    } else {
        throw std::runtime_error("File not found: " + FILENAME);
    }
}

User *Database::createUser(const std::string &name, const std::string &password) {
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
    std::copy_if(this->cars.begin(), this->cars.end(), std::back_inserter(availableCars), [](const Car &car) {
        return !car.rented;
    });

    if (availableCars.empty()) {
        std::cout << "No cars available" << std::endl;
    } else {
        // todo: print available cars
    }
}

std::optional<User *> Database::getUserByName(const std::string &name) {
    for (auto &user: this->users) {
        if (user.name == name) {
            return &user;
        }
    }

    return std::nullopt;
}

void Database::save() const {
    std::ofstream os(FILENAME, std::ios::binary);

    if (!os) {
        throw std::runtime_error("Failed to open file: " + FILENAME);
    }

    cereal::BinaryOutputArchive oarchive(os);
    oarchive(this->users, this->cars);
}

const std::vector<Car> &Database::getCars() const {
    return this->cars;
}

Database::~Database() {
    save();
    std::cout << "Save car rental state" << std::endl;
}
