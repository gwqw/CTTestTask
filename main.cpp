#include <iostream>

#include <boost/program_options.hpp>

#include "parse_command_options.h"
#include "framedata_reader.h"
#include "simple_decompress.h"
#include "file_utils.h"

using namespace std;
namespace opt = boost::program_options;

int main(int argc, char* argv[]) {
    try {
        auto optional_options = parse_command_oprions(argc, argv);
        if (!optional_options) {
            return 0;
        }
        auto [inputDir, startEpisodeNumber, endEpisodeNumber, ouputFname] =
            *optional_options;
        auto file_list = getFileList(inputDir, startEpisodeNumber, endEpisodeNumber);
        Episode episode;
        for (const auto& filename : file_list) {
            Decompressor decompressor(filename);
            if (decompressor.isDecompressed()) {
                episode.addDataFromFile(decompressor.getFileName());
            }
        }
        auto res = episode.getFps();
        saveToCSV(res, ouputFname);

        return 0;

    } catch (const opt::error& e) {
        cerr << e.what() << endl;
        return 1;
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return 2;
    }
}
