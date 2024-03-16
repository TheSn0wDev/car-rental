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
    long long ownerId;
    std::optional<long long> renterId;
    std::string description;
    bool rented;
    std::optional<std::chrono::time_point<std::chrono::system_clock> > until;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(id, ownerId, renterId, description, rented, until);
    }
} Car;

typedef struct User_t {
    long long id;
    std::string name;
    // we reference cars by their id
    std::vector<long long> rentedCars;
    std::string password;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(id, name, rentedCars, password);
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

    Car *createCar(const std::string &description);

    bool removeCarByID(long long id);

    void listCars();

    void save();

    void refresh();

    [[nodiscard]] std::optional<Car *> getCarByID(long long id);

    [[nodiscard]] std::optional<User *> getUserByName(const std::string &name);

    [[nodiscard]] const std::vector<Car> &getCars() const;

    ~Database();
};

#endif //DATABASE_H
