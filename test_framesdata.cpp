#define BOOST_TEST_MODULE fps_framesdata_test_module
#include <boost/test/unit_test.hpp>

#include <string>
#include <iostream>

#include "simple_decompress.h"
#include "framedata_reader.h"
#include "file_utils.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(fps_framesdata_test_suite)

	BOOST_AUTO_TEST_CASE(test_read_archive) {
        const string archive_path = "../archive_test/";
        {
            const string filename_in = archive_path + "bui.004.001.info.yml.gz";
            const string filename_out = archive_path + "bui.004.001.info.yml";
            bool res = decompress(filename_in, filename_out);
            BOOST_CHECK(res);
            const string filename_expexted = archive_path + "bui.004.001.info_orig.yml";
            auto obtained_size = get_file_size(filename_out);
            auto expected_size = get_file_size(filename_expexted);
            BOOST_CHECK(obtained_size == expected_size);
            BOOST_CHECK(cmp_files(filename_out, filename_expexted));
        }
        {
            const string filename_in = archive_path + "bui.004.001.info_broken.gz";
            const string filename_out = archive_path + "bui.004.001.info_broken.yml";
            bool res = decompress(filename_in, filename_out);
            BOOST_CHECK(!res);
        }
    }

    BOOST_AUTO_TEST_CASE(test_parse_yaml) {
        const string archive_path = "../archive_test/";
        {
            const string filename = archive_path + "fps_test.yml";
            Episode ep;
            ep.addDataFromFile(filename, false);
            auto res = ep.getFps();
            BOOST_CHECK(res.size() == 2u);
            for (const auto& r : res) {
                if (r.capture_name == "centerImage") {
                    BOOST_REQUIRE_CLOSE(r.fps, 2.51166128453534, 1e-6);
                } else if (r.capture_name == "leftImage") {
                    BOOST_REQUIRE_CLOSE(r.fps, 0.86792998698105, 1e-6);
                } else {
                    BOOST_CHECK(false);
                }
            }
        }
    }

BOOST_AUTO_TEST_SUITE_END()
