#include <iostream>

#include "efp/cpp_core.hpp"
#include "efp/io.hpp"

using namespace efp;

int main() {
    const char* path = "test.txt";
    const char* c_str_data = "Hello, world!\nThis is a test file.\nEnd of test.";
    String string_data = "Hello, world!\nThis is a test file.\nEnd of test.";

    auto maybe_file = File::open(path, "w+");
    if (!maybe_file)
        throw std::runtime_error("Failed to open file for writing.");

    {
        File file = maybe_file.move();

        if (!file.write(c_str_data))
            throw std::runtime_error("Failed to write char* data to file.");

        file.flush();

        if (!file.seek(0))
            throw std::runtime_error("Failed to seek to the beginning of the file.");

        auto lines = file.read_lines();

        println("File content after writing c_str data:");
        for_each([](const String& line) { println("{}", line); }, lines);

        long position = file.tell();
        println("Current file position: {}\n", position);

        if (!file.write(string_data))
            throw std::runtime_error("Failed to write String data to file.");
    }

    auto maybe_file_to_read = File::open(path, "r");
    if (!maybe_file_to_read)
        throw std::runtime_error("Failed to reopen file for reading.");

    File file_to_read = maybe_file_to_read.move();

    const auto read_lines = file_to_read.read_lines();

    println("File content after writing String data:");
    for_each([](const String& line) { println("{}", line); }, read_lines);

    if (!file_to_read.close())
        throw std::runtime_error("Failed to close the file.");

    return 0;
}
