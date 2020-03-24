#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

struct Capture {
    int t_first = 0;
    int t_last = 0;
    int counts = 0;
    [[nodiscard]] double calc_fps() const;
    void update(int msec);
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

class Episode {
public:
    void addDataFromFile(const std::string& filename);

    [[nodiscard]] std::vector<FpsRec> getFps() const;
    std::size_t getReadedCapturesCount() const { return captures_.size();}
private:
    std::unordered_map<std::string, Capture> captures_;
};

