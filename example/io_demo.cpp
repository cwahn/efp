#include "efp/io.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace efp;

int main() {
    const char* path = "test.txt";
    const char* test_data = "Hello, world!\nThis is a test file.\nEnd of test.";
    String test_data_string = test_data;

    auto maybe_file = File::open(path, "w+");
    if (!maybe_file) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return 1;
    }

    File file = maybe_file.move();

    if (!file.write(test_data)) {
        std::cerr << "Failed to write char* data to file." << std::endl;
        return 1;
    }

    file.flush();

    if (!file.seek(0)) {
        std::cerr << "Failed to seek to the beginning of the file." << std::endl;
        return 1;
    }

    auto lines = file.read_lines();
    for (const auto& line : lines) {
        std::cout << "Read line: " << line << std::endl;
    }

    long position = file.tell();
    std::cout << "Current file position: " << position << std::endl;

    if (!file.write(test_data_string)) {
        std::cerr << "Failed to write String data to file." << std::endl;
        return 1;
    }

    maybe_file = File::open(path, "r");
    if (!maybe_file) {
        std::cerr << "Failed to reopen file for reading." << std::endl;
        return 1;
    }

    file = maybe_file.move();

    lines = file.read_lines();
    std::cout << "File content after writing String data:" << std::endl;
    for (const auto& line : lines) {
        std::cout << line << std::endl;
    }

    if (!file.close()) {
        std::cerr << "Failed to close the file." << std::endl;
        return 1;
    }

    // file = File::open("test", "w+").move();
    // file.write("Hello, world!\nThis is a test file.\nEnd of test.");
    // file.close();

    return 0;
}
