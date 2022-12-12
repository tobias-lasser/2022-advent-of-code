#include "ParserHelper.h"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <queue>

struct Node {
    int x, y;
    char elevation;
    Node* pred;
};

class Elevation {
    std::vector<std::vector<Node>> grid;
    std::pair<std::size_t, std::size_t> size;
    Node* source;
    Node* target;

public:
    explicit Elevation(std::string_view filename) {
        auto lines = ParserHelper::readLinesFromFile(filename);

        grid.reserve(lines.size());
        size.first = lines.size();
        int y{0};
        for (const auto& l : lines) {
            std::vector<Node> temp;
            temp.reserve(l.size());
            size.second = l.size();
            int x{0};
            for (auto c : l) {
                temp.push_back({x, y, c, nullptr});
                if (c == 'S') {
                    source = &temp.back();
                    source->elevation = 'a';
                }
                if (c == 'E') {
                    target = &temp.back();
                    target->elevation = 'z';
                }
                ++x;
            }
            grid.push_back(std::move(temp));
            ++y;
        }
    }

    void printGrid() const {
        for (const auto& v : grid) {
            for (const auto& n : v)
                fmt::print("{}", n.elevation);
            fmt::print("\n");
        }
        fmt::print("source = ({},{}); target = ({},{})\n", source->x, source->y, target->x, target->y);
    }

    std::vector<Node*> getValidNeighbors(Node* c) {
        std::vector<Node*> n;
        if (c->x-1 >= 0 && grid[c->y][c->x -1].elevation - 1 <= c->elevation)
            n.push_back(&grid[c->y][c->x -1]);
        if (c->x+1 < size.second && grid[c->y][c->x +1].elevation - 1 <= c->elevation)
            n.push_back(&grid[c->y][c->x +1]);
        if (c->y-1 >= 0 && grid[c->y -1][c->x].elevation - 1 <= c->elevation)
            n.push_back(&grid[c->y -1][c->x]);
        if (c->y+1 < size.first && grid[c->y +1][c->x].elevation - 1 <= c->elevation)
            n.push_back(&grid[c->y +1][c->x]);

        return n;
    }

    void bfs() {
        source->pred = source;
        std::queue<Node*> q;
        q.push(source);

        while (!q.empty()) {
            auto c = q.front();
            q.pop();

            auto ns = getValidNeighbors(c);
            for (auto n : ns) {
                if (n->pred == nullptr) {
                   q.push(n);
                   n->pred = c;
                }
            }
        }

        source->pred = nullptr;
    }

    int getShortestPathLength() {
        Node* c = target;
        int counter{0};
        while (c != nullptr) {
            c = c->pred;
            ++counter;
        }

        return counter-1;
    }

    void clearPred() {
        for (auto& rows : grid) {
            for (auto& n : rows) {
                n.pred = nullptr;
            }
        }
    }

    int getShortestStartPointLength() {
        std::vector<Node*> starts;
        // find all 'a'
        for (auto& rows : grid) {
            for (auto& n : rows) {
                if (n.elevation == 'a') {
                    starts.push_back(&n);
                }
            }
        }

        std::vector<int> paths;
        for (auto n : starts) {
            clearPred();
            source = n;
            bfs();
            auto len = getShortestPathLength();
            if (len > 0)
                paths.push_back(len);
        }

        std::sort(paths.begin(), paths.end());
        return paths.front();
    }
};

int main() {
    fmt::print("AoC 2022/12/12\n");

    try {
        Elevation e("../data/12-1.txt");
        e.bfs();
        fmt::print("Shortest path length: {}\n", e.getShortestPathLength());
        fmt::print("Shortest start length: {}\n", e.getShortestStartPointLength());
    }
    catch (std::exception& e) {
        fmt::print("Exception caught: {}\n", e.what());
    }

    return 0;
}