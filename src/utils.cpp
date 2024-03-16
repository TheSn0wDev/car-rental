#include "../includes/common.h"
#include <chrono>
#include <ctime>
#include <iomanip>

std::string timestampToReadable(const std::chrono::system_clock::time_point &timestamp) {
    std::time_t const time = std::chrono::system_clock::to_time_t(timestamp);
    std::tm const *tm = std::localtime(&time);
    std::stringstream ss;

    ss << std::put_time(tm, "%Y-%m-%d %H:%M:%S"); // Format: YYYY-MM-DD HH:MM:SS

    return ss.str();
}

void dumpCarList(const std::vector<Car> &cars) {
    std::cout << "ID\t\tDescription\tRented\tUntil" << std::endl;

    for (const auto &[id, _, description, rented, until]: cars) {
        std::cout << id << "\t" << description << "\t";
        if (rented) {
            std::cout << "\033[1;32mYes\033[0m"; // Green color
        } else {
            std::cout << "\033[1;31mNo\033[0m"; // Red color
        }
        if (until.has_value()) {
            std::cout << "\t" << timestampToReadable(until.value()) << std::endl;
        } else {
            std::cout << "\t-" << std::endl;
        }
    }
}
