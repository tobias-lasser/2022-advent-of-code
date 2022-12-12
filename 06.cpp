#include <unordered_set>
#include "ParserHelper.h"

class Signal {
    std::string signal;
public:
    explicit Signal(std::string_view filename) {
        auto lines = ParserHelper::readLinesFromFile(filename);
        if (lines.empty()) throw std::logic_error("empty input");
        signal = lines[0];
    }

    bool isValidMarker(std::size_t i, std::size_t packetSize = 3) {
        if (i > signal.size()) throw std::runtime_error("wrong index");
        if (i < packetSize) return false;

        bool dupFound{false};
        std::unordered_set<char> c;
        for (std::size_t j = i-packetSize; j <= i; ++j) {
            if (c.contains(signal[j])) {
                dupFound = true;
                break;
            }
            else
                c.insert(signal[j]);
        }

        return !dupFound;
    }

    std::size_t findMarker(std::size_t packetSize) {
        for (std::size_t i = 0; i < signal.size(); ++i) {
            if (isValidMarker(i, packetSize))
                return i+1;
        }
        return 0;
    }
};

int main() {
    fmt::print("AoC 2022/12/06\n");

    try {
        Signal s("../data/06-1.txt");
        fmt::print("Marker found at {}\n", s.findMarker(3));
        fmt::print("Other Marker found at {}\n", s.findMarker(13));
    }
    catch (std::exception& e) {
        fmt::print("Exception caught: {}\n", e.what());
    }

    return 0;
}