#include "ParserHelper.h"
#include <fmt/core.h>
#include <fmt/ranges.h>

using namespace std::literals;

enum class type { noop, addx };

struct Instruction {
    type opcode;
    int value;

    explicit Instruction(type op, int val = 0) : opcode{op}, value{val} {}
};

class CRT {
    std::vector<Instruction> instructions;
    std::vector<int> registerX;

public:
    explicit CRT(std::string_view filename) {
        auto lines = ParserHelper::readLinesFromFile(filename);

        for (const auto& l : lines) {
            if (l.empty()) continue;
            if (l.starts_with("noop"sv))
                instructions.emplace_back(type::noop);
            else {
                auto strs = ParserHelper::parseStrsFromString(l, ' ');
                if (strs.size() != 2) continue;
                instructions.emplace_back(type::addx, std::stoi(strs[1]));
            }
        }

        registerX.push_back(1);
    }

    void printInstructions() const {
        for (const auto& i : instructions) {
            fmt::print("{}", (i.opcode == type::noop) ? "noop" : "addx");
            if (i.opcode == type::addx)
                fmt::print(" {}", i.value);
            fmt::print("\n");
        }
    }

    void printRegisterX() const {
        for (std::size_t i = 0; i < registerX.size(); ++i) {
            if (i % 20 == 0) fmt::print("\ni = {}: ", i);
            fmt::print("{}, ", registerX[i]);
        }
        fmt::print("Register X: {}\n", registerX);
    }

    void executeInstructions() {
        int value{1};
        for (const auto& i : instructions) {
            switch (i.opcode) {
                case type::noop:
                    registerX.push_back(value);
                    break;
                case type::addx:
                    registerX.push_back(value);
                    registerX.push_back(value);
                    value += i.value;
                    break;
                default: break;
            }
        }
    }

    int computeSumOfSignalStrengths() const {
        if (registerX.size() < 221) throw std::runtime_error("not enough cycles");

        std::array<int, 6> idx{20, 60, 100, 140, 180, 220};

        int sum{0};
        for (auto i : idx)
            sum += registerX[i] * i;

        return sum;
    }

    void printCRT() {
        if (registerX.size() < 241) throw std::runtime_error("not enough cycles");

        for (int i = 0; i < 240; ++i) {
            if (i % 40 == 0) {
                fmt::print("\n");
            }

            if (std::abs(registerX[i+1] - (i % 40)) <= 1)
                fmt::print("#");
            else fmt::print(".");
        }
    }
};

int main() {
    fmt::print("AoC 2022/12/10\n");

    try {
        CRT crt("../data/10-1.txt");
        crt.executeInstructions();
        fmt::print("Sum of signal strengths: {}\n", crt.computeSumOfSignalStrengths());
        crt.printCRT();
    }
    catch (std::exception& e) {
        fmt::print("Exception caught: {}\n", e.what());
    }

    return 0;
}