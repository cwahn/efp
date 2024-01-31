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
            return File{file, mode};

        return nothing;
    }

    static Maybe<File> open(const String& path, const char* mode) {
        FILE* file = fopen(path.c_str(), mode);

        if (file)
            return File{file, mode};

        return nothing;
    }

    ~File() {
        if (file_)
            fclose(file_);
    }

    File(const File&) = delete;

    File& operator=(const File&) = delete;

    File(File&& other) noexcept
        : file_(other.file_) {
        other.file_ = nullptr;
    }

    File& operator=(File&& other) noexcept {
        if (this != &other) {
            file_ = other.file_;
            other.file_ = nullptr;
        }
        return *this;
    }

    Maybe<String> read_line() {
        if (file_) {
            String buffer{};
            int ch;

            while ((ch = fgetc(file_)) != '\n' && ch != EOF) {
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
        Vector<String> lines{};

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
        if (file_ == nullptr)
            return false;

        if (strchr(mode_, 'b')) {
            // Write binary data
            if (length == -1)
                length = strlen(data);

            int written = fwrite(data, sizeof(char), length, file_);

            return written == length;
        } else {
            // Write text data
            if (length == -1)
                length = strlen(data);

            for (size_t i = 0; i < length; ++i) {
                if (fputc(data[i], file_) == EOF)
                    return false;
            }

            return true;
        }
    }

    bool write(const String& data) {
        if (file_ == nullptr)
            return false;

        if (strchr(mode_, 'b')) {
            // Write binary data
            int written = fwrite(data.data(), sizeof(char), data.size(), file_);
            return written == data.size();
        } else {
            return fputs(data.c_str(), file_) != EOF;
        }
    }

    int flush() {
        return fflush(file_);
    }

    bool seek(long offset) {
        return fseek(file_, offset, SEEK_SET) == 0;
    }

    int tell() {
        return ftell(file_);
    }

    bool close() {
        if (file_ && fclose(file_) != EOF) {
            file_ = nullptr;
            return true;
        }

        return false;
    }

private:
    explicit File(FILE* file, const char* mode)
        : file_(file) {
        strncpy(mode_, mode, sizeof(mode_) - 1);
        mode_[sizeof(mode_) - 1] = '\0';
    }

    FILE* file_;
    char mode_[8];
};

}; // namespace efp

#endif