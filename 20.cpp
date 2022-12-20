#include "ParserHelper.h"
#include <fmt/core.h>
#include <algorithm>
#include <list>

class GPS {
    std::list<int64_t> numbers;
    std::vector<std::string> lines;

public:
    explicit GPS(std::string_view filename) {
        lines = ParserHelper::readLinesFromFile(filename);
        for (const auto& l : lines)
            numbers.push_back(std::stoi(l));
    }

    void mix(int howOften = 1) {
        std::vector<decltype(numbers)::const_iterator> iters;
        iters.reserve(numbers.size());
        for (auto iter = numbers.cbegin(); iter != numbers.cend(); ++iter)
            iters.push_back(iter);

        int len = numbers.size() - 1;

        for (int kk = 0; kk < howOften; ++kk) {
            for (auto &i: iters) {
                int64_t count = *i % len;
                if (count == 0)
                    continue;

                int64_t currentIndex = std::distance(numbers.cbegin(), i);
                int64_t newIndex = currentIndex + count;

                auto it = numbers.cbegin();
                if (count > 0) {
                    if (newIndex >= len + 1) {
                        newIndex %= len;
                        it = std::next(numbers.cbegin(), newIndex);
                    } else
                        it = std::next(numbers.cbegin(), newIndex + 1);
                } else {
                    if (newIndex <= 0)
                        it = std::next(numbers.cend(), newIndex);
                    else
                        it = std::next(numbers.cbegin(), newIndex);
                }

                numbers.splice(it, numbers, i);
            }
        }
    }

    auto advance1000Steps(auto iter) {
        for (int i = 0; i < 1000; ++i) {
            if (++iter == numbers.cend())
                iter = numbers.cbegin();
        }
        return iter;
    }

    int64_t getGroveCoords(int howOften = 1) {
        mix(howOften);

        int64_t sum{0};

        auto it = std::find(numbers.cbegin(), numbers.cend(), 0);
        for (int i = 0; i < 3; ++i) {
            it = advance1000Steps(it);
            sum += *it;
        }

        return sum;
    }

    void resetAndApplyEncryption() {
        const int64_t magicNumber = 811589153;
        numbers.clear();
        for (const auto& l : lines)
            numbers.push_back(std::stoi(l) * magicNumber);
    }
};

int main() {
    fmt::print("AoC 2022/12/20\n");

    try {
        GPS gps("../data/20-1.txt");
        fmt::print("Grove coordinates sum: {}\n", gps.getGroveCoords());
        gps.resetAndApplyEncryption();
        fmt::print("Grove coordinates decrypted: {}\n", gps.getGroveCoords(10));
    }
    catch (std::exception& e) {
        fmt::print("Exception caught: {}\n", e.what());
    }

    return 0;
}