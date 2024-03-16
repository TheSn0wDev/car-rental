#include "database.h"
#include <iostream>
#include <functional>
#include <fstream>
#include "cereal/archives/binary.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/chrono.hpp"
#include "cereal/types/optional.hpp"
#include "globals.h"

const std::string Database::FILENAME = "database.bin";

Database::Database() {
    if (std::ifstream is(FILENAME, std::ios::binary); is) {
        cereal::BinaryInputArchive archive(is);
        archive(this->users, this->cars);
    } else {
        std::cout << "No data found, fresh database" << std::endl;
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

Car *Database::createCar(const std::string &description) {
    this->cars.push_back({
        idGenerator.generateID(),
        currentUser->id,
        description,
        false,
        std::nullopt
    });

    return &this->cars.back();
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

std::optional<Car *> Database::getCarByID(long long id) {
    auto it = std::find_if(this->cars.begin(), this->cars.end(), [id](const Car &car) {
        return car.id == id;
    });

    if (it != this->cars.end()) {
        return &(*it);
    }

    return std::nullopt;
}

bool Database::removeCarByID(long long id) {
    auto it = std::find_if(this->cars.begin(), this->cars.end(), [id](const Car &car) {
        return car.id == id;
    });

    if (it != this->cars.end()) {
        this->cars.erase(it);
        return true;
    }

    return false;
}

Database::~Database() {
    save();
    std::cout << "Save car rental state" << std::endl;
}
