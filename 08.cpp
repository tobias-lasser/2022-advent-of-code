#include "ParserHelper.h"
#include <fmt/core.h>

class Trees {
    std::vector<std::vector<int>> grid;

public:
    explicit Trees(std::string_view filename) {
        auto lines = ParserHelper::readLinesFromFile(filename);

        grid.reserve(lines.size());
        for (const auto& l : lines) {
            std::vector<int> heights;
            for (auto c : l)
                heights.push_back(c - '0');
            grid.push_back(heights);
        }
    }

    bool visible(int j, int i) {
        // trees on edge
        if (j == 0 || j == grid.size() - 1)
            return true;
        if (i == 0 || i == grid[j].size() - 1)
            return true;

        // inner trees
        int tree = grid[j][i];

        // check from top
        bool visibleTop{true};
        for (int jj = 0; jj < j; ++jj)
            if (grid[jj][i] >= tree) visibleTop = false;

        // check from bottom
        bool visibleBottom{true};
        for (int jj = grid.size()-1; jj > j; --jj)
            if (grid[jj][i] >= tree) visibleBottom = false;

        // check from left
        bool visibleLeft{true};
        for (int ii = 0; ii < i; ++ii)
            if (grid[j][ii] >= tree) visibleLeft = false;

        // check from right
        bool visibleRight{true};
        for (int ii = grid[j].size()-1; ii > i; --ii)
            if (grid[j][ii] >= tree) visibleRight = false;

        return visibleTop || visibleBottom || visibleLeft || visibleRight;
    }

    int countVisibleTrees() {
        int result{0};

        for (int j = 0; j < grid.size(); ++j) {
            for (int i = 0; i < grid[j].size(); ++i) {
                if (visible(j, i))
                    ++result;
            }
        }

        return result;
    }

    int score(int j, int i) {
        int scoreLeft{0}, scoreRight{0}, scoreUp{0}, scoreDown{0};
        int tree = grid[j][i];

        for (int jj = j-1; jj >= 0; --jj) {
            ++scoreUp;
            if (grid[jj][i] >= tree) break;
        }

        for (int jj = j+1; jj < grid.size(); ++jj) {
            ++scoreDown;
            if (grid[jj][i] >= tree) break;
        }

        for (int ii = i-1; ii >= 0; --ii) {
            ++scoreLeft;
            if (grid[j][ii] >= tree) break;
        }

        for (int ii = i+1; ii < grid[j].size(); ++ii) {
            ++scoreRight;
            if (grid[j][ii] >= tree) break;
        }

        return scoreLeft * scoreRight * scoreUp * scoreDown;
    }

    int getHighestScenicScore() {
        int maxScore{-1};

        for (int j = 0; j < grid.size(); ++j) {
            for (int i = 0; i < grid[j].size(); ++i) {
                if (int temp = score(j, i); temp > maxScore)
                    maxScore = temp;
            }
        }

        return maxScore;
    }
};

int main() {
    fmt::print("AoC 2022/12/08\n");

    try {
        Trees t("../data/08-1.txt");
        fmt::print("visible trees: {}\n", t.countVisibleTrees());
        fmt::print("highest scenic score: {}\n", t.getHighestScenicScore());
    }
    catch (std::exception& e) {
        fmt::print("Exception caught: {}\n", e.what());
    }

    return 0;
}