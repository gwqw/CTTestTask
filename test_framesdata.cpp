#define BOOST_TEST_MODULE fps_framesdata_test_module
#include <boost/test/unit_test.hpp>

#include <string>
#include <iostream>

#include "simple_decompress.h"
#include "framedata_reader.h"
#include "file_utils.h"
#include "string_utils.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(fps_framesdata_test_suite)

	BOOST_AUTO_TEST_CASE(test_read_archive) {
        const string archive_path = "../tests/archive_test/";
        {
            const string filename_in = archive_path + "bui.004.001.info.yml.gz";
            const string filename_out = archive_path + "bui.004.001.info.yml";
            Decompressor d(filename_in);
            BOOST_CHECK(d.isDecompressed());
            const string filename_expected = archive_path + "bui.004.001.info_orig.yml";
            auto obtained_size = get_file_size(filename_out);
            auto expected_size = get_file_size(filename_expected);
            BOOST_CHECK(obtained_size == expected_size);
            BOOST_CHECK(cmp_files(filename_out, filename_expected));
        }
        {
            const string filename_in = archive_path + "bui.004.001.info_broken.yml.gz";
            Decompressor d(filename_in);
            BOOST_CHECK(!d.isDecompressed());
        }
    }

    BOOST_AUTO_TEST_CASE(test_parse_yaml) {
        const string archive_path = "../tests/archive_test/";
        {
            const string filename = archive_path + "fps_test.yml";
            Episode ep;
            ep.addDataFromFile(filename, false);
            auto res = ep.getFps();
            BOOST_CHECK(res.size() == 2u);
            for (const auto& r : res) {
                if (r.capture_name == "centerImage") {
                    BOOST_REQUIRE_CLOSE(r.fps, 2.15285252960172, 1e-6);
                } else if (r.capture_name == "leftImage") {
                    BOOST_REQUIRE_CLOSE(r.fps, 0.723274989150875, 1e-6);
                } else {
                    BOOST_CHECK(false);
                }
            }
        }
    }

    BOOST_AUTO_TEST_CASE(test_get_filelist) {
        const string test_path = "../tests/filelist_test/";
        {
            auto filelist = getFileList(test_path, -1, -1);
            BOOST_CHECK(filelist.size() == 10u);
            vector<string> expected {
                test_path + "bui.004.001.info.yml.gz",
                test_path + "bui.004.002.info.yml.gz",
                test_path + "bui.004.003.info.yml.gz",
                test_path + "bui.004.004.info.yml.gz",
                test_path + "bui.004.005.info.yml.gz",
                test_path + "bui.004.006.info.yml.gz",
                test_path + "bui.004.007.info.yml.gz",
                test_path + "bui.004.008.info.yml.gz",
                test_path + "bui.004.009.info.yml.gz",
                test_path + "bui.004.010.info.yml.gz",
            };
            BOOST_CHECK(filelist == expected);
        }
    }

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(utils_test_suite)

    BOOST_AUTO_TEST_CASE(test_str_utils) {
        auto words = split_words("bui.004.001.info.yml.gz", '.');
        BOOST_CHECK(words.size() == 6u);
        vector<string> expected{"bui", "004", "001", "info", "yml", "gz"};
        BOOST_CHECK(words == expected);
    }

    BOOST_AUTO_TEST_CASE(test_file_utils) {
        BOOST_CHECK(
                isCorrectFileName("bui.004.001.info.yml.gz", -1, -1)
        );
        BOOST_CHECK(
                isCorrectFileName("bui.004.001.info.yml.gz", 0, -1)
        );
        BOOST_CHECK(
                isCorrectFileName("bui.004.001.info.yml.gz", 1, -1)
        );
        BOOST_CHECK(
                isCorrectFileName("bui.004.001.info.yml.gz", 1, 1)
        );
        BOOST_CHECK(
                isCorrectFileName("bui.004.001.info.yml.gz", 1, -1)
        );
        BOOST_CHECK(
                isCorrectFileName("bui.004.001.info.yml.gz", 1, 2)
        );
        BOOST_CHECK(
                !isCorrectFileName("bui.004.001.info.yml.gz", 2, 2)
        );
        BOOST_CHECK(
                !isCorrectFileName("bui.004.001.info.yml.gz", 2, -1)
        );
    }

BOOST_AUTO_TEST_SUITE_END()