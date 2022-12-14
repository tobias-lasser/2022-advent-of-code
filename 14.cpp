#include "ParserHelper.h"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <optional>

struct Coordinate {
    int x, y;
    Coordinate() : x{-1}, y{-1} {}
    Coordinate(int x_, int y_) : x{x_}, y{y_} {}
};

class SandCastle {
    std::vector<std::vector<Coordinate>> rocks;
    std::vector<std::vector<char>> grid;
    Coordinate sandStart;
    int maxY{0}, maxX{0};

public:
    explicit SandCastle(std::string_view filename) {
        auto lines = ParserHelper::readLinesFromFile(filename);

        rocks.reserve(lines.size());
        for (const auto& l : lines) {
            std::vector<Coordinate> coords;
            auto strs = ParserHelper::parseStrsFromString(l, ' ');
            for (const auto& s : strs) {
                if (s == "->") continue;
                auto ints = ParserHelper::parseIntsFromString(s, ',');
                if (ints.size() != 2) continue;
                coords.emplace_back(ints[0], ints[1]);
            }
            rocks.push_back(coords);
        }
    }

    std::pair<Coordinate, Coordinate> getMinMaxCoords() {
        Coordinate cmax{-1, -1};
        Coordinate cmin{10000, 10000};
        for (const auto& r : rocks) {
            for (const auto& c : r) {
                if (c.x > cmax.x) cmax.x = c.x;
                if (c.y > cmax.y) cmax.y = c.y;
                if (c.x < cmin.x) cmin.x = c.x;
                if (c.y < cmin.y) cmin.y = c.y;
            }
        }
        if (cmin.y > 0) cmin.y = 0;
        cmax.y += 2;
        int extraX = ((cmax.x - cmin.x) / 2 + 1) * 3;
        cmin.x -= extraX; cmax.x += extraX;
        return std::make_pair(cmin, cmax);
    }

    void buildGrid(bool withFloor = false) {
        auto [cmin, cmax] = getMinMaxCoords();
        maxY = cmax.y - cmin.y;
        maxX = cmax.x - cmin.x;

        grid.clear();
        grid.reserve(cmax.y-cmin.y);
        for (int j = cmin.y; j <= cmax.y; ++j) {
            std::vector<char> row;
            row.reserve(cmax.x-cmin.x);
            for (int i = cmin.x; i <= cmax.x; ++i)
                row.push_back('.');
            grid.push_back(row);
        }

        for (const auto& r : rocks) {
            for (int i = 0; i < r.size()-1; ++i) {
                Coordinate start, stop;
                if (r[i].x < r[i + 1].x) {
                    start = r[i];
                    stop = r[i + 1];
                } else if (r[i].x > r[i + 1].x) {
                    start = r[i + 1];
                    stop = r[i];
                } else if (r[i].y < r[i + 1].y) {
                    start = r[i];
                    stop = r[i + 1];
                } else {
                    start = r[i + 1];
                    stop = r[i];
                }

                for (int y = start.y - cmin.y; y <= stop.y - cmin.y; ++y) {
                    for (int x = start.x - cmin.x; x <= stop.x - cmin.x; ++x) {
                        grid[y][x] = '#';
                    }
                }
            }
        }

        if (withFloor) {
            for (int x = 0; x <= cmax.x - cmin.x; ++x) {
                grid[cmax.y - cmin.y][x] = '#';
            }
        }

        sandStart.x = 500 - cmin.x;
        sandStart.y = 0 - cmin.y;
        grid[sandStart.y][sandStart.x] = '+';
    }

    void printGrid() const {
        for (const auto& g : grid) {
            for (auto c : g)
                fmt::print("{}", c);
            fmt::print("\n");
        }
    }

    std::optional<Coordinate> movePossible(const Coordinate& c) {
        if (grid[c.y+1][c.x] == '.')
            return std::make_optional(Coordinate{c.x, c.y+1});
        else if (grid[c.y+1][c.x-1] == '.')
            return std::make_optional(Coordinate{c.x - 1, c.y + 1});
        else if (grid[c.y+1][c.x+1] == '.')
            return std::make_optional(Coordinate{c.x + 1, c.y + 1});
        else
            return std::nullopt;
    }

    bool newSandUnit(bool floor = false) {
        Coordinate current = sandStart;

        auto c = movePossible(current);
        while (c) {
            if (c->y >= maxY) return false;
            if (floor && (c->x < 0 || c->x > maxX)) return false;
            current = *c;
            c = movePossible(current);
        }

        grid[current.y][current.x] = 'o';
        return true;
    }

    int pourSand() {
        int counter{0};

        while (newSandUnit())
            ++counter;

        return counter;
    }

    int findSafeSpot() {
        int counter{0};

        while (newSandUnit(true)) {
            ++counter;
            if (grid[sandStart.y][sandStart.x] == 'o')
                break;
        }

        return counter;
    }
};

int main() {
    fmt::print("AoC 2022/12/14\n");

    try {
        SandCastle s("../data/14-1.txt");
        s.buildGrid();
        auto i = s.pourSand();
        s.printGrid();
        fmt::print("sand poured: {}\n", i);

        s.buildGrid(true);
        auto j = s.findSafeSpot();
        s.printGrid();
        fmt::print("source blocked after: {}\n", j);
    }
    catch (std::exception& e) {
        fmt::print("Exception caught: {}\n", e.what());
    }

    return 0;
}