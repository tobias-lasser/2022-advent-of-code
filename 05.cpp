#include <array>
#include <stack>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include "ParserHelper.h"

struct Command {
    int howMany, from, where;
};

class Crates {
    std::vector<std::vector<char>> stacks;
    std::vector<Command> commands;

public:
    Crates(std::string_view filename) {
        // cheat by hand-coding stack initialization
        std::vector<std::vector<char>> content{ {'Q', 'S', 'W', 'C', 'Z', 'V', 'F', 'T'},
                                                {'Q', 'R', 'B'},
                                                {'B', 'Z', 'T', 'Q', 'P', 'M', 'S'},
                                                {'D', 'V', 'F', 'R', 'Q', 'H'},
                                                {'J', 'G', 'L', 'D', 'B', 'S', 'T', 'P'},
                                                {'W', 'R', 'T', 'Z'},
                                                {'H', 'Q', 'M', 'N', 'S', 'F', 'R', 'J'},
                                                {'R', 'N', 'F', 'H', 'W'},
                                                {'J', 'Z', 'T', 'Q', 'P', 'R', 'B'}};
        for (const auto& cs : content) {
            std::vector<char> s;
            for (auto c : cs)
                s.push_back(c);
            stacks.push_back(s);
        }

        auto lines = ParserHelper::readLinesFromFile(filename);

        for (const auto& l : lines) {
            if (l.empty() || l[0] != 'm') continue;

            auto strs = ParserHelper::parseStrsFromString(l, ' ');
            if (strs.size() != 6) continue;

            Command c;
            c.howMany = std::stoi(strs[1]);
            c.from = std::stoi(strs[3]) - 1;
            c.where = std::stoi(strs[5]) - 1;
            if (c.from < 0 || c.from > 9 || c.where < 0 || c.where > 9)
                throw std::runtime_error("parsed invalid stack number");
            commands.push_back(c);
        }
    }

    std::string executeCommands9000() {
        for (const auto& c : commands) {
            if (stacks[c.from].size() < c.howMany)
                throw std::runtime_error("stack does not have enough elements for command");
            for (int i = 0; i < c.howMany; ++i) {
                stacks[c.where].push_back(stacks[c.from].back());
                stacks[c.from].pop_back();
            }
        }

        std::string result;
        for (const auto& s : stacks) {
            if (s.empty()) continue;
            result += s.back();
        }

        return result;
    }

    std::string executeCommands9001() {
        for (const auto& c : commands) {
            if (stacks[c.from].size() < c.howMany)
                throw std::runtime_error("stack does not have enough elements for command");
            std::vector<char> temp;
            for (int i = 0; i < c.howMany; ++i) {
                temp.push_back(stacks[c.from].back());
                stacks[c.from].pop_back();
            }
            for (int i = 0; i < c.howMany; ++i) {
                stacks[c.where].push_back(temp.back());
                temp.pop_back();
            }
        }

        std::string result;
        for (const auto& s : stacks) {
            if (s.empty()) continue;
            result += s.back();
        }

        return result;
    }
};

int main() {
    fmt::print("AoC 2022/12/05\n");

    try {
        Crates c("../data/05-1.txt");
        fmt::print("Top of stacks after commands: {}\n", c.executeCommands9001());
    }
    catch (std::exception& e) {
        fmt::print("Exception caught: {}\n", e.what());
    }

    return 0;
}