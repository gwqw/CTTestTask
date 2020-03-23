#pragma once

#include <vector>
#include <string>
#include <fstream>

void make_full_paths(std::vector<std::string>& files);
void remove_non_valid_paths(std::vector<std::string>& files);
void sort_names_and_remove_duplic(std::vector<std::string>& files);
std::vector<std::string> getFileListRecursive(const std::string& path_name);
std::size_t get_file_size(const std::string& filename);
std::size_t findEol(std::istream& f, std::size_t start_pos);
bool cmp_files(const std::string& lhs_fname, const std::string& rhs_fname,
        std::size_t block_size = 1024);

// utils for episode files
std::vector<std::string> getFileList(const std::string& input_path, int start_ep, int end_ep);
bool isCorrectFileName(const std::string& filename, int start_ep, int end_ep);
