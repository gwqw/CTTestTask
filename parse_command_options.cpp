#include "parse_command_options.h"

#include <iostream>
#include <utility>

#include <boost/program_options.hpp>

using namespace std;
namespace opt = boost::program_options;

boost::optional<std::tuple<std::string, int, int, std::string>>
    parse_command_oprions(int argc, char* argv[])
{
    opt::options_description desc("Usage: fps_framesdata [OPTIONS] \n"
                                  "This program counts average fps for episode files for all devices.\n"
                                  "All options:");
    desc.add_options()
            ("help,h", "This screen")
            ("dir,d", opt::value<std::string>(), "Input directory with *.info.yaml.gz files")
            ("start,s", opt::value<int>(), "episode number to start counting fps. Empty means from the first in directory")
            ("end,e", opt::value<int>(), "episode number to stop counting fps. Empty means to the last in directory")
            (",o", opt::value<std::string>(), "output filename (csv-table)");

    opt::command_line_parser parser{argc, argv};
    parser.options(desc).allow_unregistered();
    opt::parsed_options parsed_options = parser.run();

    opt::variables_map vm;
    opt::store(parsed_options, vm);
    opt::notify(vm);

    string inputDir, output_filename;
    int start{-1}, finish{-1};
    if (vm.count("help") || vm.count("dir") == 0) {
        cout << desc << endl;
        return boost::none;
    }
    inputDir = vm["dir"].as<string>();
    if (vm.count("start")) {
        start = vm["start"].as<int>();
        if (start < -1) start = -1;
    }
    if (vm.count("end")) {
        finish = vm["end"].as<int>();
        if (finish < -1) finish = -1;
    }
    if (vm.count("-o")) {
        output_filename = vm["-o"].as<string>();
    }
    return make_tuple(move(inputDir), start, finish, move(output_filename));
}