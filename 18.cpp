#include "ParserHelper.h"
#include <fmt/core.h>
#include <queue>

struct Coord {
    int x, y, z;
};

class Lava {
    std::vector<Coord> coords;
    std::vector<std::vector<std::vector<int>>> grid;

public:
    explicit Lava(std::string_view filename) {
        auto lines = ParserHelper::readLinesFromFile(filename);

        coords.reserve(lines.size());
        for (const auto& l : lines) {
            auto ints = ParserHelper::parseIntsFromString(l, ',');
            if (ints.size() != 3) continue;
            coords.push_back({ints[0], ints[1], ints[2]});
        }

        int maxX = std::max_element(coords.begin(), coords.end(),
                                    [] (auto a, auto b) { return a.x < b.x; })->x;
        int maxY = std::max_element(coords.begin(), coords.end(),
                                    [] (auto a, auto b) { return a.y < b.y; })->y;
        int maxZ = std::max_element(coords.begin(), coords.end(),
                                    [] (auto a, auto b) { return a.z < b.z; })->z;

        grid.resize(maxX+3);
        for (auto& gy : grid) {
            gy.resize(maxY+3);
            for (auto& gz : gy)
                gz.resize(maxZ+3);
        }

        for (const auto& c : coords)
            grid[c.x+1][c.y+1][c.z+1] = 1;
    }

    int countSides() const {
        int sides{0};

        for (int x = 0; x < grid.size(); ++x) {
            for (int y = 0; y < grid[x].size(); ++y) {
                for (int z = 0; z < grid[x][y].size(); ++z) {
                    if (grid[x][y][z] == 0) continue;
                    if (x-1 < 0 || grid[x-1][y][z] == 0) ++sides;
                    if (x+1 >= grid.size() || grid[x+1][y][z] == 0) ++sides;
                    if (y-1 < 0 || grid[x][y-1][z] == 0) ++sides;
                    if (y+1 >= grid[x].size() || grid[x][y+1][z] == 0) ++sides;
                    if (z-1 < 0 || grid[x][y][z-1] == 0) ++sides;
                    if (z+1 >= grid[x][y].size() || grid[x][y][z+1] == 0) ++sides;
                }
            }
        }
        return sides;
    }

    static constexpr auto deltas = std::array{ std::array{-1, 0, 0}, std::array{1, 0, 0},
                                        std::array{0, -1, 0}, std::array{0, 1, 0},
                                        std::array{0, 0, -1}, std::array{0, 0, 1}};

    int countOuterSides() {
        int counter{0};

        // propagate steam from 0,0,0
        std::queue<Coord> q;
        q.push({0, 0, 0});
        grid[0][0][0] ^= 2;
        while (!q.empty()) {
            auto cc = q.front();
            q.pop();

            for (auto [dx, dy, dz]: deltas) {
                bool xFine{dx == 0 || (dx < 0 && cc.x > 0) || (dx > 0 && cc.x < grid.size() - 1)};
                bool yFine{dy == 0 || (dy < 0 && cc.y > 0) || (dy > 0 && cc.y < grid[cc.x].size() - 1)};
                bool zFine{dz == 0 || (dz < 0 && cc.z > 0) || (dz > 0 && cc.z < grid[cc.x][cc.y].size() - 1)};

                if (xFine && yFine && zFine && (grid[cc.x + dx][cc.y + dy][cc.z + dz] & 2) == 0) {
                    if ((grid[cc.x + dx][cc.y + dy][cc.z + dz] & 1) == 0) {
                        q.push({cc.x + dx, cc.y + dy, cc.z + dz});
                        grid[cc.x + dx][cc.y + dy][cc.z + dz] ^= 2;
                    } else
                        ++counter;
                }
            }
        }

        return counter;
    }
};

int main() {
    fmt::print("AoC 2022/12/18\n");

    try {
        Lava l("../data/18-1.txt");
        fmt::print("Number of sides visible: {}\n", l.countSides());
        fmt::print("Number of outer sides visible: {}\n", l.countOuterSides());
    }
    catch (std::exception& e) {
        fmt::print("Exception caught: {}\n", e.what());
    }

    return 0;
}
