#include "framedata_reader.h"

#include <cassert>
#include <fstream>
#include <algorithm>

#include <opencv2/opencv.hpp>

using namespace std;

void Capture::update(int msec) {
    if (t_first == 0) {
        t_first = msec;
    }
    assert(t_last <= msec);
    t_last = msec;
    ++counts;
}

double Capture::calc_fps() const {
    return (counts > 0 && t_last != t_first) ?
           static_cast<double>(counts-1) * 1000.0 / static_cast<double>(t_last - t_first) :
           0;
}


std::ostream& operator<<(std::ostream& out, const FpsRec& fps_rec) {
    out << fps_rec.capture_name << ',' << fps_rec.fps;
    return out;
}

void saveToCSV(const std::vector<FpsRec>& fpsRec, const std::string& filename) {
    if (filename.empty()) {
        saveToCSV(fpsRec, cout);
    } else {
        ofstream out(filename);
        saveToCSV(fpsRec, out);
    }
}

void saveToCSV(const std::vector<FpsRec>& fpsRec, std::ostream& out) {
    for (const auto& r : fpsRec) {
        out << r << '\n';
    }
}


void Episode::addDataFromFile(const std::string &filename) {
    // preprocess file, because it is not valid yaml-file
    try {
        cv::FileStorage yaml;
        bool res = yaml.open(filename, cv::FileStorage::READ);

        if (res) {
            // treat header
            auto header = yaml["header"];
            auto captures = header["captures"];
            for (const auto &capture : captures) {
                string name = static_cast<string>(capture["name"]);
                if (captures_.count(name) == 0) {
                    captures_[name] = Capture{};
                }
            }

            // treat captures body (shots)
            auto shots = yaml["shots"];
            for (const auto &shot : shots) {
                for (const auto &capture : shot) {
                    auto cap_name = capture.name();
                    if (cap_name != "grabNumber" && cap_name != "grabMsec") {
                        if (auto it = captures_.find(cap_name); it != captures_.end()) {
                            it->second.update(static_cast<int>(capture["grabMsec"]));
                        } else {
                            cerr << "Unknown capture name: " << cap_name << endl;
                        }
                    }
                }
            }
        } else {
            cerr << "Cannot parse yaml-filename: " << filename << endl;
        }

    } catch (const exception& e) {
        cerr << "Cannot parse yaml-filename: " << filename << '\n'
             << e.what() << endl;
    }
}

std::vector<FpsRec> Episode::getFps() const{
    std::vector<FpsRec> res;
    res.reserve(captures_.size());
    for (const auto& [name, capture] : captures_) {
        res.emplace_back(name, capture.calc_fps());
    }
    return res;
}


