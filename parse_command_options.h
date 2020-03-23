#pragma once

#include <string>

#include <boost/optional.hpp>

boost::optional<std::tuple<std::string, int, int, std::string>>
    parse_command_oprions(int argc, char* argv[]);