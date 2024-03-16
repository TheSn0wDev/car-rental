#include "../includes/common.h"
#include <chrono>
#include <ctime>
#include <iomanip>

std::string timestampToReadable(const std::chrono::system_clock::time_point& timestamp) {
    // Convert time_point to time_t
    std::time_t time = std::chrono::system_clock::to_time_t(timestamp);

    // Convert time_t to tm struct
    std::tm* tm = std::localtime(&time);

    // Create a stringstream to format the output
    std::stringstream ss;
    ss << std::put_time(tm, "%Y-%m-%d %H:%M:%S"); // Format: YYYY-MM-DD HH:MM:SS

    return ss.str();
}

void dumpCarList(const std::vector<Car> &cars) {
    std::cout << "ID\tDescription\tRented\tUntil" << std::endl;

    for (const auto &[id, description, rented, until] : cars) {
        std::cout << id << "\t" << description << "\t";
        if (rented) {
            std::cout << "\033[1;31mYes\033[0m"; // Red color
        } else {
            std::cout << "\033[1;32mNo\033[0m"; // Green color
        }
        if (until.has_value()) {
            std::cout << "\t" << timestampToReadable(until.value()) << std::endl;
        } else {
            std::cout << "\t-" << std::endl;
        }
    }
}
