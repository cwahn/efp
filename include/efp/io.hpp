#ifndef IO_HPP_
#define IO_HPP_

#include "efp/cpp_core.hpp"
#include "efp/prelude.hpp"
#include "efp/string.hpp"

namespace efp {
class File {
  public:
    static Maybe<File> open(const char* path, const char* mode) {
        FILE* file = fopen(path, mode);

        if (file)
            return File {file, mode};

        return nothing;
    }

    static Maybe<File> open(const String& path, const char* mode) {
        FILE* file = fopen(path.c_str(), mode);

        if (file)
            return File {file, mode};

        return nothing;
    }

    ~File() {
        if (_file)
            fclose(_file);
    }

    File(const File&) = delete;

    File& operator=(const File&) = delete;

    File(File&& other) noexcept : _file(other._file) {
        other._file = nullptr;
    }

    File& operator=(File&& other) noexcept {
        if (this != &other) {
            _file = other._file;
            other._file = nullptr;
        }
        return *this;
    }

    Maybe<String> read_line() {
        if (_file) {
            String buffer {};
            int ch;

            while ((ch = fgetc(_file)) != '\n' && ch != EOF) {
                buffer.push_back(static_cast<char>(ch));
            }

            // Check for EOF condition
            if (ch == EOF && buffer.empty())
                return nothing;

            return buffer;
        }

        return nothing;
    }

    Vector<String> read_lines() {
        Vector<String> lines {};

        bool keep_read = true;

        while (keep_read) {
            const auto maybe_line = read_line();

            if (maybe_line)
                lines.push_back(maybe_line.value());
            else
                keep_read = false;
        }

        return lines;
    }

    bool write(const char* data, int length = -1) {
        if (_file == nullptr)
            return false;

        if (strchr(_mode, 'b')) {
            // Write binary data
            if (length == -1)
                length = strlen(data);

            int written = fwrite(data, sizeof(char), length, _file);

            return written == length;
        } else {
            // Write text data
            if (length == -1)
                length = strlen(data);

            for (size_t i = 0; i < length; ++i) {
                if (fputc(data[i], _file) == EOF)
                    return false;
            }

            return true;
        }
    }

    bool write(const String& data) {
        if (_file == nullptr)
            return false;

        if (strchr(_mode, 'b')) {
            // Write binary data
            int written = fwrite(data.data(), sizeof(char), data.size(), _file);
            return written == data.size();
        } else {
            return fputs(data.c_str(), _file) != EOF;
        }
    }

    int flush() {
        return fflush(_file);
    }

    bool seek(long offset) {
        return fseek(_file, offset, SEEK_SET) == 0;
    }

    int tell() {
        return ftell(_file);
    }

    bool close() {
        if (_file && fclose(_file) != EOF) {
            _file = nullptr;
            return true;
        }

        return false;
    }

  private:
    explicit File(FILE* file, const char* mode) : _file(file) {
        strncpy(_mode, mode, sizeof(_mode) - 1);
        _mode[sizeof(_mode) - 1] = '\0';
    }

    FILE* _file;
    char _mode[8];
};

};  // namespace efp

#endif