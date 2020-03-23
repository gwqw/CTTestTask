#include "simple_decompress.h"

#include <fstream>
#include <cassert>
#include <cstdio>

#include <zlib.h>

using namespace std;

bool decompress(const std::string& filename_in, const std::string& filename_out) {
    gzFile f = gzopen(filename_in.c_str(), "rb");
    if (!f) return false;
    int res = 0;
    constexpr int SIZE = 1024;
    string s(SIZE, '\0');
    ofstream out(filename_out);
    while (true) {
        res = gzread(f, s.data(), SIZE);
        if (res <= 0) break;
        if (res == SIZE) {
            out << s;
        } else {
            assert(res < SIZE);
            s[res] = '\0';
            out << s.c_str();
        }
    }
    gzclose(f);
    return (res == 0);
}

Decompressor::Decompressor(const std::string& filename) {
    if (filename.size() < 3) {
        throw invalid_argument("wrong filename " + filename);
    }
    yaml_filename_ = string(filename.begin(), filename.begin() + filename.size() - 3);
    decompress_result_ = decompress(filename, yaml_filename_);
}

Decompressor::~Decompressor() {
    if (decompress_result_) {
        remove(yaml_filename_.c_str());
    }
}