#include "framedata_reader.h"

#include <cassert>
#include <fstream>
#include <algorithm>

#include <yaml-cpp/yaml.h>

using namespace std;

void Capture::update(uint64_t msec) {
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


void YamlPreprocessor::yamlPreprocessor(const std::string &filename) {
    ifstream in(filename);
    string out_fname = filename + "_tmp";
    ofstream out(filename + "_tmp");
    for (string line; getline(in, line); ) {
        if (line == "%YAML:1.0") continue;
        out << processLine(line) << '\n';
    }
    in.close();
    remove(filename.c_str());
    rename(out_fname.c_str(), filename.c_str());
}

std::string YamlPreprocessor::processLine(std::string& line) {
    std::size_t colon_count = count(line.begin(), line.end(), ':');
    string res;
    res.reserve(line.size() + colon_count);
    for (size_t i = 0; i < line.size(); ++i) {
        res.push_back(line[i]);
        // add space after ':' if needed
        if (line[i] == ':'
            && i+1 != line.size()  && line[i+1] != ' ')
        {
            res.push_back(' ');
        }
    }
    return res;
}


void Episode::addDataFromFile(const std::string &filename, bool needToProcess) {
    // preprocess file, because it is not valid yaml-file
    try {
        if (needToProcess) {
            YamlPreprocessor::yamlPreprocessor(filename);
        }

        auto yaml = YAML::LoadFile(filename);
        // treat header
        auto header = yaml["header"];
        auto captures = header["captures"];
        for (const auto &capture : captures) {
            const auto &cap_data = capture.second;
            if (captures_.count(cap_data["name"].as<string>()) == 0) {
                captures_[cap_data["name"].as<string>()] = Capture{};
            }
        }

        // treat captures body (shots)
        auto shots = yaml["shots"];
        for (const auto &shot : shots) {
            for (const auto &capture : shot) {
                auto cap_name = capture.first.as<string>();
                const auto &cap_data = capture.second;
                if (cap_name != "grabNumber" && cap_name != "grabMsec") {
                    if (auto it = captures_.find(cap_name); it != captures_.end()) {
                        it->second.update(cap_data["grabMsec"].as<uint64_t>());
                    } else {
                        cerr << "Unknown capture name: " << cap_name << endl;
                    }
                }
            }
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

double Episode::getFpsForName(const std::string &capture_name) const {
    return captures_.at(capture_name).calc_fps();
}
