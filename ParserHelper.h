#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <fmt/core.h>
#include <string_view>
#include <range/v3/view.hpp>
#include <range/v3/range.hpp>

class ParserHelper {
public:
    /// open and read file "fileName", return all lines in file as a vector of strings
    static std::vector<std::string> readLinesFromFile(std::string_view fileName);

    /// parse comma separated list of integers from string
    static std::vector<int> parseIntsFromString(std::string_view string, char delimiter);

    /// parse comma separated list of strings from string
    static std::vector<std::string> parseStrsFromString(std::string_view string, char delimiter);
};


std::vector<std::string> ParserHelper::readLinesFromFile(std::string_view fileName)
{
    std::ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        throw std::runtime_error(fmt::format("error opening file {}", fileName));
    }

    std::vector<std::string> fileLines;
    std::string line;
    while (std::getline(inputFile, line)) {
        fileLines.push_back(line);
    }

    fmt::print("Read {} lines from {}\n", fileLines.size(), fileName);
    return fileLines;
}

std::vector<int> ParserHelper::parseIntsFromString(std::string_view string, char delimiter)
{
    auto splitText = string | ranges::views::split(delimiter) | ranges::to<std::vector<std::string>>();

    std::vector<int> vec;
    vec.reserve(splitText.size());
    for (const auto& s : splitText) {
        if (s.empty()) continue;
        vec.emplace_back(std::stoi(s));
    }

    return vec;
}

std::vector<std::string> ParserHelper::parseStrsFromString(std::string_view string, char delimiter)
{
    auto splitText = string | ranges::views::split(delimiter) | ranges::to<std::vector<std::string>>();

    return splitText;
}

// end of ParserHelper.h