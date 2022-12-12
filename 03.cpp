#include <string_view>
#include <string>
#include <cctype>
#include <fmt/core.h>
#include "ParserHelper.h"

struct Compartments {
    std::string  left, right;
};


class Backpack {
    std::vector<Compartments> packs;

public:
    explicit Backpack(std::string_view filename) {
        auto lines = ParserHelper::readLinesFromFile(filename);

        for (const auto& l : lines) {
            if (l.empty()) continue;
            packs.push_back(Compartments{l.substr(0, l.size()/2), l.substr(l.size()/2, l.npos)});
        }
    }

    int getPriority(char c) {
        if (isupper(c))
            return c - 'A' + 27;
        if (islower(c))
            return c - 'a' + 1;

        return 0;
    }

    int getDuplicatePriority() {
        int priority{0};

        for (auto& cmp : packs) {
            if (cmp.left.size() != cmp.right.size()) continue;

            for (const auto& c : cmp.left) {
                if (cmp.right.find(c) != cmp.right.npos) {
                    priority += getPriority(c);
                    break;
                }
            }
        }

        return priority;
    }

    int getBadgePriorities() {
        int priority{0};

        if (packs.size() % 3 != 0) throw std::logic_error("packs not divisible by 3");

        for (std::size_t i = 0; i < packs.size(); i += 3) {
            const auto a = packs[i].left + packs[i].right;
            const auto b = packs[i+1].left + packs[i+1].right;
            const auto c = packs[i+2].left + packs[i+2].right;

            for (const auto& ch : a) {
                if (b.find(ch) != b.npos && c.find(ch) != c.npos) {
                    priority += getPriority(ch);
                    break;
                }
            }
        }

        return priority;
    }
};

int main() {
    fmt::print("AoC 2022/12/03\n");

    try {
        Backpack bp("../data/03-1.txt");
        fmt::print("Duplicate priority: {}\n", bp.getDuplicatePriority());
        fmt::print("Badge priorities: {}\n", bp.getBadgePriorities());
    }
    catch (std::exception& e) {
        fmt::print("Exception caught: {}\n");
    }

    return 0;
}