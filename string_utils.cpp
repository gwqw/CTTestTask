#include "string_utils.h"

#include <algorithm>

using namespace std;

std::vector<std::string> split_words(const std::string& str, char sep) {
    vector<string> result;
    auto str_begin = begin(str);
    const auto str_end = end(str);

    while (true) {
        auto it = find(str_begin, str_end, sep);
        result.emplace_back(str_begin, it);

        if (it == str_end) {
            break;
        } else {
            str_begin = next(it);
            while (str_begin != str_end && *str_begin == ' ') {
                ++str_begin;
            }
            if (str_begin == str_end) {
                break;
            }
        }
    }
    return result;
}