#include "ParserHelper.h"
#include <fmt/core.h>
#include <fmt/ranges.h>

using namespace std::literals;

struct Monkey {
    int id;
    std::vector<long long> items;
    std::function<long long(long long)> operation;
    std::function<bool(long long)> test;
    int testModulus;
    std::pair<int, int> targets;
    int active{0};
};

class MonkeyBusiness {
    std::vector<Monkey> monkeys;
    long long modulus{1};

public:
    explicit MonkeyBusiness(std::string_view filename) {
        auto lines = ParserHelper::readLinesFromFile(filename);

        for (std::size_t i = 0; i < lines.size(); ++i) {
            Monkey m;
            if (lines[i].starts_with("Monkey"sv)) {
                m.id = std::stoi(lines[i].substr(6, std::string::npos));
                fmt::print("Monkey id = {}: ", m.id);
            }
            ++i;
            if (lines[i].starts_with("  Starting items: "sv)) {
                auto numbers = ParserHelper::parseIntsFromString(lines[i].substr(17, std::string::npos), ',');
                for (auto i : numbers)
                    m.items.push_back(i);
                fmt::print("items = {}; ", m.items);
            }
            ++i;
            if (lines[i].starts_with("  Operation: "sv)) {
                auto strs = ParserHelper::parseStrsFromString(lines[i].substr(12, std::string::npos), ' ');
                char op = strs[4][0];
                int operand;
                if (strs[5] == "old")
                    operand = -1;
                else
                    operand = std::stoi(strs[5]);
                fmt::print("operation: op = {}, operand = {}; ", op, operand);
                switch (op) {
                    case '+':
                        if (operand == -1)
                            m.operation = [operand](auto old) { return old + old; };
                        else
                            m.operation = [operand](auto old) { return old + operand; };
                        break;
                    case '*':
                        if (operand == -1)
                            m.operation = [operand](auto old) { return old * old; };
                        else
                            m.operation = [operand](auto old) { return old * operand; };
                        break;
                    default:
                        throw std::runtime_error("invalid op");
                }
            }
            ++i;
            if (lines[i].starts_with("  Test: "sv)) {
                auto strs = ParserHelper::parseStrsFromString(lines[i].substr(7, std::string::npos), ' ');
                int operand = std::stoi(strs[3]);
                fmt::print("test: operand = {}; ", operand);
                m.test = [operand](int worry) { return (worry % operand == 0); };
                m.testModulus = operand;
                modulus *= operand;
            }
            ++i;
            if (lines[i].starts_with("    If true"sv)) {
                auto strs = ParserHelper::parseStrsFromString(lines[i].substr(12, std::string::npos), ' ');
                int idTrue = std::stoi(strs[4]);
                ++i;
                if (lines[i].starts_with("    If false"sv)) {
                    auto strs2 = ParserHelper::parseStrsFromString(lines[i].substr(13, std::string::npos), ' ');
                    int idFalse = std::stoi(strs2[4]);

                    fmt::print("throw true = {}, false = {}\n", idTrue, idFalse);
                    m.targets = std::make_pair(idTrue, idFalse);
                }
                ++i;
            }
            monkeys.push_back(m);
        }
    }

    void playRound(bool divby3 = true) {
        for (auto& monkey : monkeys) {
            for (auto& i : monkey.items) {
                i = monkey.operation(i);
                if (divby3) i /= 3;
                i %= modulus;
                if (monkey.test(i))
                    monkeys[monkey.targets.first].items.push_back(i);
                else
                    monkeys[monkey.targets.second].items.push_back(i);
            }
            monkey.active += monkey.items.size();
            monkey.items.clear();
        }
    }

    void printMonkeys() const {
        for (const auto& m: monkeys) {
            fmt::print("Monkey {}: {}\n", m.id, m.items);
        }
    }

    std::vector<long long> getActivityOverRounds(int rounds, bool divby3 = true, bool verbose = false) {
        if (verbose) printMonkeys();
        for (int i = 0; i < rounds; ++i) {
            playRound(divby3);
            if (verbose) {
                fmt::print("Round {}:\n", i+1);
                printMonkeys();
                fmt::print("\n");
            }
        }

        std::vector<long long> activities;
        for (const auto& m : monkeys)
            activities.push_back(m.active);

        return activities;
    }

    long long getMonkeyBusiness(int rounds, bool divby3 = true, bool verbose = true) {
        auto act = getActivityOverRounds(rounds, divby3, verbose);
        fmt::print("activities: {}\n", act);
        std::sort(act.begin(), act.end(), [](auto a, auto b) { return a > b; });
        if (act.size() < 2)
            return -1;
        else
            return act[0] * act[1];
    }
};

int main() {
    fmt::print("AoC 2022/12/11\n");

    try {
        MonkeyBusiness m("../data/11-1.txt");
//        fmt::print("monkey business 20: {}\n", m.getMonkeyBusiness(20, true, true));
        fmt::print("monkey business 10000: {}\n", m.getMonkeyBusiness(10000, false, false));
    }
    catch (std::exception& e) {
        fmt::print("Exception caught: {}\n", e.what());
    }

    return 0;
}