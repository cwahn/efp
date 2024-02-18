#include <iostream>
#include "efp/format.hpp"

using namespace efp;

int main() {
    // Basic string formatting
    println("Hello, {}!", "world");

    // Formatting integers
    println("Integer: {}", 42);
    // Formatting floating-point numbers with precision
    println("Floating-point: {:.2f}", 3.14159265);

    // Using format to create a formatted string without immediate printing
    const String name_0 = "Lighthouse";
    const String name_1 = "Tinker Bell";
    println("{}", format("{} loves {}.", name_0, name_1));

    // Formatting Unicode characters
    println("Korean: {}", "안녕하세요!");

    // Formatting with padding and alignment
    // println("Right-aligned, padded with spaces: {:>10}", "right");
    // println("Left-aligned, padded with zeros: {:0<10}", "left");
    // println("Center-aligned, padded with asterisks: {:*^15}", "center");

    return 0;
}