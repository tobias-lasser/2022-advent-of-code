#include "ParserHelper.h"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <unordered_set>

struct Command {
    char dir;
    int repeats;

    Command(char d, int r) : dir{d}, repeats{r} {}
};

struct Position {
    int x{0}, y{0};

    Position() = default;
    Position(int a, int b) : x{a}, y{b} {}
    bool operator==(const Position& p ) const { return (p.x == x) && (p.y == y); }
};

auto hashFunc = [](const Position &l) {
    return l.x << 16 ^ l.y;
};


class Rope {
    static constexpr std::size_t knots = 10;
    std::vector<Command> commands;
    std::array<Position, knots> pos;
    std::unordered_set<Position, decltype(hashFunc)> tailPositions;

public:
    explicit Rope(std::string_view filename) {
        auto lines = ParserHelper::readLinesFromFile(filename);

        commands.reserve(lines.size());
        for (const auto& l : lines) {
            if (l.empty()) continue;
            commands.emplace_back(l[0], std::stoi(l.substr(2, std::string::npos)));
        }

        tailPositions.emplace(pos.back().x, pos.back().y);
    }

    void updateHeadPosition(char dir) {
        switch (dir) {
            case 'L': pos.front().x--; break;
            case 'R': pos.front().x++; break;
            case 'U': pos.front().y++; break;
            case 'D': pos.front().y--; break;
            default: break;
        }
    }

    void updateTailPosition(std::size_t idx) {
        if (idx >= pos.size() || idx < 1) throw std::runtime_error("idx wrong");

        int diffX = pos[idx-1].x - pos[idx].x;
        int diffY = pos[idx-1].y - pos[idx].y;

        // tail next to head
        if (diffX == 0 && std::abs(diffY) <= 1) return;
        if (diffY == 0 && std::abs(diffX) <= 1) return;

        // in same row or column
        if (diffX == 0) { if (diffY > 0) pos[idx].y++; else pos[idx].y--; }
        if (diffY == 0) { if (diffX > 0) pos[idx].x++; else pos[idx].x--; }

        // diagonal move
        if ((diffX != 0 && diffY != 0) &&
            (std::abs(diffX) > 1 || std::abs(diffY) > 1)) {
            if (diffX > 0) pos[idx].x++; else pos[idx].x--;
            if (diffY > 0) pos[idx].y++; else pos[idx].y--;
        }

        if (idx == pos.size()-1)
            tailPositions.emplace(pos.back().x, pos.back().y);
    }

    std::size_t countTailPositions(bool verbose = false) {
        if (verbose) printPositions();
        for (const auto& c : commands) {
            if (verbose) fmt::print("Command {} {}:\n", c.dir, c.repeats);
            for (int i = 0; i < c.repeats; ++i) {
                updateHeadPosition(c.dir);
                for (std::size_t k = 1; k < knots; ++k)
                    updateTailPosition(k);
                if (verbose) printPositions();
            }
        }

        return tailPositions.size();
    }

    void printPositions() const {
        fmt::print("H = ({},{}); ", pos.front().x, pos.front().y);
        for (std::size_t i = 1; i < pos.size(); ++i)
            fmt::print("{} = ({},{}); ", i, pos[i].x, pos[i].y);
        fmt::print("\n");
    }
};

int main() {
    fmt::print("AoC 2022/12/09\n");

    try {
        Rope r("../data/09-1.txt");
        fmt::print("Number of tail positions: {}\n", r.countTailPositions());
    }
    catch (std::exception& e) {
        fmt::print("Exception caught: {}\n", e.what());
    }

    return 0;
}