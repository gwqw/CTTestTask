#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

struct Capture {
    uint64_t t_first = 0;
    uint64_t t_last = 0;
    uint64_t counts = 0;
    [[nodiscard]] double calc_fps() const;
    void update(uint64_t msec);
};

struct FpsRec {
    std::string capture_name;
    double fps = 0;
    FpsRec(const std::string& capture_name, double fps)
        : capture_name(capture_name), fps(fps) {}
};
std::ostream& operator<<(std::ostream& out, const FpsRec& fps_rec);

void saveToCSV(const std::vector<FpsRec>& fpsRec, const std::string& filename);
void saveToCSV(const std::vector<FpsRec>& fpsRec, std::ostream& out);

namespace YamlPreprocessor{
    void yamlPreprocessor(const std::string& filename);
    std::string processLine(std::string& line);
};

class Episode {
public:
    void addDataFromFile(const std::string& filename, bool needToProcess = true);

    std::vector<FpsRec> getFps() const;
    double getFpsForName(const std::string& capture_name) const;
private:
    std::unordered_map<std::string, Capture> captures_;
};

