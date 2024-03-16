#include "../includes/common.h"
#include <chrono>
#include <ctime>
#include <iomanip>
using namespace std;

string timestampToReadable(const chrono::system_clock::time_point &timestamp) {
    time_t const time = chrono::system_clock::to_time_t(timestamp);
    tm const *tm = localtime(&time);
    stringstream ss;

    ss << put_time(tm, "%Y-%m-%d %H:%M:%S"); // Format: YYYY-MM-DD HH:MM:SS

    return ss.str();
}

void dumpCarList(const vector<Car> &cars) {
    cout << "ID\t\tDescription\tRented\tUntil\tRenter" << endl;

    for (const auto &car: cars) {
        cout << car.id << "\t" << car.description << "\t";
        if (car.rented) {
            cout << "\033[1;32mYes\033[0m"; // Green color
        } else {
            cout << "\033[1;31mNo\033[0m"; // Red color
        }
        if (car.until.has_value()) {
            cout << "\t" << timestampToReadable(car.until.value()) << "\t" << car.renterId.value() << endl;
        } else {
            cout << "\t-\t-" << endl;
        }
    }
}

std::optional<long long> parseLong(const string &input) {
    char *end;
    errno = 0;
    long long const id = strtoll(input.c_str(), &end, 10);

    if (end == input.c_str() || *end != '\0' || errno != 0) {
        return nullopt;
    }
    return id;
}
