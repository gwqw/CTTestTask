#pragma once

#include <string>

class Decompressor {
public:
    explicit Decompressor(const std::string& filename);
    ~Decompressor();
    [[nodiscard]] bool isDecompressed() const {return decompress_result_;}
    [[nodiscard]] const std::string& getFileName() const {return yaml_filename_;}
    static bool decompress(const std::string& filename_in, const std::string& filename_out);
private:
    std::string yaml_filename_;
    bool decompress_result_ = false;
};