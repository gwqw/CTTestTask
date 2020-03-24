#include "file_utils.h"

#include <algorithm>
#include <iostream>
#include <utility>

#include <boost/filesystem.hpp>

#include "string_utils.h"

using namespace std;
namespace fs = boost::filesystem;

void make_full_paths(std::vector<std::string>& files) {
    fs::path cur_path = fs::current_path();
    transform(files.begin(), files.end(), files.begin(), [cur_path](string& fname){
        return cur_path.string() + "/" + fname;
    });
}

void remove_non_valid_paths(std::vector<std::string>& files) {
    files.erase(
            remove_if(files.begin(), files.end(), [](const string& f){
                return !fs::exists(f);}),
            files.end()
    );
}

void sort_names_and_remove_duplic(std::vector<std::string>& files) {
    sort(files.begin(), files.end());
    files.erase(unique(files.begin(), files.end()), files.end());
}

std::vector<std::string> getFileListRecursive(const std::string& path_name) {
    fs::path parent_path;
    if (!path_name.empty()) {
        if (!fs::exists(path_name)) return {};
        parent_path = fs::path(path_name);
    } else {
        parent_path = fs::current_path();
    }
    vector<string> res;
    try {
        for (const auto& p : fs::recursive_directory_iterator(parent_path)) {
            if (fs::is_regular_file(p.path())) {
                res.push_back(p.path().string());
            }
        }
    } catch (const exception& e) {
        cerr << e.what() << " " << res.back() << endl;
        return res;
    }
    return res;
}

std::size_t get_file_size(const std::string& filename) {
    return fs::file_size(fs::path(filename));
}

std::size_t findEol(std::istream& f, std::size_t start_pos) {
    f.seekg(start_pos, ios_base::beg);
    char c;
    while (f.get(c)) {
        ++start_pos;
        if (c == '\n') break;
    }
    return start_pos;
}

bool cmp_files(const std::string& lhs_fname, const std::string& rhs_fname, std::size_t block_size) {
    ifstream lhs(lhs_fname, ios::binary);
    ifstream rhs(rhs_fname, ios::binary);
    if (!lhs || !rhs) return false;
    vector<char> lbuf(block_size), rbuf(block_size);
    while (true) {
        fill(lbuf.begin(), lbuf.end(), 0);
        fill(rbuf.begin(), rbuf.end(), 0);
        lhs.read(lbuf.data(), block_size);
        rhs.read(rbuf.data(), block_size);
        if (lbuf != rbuf) return false;
        if (!lhs && !rhs) return true;
        if (!lhs || !rhs) return false;
    }
}

bool isCorrectFileName(const std::string& filename, int start_ep, int end_ep) {
    // bui.004.001.info.yml.gz
    auto words = split_words(filename, '.');
    if (words.size() != 6 || words.back() != "gz") return false;
    try {
        int ep_num = stoi(words[2]);
        bool res = true;
        res = res && (start_ep < 0 || (start_ep >= 0 && ep_num >= start_ep));
        res = res && (end_ep < 0 || (end_ep >= 0 && ep_num <= end_ep));
        return res;
    } catch (const exception& e) {
        return false;
    }
}

std::vector<std::string> getFileList(const std::string& input_path, int start_ep, int end_ep) {
    fs::path parent_path(input_path);
    std::vector<std::string> res;
    for (const auto& p : fs::directory_iterator(parent_path)) {
        auto filename = p.path().filename().string();
        auto filepath = p.path().string();
        if (isCorrectFileName(filename, start_ep, end_ep))
        res.push_back(move(filepath));
    }
    sort(res.begin(), res.end());
    return res;
}

bool removeFile(const std::string& filename) {
    return fs::remove(filename);
}

void renameFile(const std::string& filename_from, const std::string& filename_to) {
    fs::rename(filename_from, filename_to);
}