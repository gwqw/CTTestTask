#pragma once

#include <string>

bool decompress(const std::string& filename_in, const std::string& filename_out);

class Decompressor {
public:
    explicit Decompressor(const std::string& filename);
    ~Decompressor();
    [[nodiscard]] bool isDecompressed() const {return decompress_result_;}
    [[nodiscard]] const std::string& getFileName() const {return yaml_filename_;}
private:
    std::string yaml_filename_;
    bool decompress_result_ = false;
};