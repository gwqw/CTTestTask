#define BOOST_TEST_MODULE fps_framesdata_test_module
#include <boost/test/unit_test.hpp>

#include <string>
#include <iostream>

#include "framedata_reader.h"
#include "file_utils.h"
#include "string_utils.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(fps_framesdata_test_suite)

    BOOST_AUTO_TEST_CASE(test_parse_yaml1) {
        const string archive_path = "../tests/archive_test/";
        {
            const string filename = archive_path + "bui.004.001.info.yml";
            Episode ep;
            ep.addDataFromFile(filename);
            BOOST_CHECK(ep.getReadedCapturesCount() == 6u);
        }
        {
            const string filename = archive_path + "bui.004.001.info.yml.gz";
            Episode ep;
            ep.addDataFromFile(filename);
            BOOST_CHECK(ep.getReadedCapturesCount() == 6u);
        }
        {
            const string filename = archive_path + "bui.004.001.info_broken.yml.gz";
            Episode ep;
            ep.addDataFromFile(filename);
            BOOST_CHECK(ep.getReadedCapturesCount() == 0u);
        }
    }

    BOOST_AUTO_TEST_CASE(test_parse_yaml2) {
        const string archive_path = "../tests/archive_test/";
        {
            const string filename = archive_path + "fps_test.yml";
            Episode ep;
            ep.addDataFromFile(filename);
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
        const string test_path = "../tests/filelist_test";
        {
            auto filelist = getFileList(test_path, -1, -1);
            BOOST_CHECK(filelist.size() == 10u);
            vector<string> expected {
                test_path + getSlash() + "bui.004.001.info.yml.gz",
                test_path + getSlash() + "bui.004.002.info.yml.gz",
                test_path + getSlash() + "bui.004.003.info.yml.gz",
                test_path + getSlash() + "bui.004.004.info.yml.gz",
                test_path + getSlash() + "bui.004.005.info.yml.gz",
                test_path + getSlash() + "bui.004.006.info.yml.gz",
                test_path + getSlash() + "bui.004.007.info.yml.gz",
                test_path + getSlash() + "bui.004.008.info.yml.gz",
                test_path + getSlash() + "bui.004.009.info.yml.gz",
                test_path + getSlash() + "bui.004.010.info.yml.gz",
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