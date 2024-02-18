#include <iostream>

#include "efp/cpp_core.hpp"
#include "efp/io.hpp"

using namespace efp;

int main() {
    const char* path = "test.txt";
    const char* test_data = "Hello, world!\nThis is a test file.\nEnd of test.";
    String test_data_string = test_data;

    auto maybe_file = File::open(path, "w+");
    if (!maybe_file)
        throw std::runtime_error("Failed to open file for writing.");

    File file = maybe_file.move();

    if (!file.write(test_data))
        throw std::runtime_error("Failed to write char* data to file.");

    file.flush();

    if (!file.seek(0))
        throw std::runtime_error("Failed to seek to the beginning of the file.");

    auto lines = file.read_lines();
    for (const auto& line : lines) {
        std::cout << "Read line: " << line << std::endl;
    }

    long position = file.tell();
    std::cout << "Current file position: " << position << std::endl;

    if (!file.write(test_data_string))
        throw std::runtime_error("Failed to write String data to file.");

    maybe_file = File::open(path, "r");
    if (!maybe_file)
        throw std::runtime_error("Failed to reopen file for reading.");

    file = maybe_file.move();

    lines = file.read_lines();
    std::cout << "File content after writing String data:" << std::endl;
    for (const auto& line : lines) {
        std::cout << line << std::endl;
    }

    if (!file.close())
        throw std::runtime_error("Failed to close the file.");

    return 0;
}
