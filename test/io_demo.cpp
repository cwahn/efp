#include "io.hpp"
#include <iostream>
#include <string>

using namespace efp;

int main()
{
    const char *filePath = "test.txt";
    const char *testData = "Hello, world!\nThis is a test file.\nEnd of test.";

    // Open a file for writing
    auto maybe_file = File::open(filePath, "w+");
    if (!maybe_file)
    {
        std::cerr << "Failed to open file for writing." << std::endl;
        return EXIT_FAILURE;
    }

    // Writing data to file
    File file = maybe_file.move(); // Assuming we have an unwrap method
    if (!file.write(testData))
    {
        std::cerr << "Failed to write data to file." << std::endl;
        return EXIT_FAILURE;
    }

    // Flush the stream and seek to the beginning of the file to read from the beginning
    // ! fflush(file.file_); // Assuming file_ is accessible, otherwise, you'd need a method to flush.
    if (!file.seek(0))
    {
        std::cerr << "Failed to seek to the beginning of the file." << std::endl;
        return EXIT_FAILURE;
    }

    // Reading data from file line by line
    // bool keep_read = true;
    // while (keep_read)
    // {
    //     file.read_line().match(
    //         [](const String &str)
    //         { std::cout << "Read line: " << str << std::endl; },
    //         [&]()
    //         { keep_read = false; });
    // }

    const auto lines = file.read_lines();
    std::cout << "File lines: " << lines << std::endl;

    // Check the current position in the file (should be at the end of file)
    long position = file.tell();
    std::cout << "Current file position: " << position << std::endl;

    // Close the file
    if (!file.close())
    {
        std::cerr << "Failed to close the file." << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
