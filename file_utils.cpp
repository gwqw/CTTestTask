#include "file_utils.h"

#include <algorithm>

#include <opencv2/opencv.hpp>

#include "string_utils.h"

using namespace std;

bool isCorrectFileName(const std::string& filename, int start_ep, int end_ep) {
    auto words = split_words(filename, '.');
    if (words.size() != 6) return true;
    try {
        int ep_num = stoi(words[2]);
        bool res = true;
        res = res && (start_ep < 0 || (start_ep >= 0 && ep_num >= start_ep));
        res = res && (end_ep < 0 || (end_ep >= 0 && ep_num <= end_ep));
        return res;
    } catch (const exception& e) {
        return true;
    }
}

std::vector<std::string> getFileList(const std::string& input_path, int start_ep, int end_ep) {
    vector<cv::String> files;
    cv::glob(input_path + "*.info.yml.gz", files, false);
    files.erase(
            remove_if(files.begin(), files.end(), [start_ep, end_ep, size = input_path.size()](const cv::String& fname) {
                auto filename = string(fname).substr(size);
                return !isCorrectFileName(filename, start_ep, end_ep);
            }),
            files.end()
    );
    vector<string> res {make_move_iterator(files.begin()), make_move_iterator(files.end())};
    return res;
}
