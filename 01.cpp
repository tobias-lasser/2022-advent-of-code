#include <string>
#include <string_view>
#include <fmt/core.h>
#include "ParserHelper.h"

class CalorieCounter {
    std::vector<int> sumCalories;

public:
    explicit CalorieCounter(std::string_view filename) {
        auto lines = ParserHelper::readLinesFromFile(filename);
        int currentCalories{0};
        for (const auto& l : lines) {
            if (l.empty()) {
                sumCalories.emplace_back(currentCalories);
                currentCalories = 0;
                continue;
            }
            currentCalories += std::stoi(l);
        }
    }

    int getMaxSumCalories() const {
        auto it = std::max_element(sumCalories.begin(), sumCalories.end());
        return *it;
    }

    int getTopThreeSumCalories() {
        std::sort(sumCalories.begin(), sumCalories.end(),
                  [](auto a, auto b) { return a > b; });
        if (sumCalories.size() < 3)
            throw std::logic_error("not enough elfs");

        return sumCalories[0] + sumCalories[1] + sumCalories[2];
    }
};

int main() {
    fmt::print("AOC 2022/12/01\n");

    try {
        CalorieCounter cc("../data/01-1.txt");
        fmt::print("Maximum calories of elf: {}\n", cc.getMaxSumCalories());
        fmt::print("Calories of top 3 elfs: {}\n", cc.getTopThreeSumCalories());
    }
    catch (std::exception& e) {
        fmt::print("Exception caught {}\n ", e.what());
    }

    return 0;
}
