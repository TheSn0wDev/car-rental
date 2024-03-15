#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <chrono>
#include <optional>
#include <functional>

class UniqueIDGenerator {
private:
    long long currentID;

public:
    UniqueIDGenerator() : currentID(0) {}

    long long generateID() {
        auto const now = std::chrono::system_clock::now();
        auto const duration = now.time_since_epoch();
        auto const millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        return millis + currentID++;
    }
};

typedef struct {
    long long id;
    std::string description;
    bool rented;
} Car;

typedef struct {
    long long id;
    std::string name;
    // we reference cards by their id
    std::vector<long long> rentedCars;
    std::vector<long long> borrowedCars;
    // should be encrypted
    std::string password;
} User;

class Database {
private:
    std::vector<User> users;
    std::vector<Car> cars;
    UniqueIDGenerator idGenerator;

public:
    Database();
    User *createUser(const std::string& name, const std::string& password);
    std::optional<User*> getUserByName(const std::string& name);
    void listCars();
    ~Database();
};

#endif //DATABASE_H