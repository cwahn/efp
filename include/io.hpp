#ifndef IO_HPP_
#define IO_HPP_

#include <stdio.h>
#include <stdlib.h>

#include "prelude.hpp"
#include "string.hpp"

namespace efp
{
    class File
    {
    public:
        static Maybe<File> open(const char *path, const char *mode)
        {
            FILE *file = fopen(path, mode);
            if (file)
            {
                return File{efp::move(file)};
            }
            return Nothing{};
        }

        // todo Support std::string path

        static Maybe<File> open(const String &path, const char *mode)
        {
            FILE *file = fopen(path.c_str(), mode);
            if (file)
            {
                return File{efp::move(file)};
            }
            return Nothing{};
        }

        ~File()
        {
            if (file_)
            {
                fclose(file_);
            }
        }

        // Disable copy and assignment.
        File(const File &) = delete;
        File &operator=(const File &) = delete;

        // Enable move semantics.
        File(File &&other) noexcept
            : file_(other.file_)
        {
            other.file_ = nullptr;
        }

        File &operator=(File &&other) noexcept
        {
            if (this != &other)
            {
                if (file_)
                {
                    fclose(file_);
                }
                file_ = other.file_;
                other.file_ = nullptr;
            }
            return *this;
        }

        // Reads a single line from the file
        Maybe<String> read_line()
        {
            String buffer{};
            int ch;

            while ((ch = fgetc(file_)) != '\n' && ch != EOF)
            {
                buffer.push_back(static_cast<char>(ch));
            }

            // Check for EOF condition
            if (ch == EOF && buffer.is_empty())
            {
                return Nothing{};
            }

            return buffer;
        }

        // Reads all lines from the file into a Vector
        Vector<String> read_lines()
        {
            Vector<String> lines{};

            bool keep_read = true;
            while (keep_read)
            {
                const auto maybe_line = read_line();

                if (maybe_line)
                    lines.push_back(maybe_line.value());
                else
                    keep_read = false;
            }

            return lines;
        }

        // Writes a string to the file
        bool write(const char *data)
        {
            if (fputs(data, file_) != EOF)
            {
                return true;
            }
            return false;
        }

        bool write(const String &data)
        {
            if (fputs(data.c_str(), file_) != EOF)
            {
                return true;
            }
            return false;
        }

        // Seeks to a position in the file
        bool seek(long offset)
        {
            return fseek(file_, offset, SEEK_SET) == 0;
        }

        // Returns the current position in the file
        int tell()
        {
            return ftell(file_);
        }

        // Closes the file
        bool close()
        {
            if (fclose(file_) != EOF)
            {
                file_ = nullptr;
                return true;
            }
            return false;
        }

    private:
        explicit File(FILE *file) : file_(file) {}

        FILE *file_;
    };

};

#endif