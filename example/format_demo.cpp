#include <iostream>
#include "efp/format.hpp"

using namespace efp;

int main() {
    // Basic string formatting
    println("Hello, {}!", "world");

    // Formatting integers
    println("Integer: {}", 42);

    // Formatting Unicode characters
    println("한국어: {}", "안녕하세요");

    // Formatting floating-point numbers with precision
    println("Floating-point: {:.2f}", 3.14159265);

    // Formatting with padding and alignment
    println("Right-aligned, padded with spaces: {:>10}", "right");
    println("Left-aligned, padded with zeros: {:0<10}", "left");
    println("Center-aligned, padded with asterisks: {:*^15}", "center");

    // Formatting date and time (requires C++20 and fmt version 8 or later)
    // std::time_t t = std::time(nullptr);
    // println("Current Date and Time: {:%Y-%m-%d %H:%M:%S}", *std::localtime(&t));

    // Formatting containers
    // std::vector<int> vec = {1, 2, 3};
    // println("Vector: {}", fmt::join(vec, ", "));

    // Formatting maps
    // std::map<std::string, int> map = {{"one", 1}, {"two", 2}};
    // for (const auto& [key, value] : map) {
    //     println("Map entry: {} = {}", key, value);
    // }

    // Conditional formatting
    int score = 75;
    println("Your score is {} and you have {}.", score, score > 60 ? "passed" : "failed");

    // Formatting user-defined types
    struct Point {
        double x, y;
    };

    Point p = {1.2345, 9.876};
    println("Point: ({:.1f}, {:.1f})", p.x, p.y);

    // Using format to create a formatted string without immediate printing
    const auto formatted_str = fmt::format("This is a formatted number: {}", 42);
    println("{}", formatted_str);

    return 0;
}