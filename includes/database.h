#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <chrono>
#include <optional>

class UniqueIDGenerator {
    long long currentID;

public:
    UniqueIDGenerator() : currentID(0) {
    }

    long long generateID() {
        auto const now = std::chrono::system_clock::now();
        auto const duration = now.time_since_epoch();
        auto const millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        return millis + currentID++;
    }
};

typedef struct Car_t {
    long long id;
    std::string description;
    bool rented;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(id, description, rented);
    }
} Car;

typedef struct User_t {
    long long id;
    std::string name;
    // we reference cars by their id
    std::vector<long long> rentedCars;
    std::vector<long long> borrowedCars;
    std::string password;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(id, name, rentedCars, borrowedCars, password);
    }
} User;

class Database {
    std::vector<User> users;
    std::vector<Car> cars;
    UniqueIDGenerator idGenerator;

    static const std::string FILENAME;

public:
    Database();

    User *createUser(const std::string &name, const std::string &password);

    std::optional<User *> getUserByName(const std::string &name);

    void listCars();

    void save() const;

    ~Database();
};

#endif //DATABASE_H
