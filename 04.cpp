#include <fmt/core.h>
#include <fmt/ranges.h>
#include <array>
#include "ParserHelper.h"

struct ElfPair {
    std::array<int, 4> sections;

    bool firstContainsSecond() const {
        return (sections[2] >= sections[0] && sections[3] <= sections[1]);
    }

    bool secondContainsFirst() const {
        return (sections[0] >= sections[2] && sections[1] <= sections[3]);
    }

    bool overlaps() const {
        bool noOverlap = ((sections[1] < sections[2])
                          || (sections[3] < sections[0]));
        return !noOverlap;
    }
};

class Cleanup {
    std::vector<ElfPair> pairs;

public:
    explicit Cleanup(std::string_view filename) {
        auto lines = ParserHelper::readLinesFromFile(filename);
        pairs.reserve(lines.size());

        for (const auto& l : lines) {
            auto strs = ParserHelper::parseStrsFromString(l, ',');
            if (strs.size() != 2) continue;

            auto numbers1 = ParserHelper::parseIntsFromString(strs[0], '-');
            auto numbers2 = ParserHelper::parseIntsFromString(strs[1], '-');

            if (numbers1.size() != 2 || numbers2.size() != 2) continue;
            pairs.push_back({numbers1[0], numbers1[1], numbers2[0], numbers2[1]});
        }
    }

    int getFullOverlaps() const {
        int number{0};

        for (const auto& ep : pairs) {
            if (ep.firstContainsSecond() || ep.secondContainsFirst())
                ++number;
        }

        return number;
    }

    int getOverlaps() const {
        int number{0};

        for (const auto& ep : pairs) {
            if (ep.overlaps())
                ++number;
        }

        return number;
    }
};

int main() {
    fmt::print("AoC 2022/12/04\n");

    try {
        Cleanup cup("../data/04-1.txt");
        fmt::print("Full overlaps: {}\n", cup.getFullOverlaps());
        fmt::print("Overlaps: {}\n", cup.getOverlaps());
    }
    catch (std::exception& e) {
        fmt::print("Exception caught: {}\n", e.what());
    }

    return 0;
}