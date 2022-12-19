#include "ParserHelper.h"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <regex>
#include <array>

struct Blueprint {
    int id;
    int ORcost;
    int CRcost;
    int ORcostOre, ORcostClay;
    int GRcostOre, GRcostObsidian;

    void print() const {
        fmt::print("Blueprint {}: ORcost {} ore; CRcost {} ore; ORcost {} ore, {} clay; GRcost {} ore, {} obsidian.\n",
                   id, ORcost, CRcost, ORcostOre, ORcostClay, GRcostOre, GRcostObsidian);
    }
};

enum rbt { Ore, Clay, Obsidian, Geode };

int ceilDiv(int i, int j) { return (i + j - 1) / j; }

class Simulator {
    std::array<std::array<int, 4>, 4> costs;

public:
    explicit Simulator(const Blueprint& bp) : costs{} {
        costs[Ore]      = {bp.ORcost, 0, 0, 0};
        costs[Clay]     = {bp.CRcost, 0, 0, 0};
        costs[Obsidian] = {bp.ORcostOre, bp.ORcostClay, 0, 0};
        costs[Geode]    = {bp.GRcostOre, 0, bp.GRcostObsidian, 0};
    }

    int doSim(int minutes) {
        return simulate(minutes, {1, 0, 0, 0}, {0, 0, 0, 0});
    }

private:
    bool shouldBuild(rbt type, const std::array<int, 4>& robots) {
        switch (type) {
            case Ore:
                return (robots[Ore] < costs[Clay][Ore] || robots[Ore] < costs[Obsidian][Ore]
                        || robots[Ore] < costs[Geode][Ore]);
            case Clay:
                return (robots[Clay] < costs[Obsidian][Clay]);
            case Obsidian:
                return (robots[Clay] && robots[Obsidian] < costs[Geode][Obsidian]);
            case Geode:
                return (robots[Obsidian]);
        }
    }

    int timeToBuild(rbt type, const std::array<int, 4>& robots, const std::array<int ,4>& materials) {
        switch (type) {
            case Ore:
                if (materials[Ore] > costs[Ore][Ore]) return 1;
                else
                    return 1 + ceilDiv(costs[Ore][Ore] - materials[Ore], robots[Ore]);
            case Clay:
                if (materials[Ore] > costs[Clay][Ore]) return 1;
                else
                    return 1 + ceilDiv(costs[Clay][Ore] - materials[Ore], robots[Ore]);
            case Obsidian:
                if (materials[Ore] > costs[Obsidian][Ore] && materials[Clay] > costs[Obsidian][Clay])
                    return 1;
                else
                    return 1 + std::max(ceilDiv(costs[Obsidian][Ore] - materials[Ore], robots[Ore]),
                                        ceilDiv(costs[Obsidian][Clay] - materials[Clay], robots[Clay]));
            case Geode:
                if (materials[Ore] > costs[Geode][Ore] && materials[Obsidian] > costs[Geode][Obsidian])
                    return 1;
                else
                    return 1 + std::max(ceilDiv(costs[Geode][Ore] - materials[Ore], robots[Ore]),
                                        ceilDiv(costs[Geode][Obsidian] - materials[Obsidian], robots[Obsidian]));
        }
    }

    std::array<int, 4> build(rbt type, std::array<int, 4> robots) {
        robots[type]++;
        return robots;
    }

    std::array<int, 4> buildAndProduce(rbt type, int time, const std::array<int, 4>& robots, std::array<int, 4> materials) {
        for (int i = 0; i < 4; ++i) {
            materials[i] -= costs[type][i];
            materials[i] += robots[i] * time;
        }
        return materials;
    }

    int simulate(int minutesLeft, std::array<int, 4> robots, std::array<int, 4> materials) {
//        fmt::print("{}min left. robots: {}; materials: {};\n", minutesLeft, robots, materials);
        int best = materials[Geode] + minutesLeft * robots[Geode];

        std::array<rbt, 4> types{Ore, Clay, Obsidian, Geode};

        for (auto t : types) {
            if (shouldBuild(t, robots)) {
                auto t2build = timeToBuild(t, robots, materials);
                if (t2build < minutesLeft)
                    best = std::max(best,
                                    simulate(minutesLeft - t2build, build(t, robots),
                                             buildAndProduce(t, t2build, robots, materials)));
            }
        }

        return best;
    }
};

class Geodes {
    std::vector<Blueprint> blueprints;

public:
    explicit Geodes(std::string_view filename) {
        auto lines = ParserHelper::readLinesFromFile(filename);

        blueprints.reserve(lines.size());
        for (const auto& l : lines) {
            std::regex rg(R"(Blueprint ([0-9]+): Each ore robot costs ([0-9]+) ore. Each clay robot costs ([0-9]+) ore. Each obsidian robot costs ([0-9]+) ore and ([0-9]+) clay. Each geode robot costs ([0-9]+) ore and ([0-9]+) obsidian.)");
            std::smatch sm;
            std::regex_match(l, sm, rg);

            blueprints.push_back({std::stoi(sm[1]), std::stoi(sm[2]), std::stoi(sm[3]),
                                     std::stoi(sm[4]), std::stoi(sm[5]), std::stoi(sm[6]),
                                     std::stoi(sm[7])});
        }

    }

    int getBlueprintScore() const {
        int score{0};
        for (const auto& bp : blueprints) {
            Simulator sim(bp);
            auto best = sim.doSim(24);
            score += bp.id * best;
        }
        return score;
    }

    int getGeodesScore() const {
        int score{1};
        for (int i = 0; i < 3; ++i) {
            Simulator sim(blueprints[i]);
            auto best = sim.doSim(32);
            score *= best;
        }

        return score;
    }
};

int main() {
    fmt::print("AoC 2022/12/19\n");

    try {
        Geodes g("../data/19-1.txt");
        fmt::print("Blueprint score: {}\n", g.getBlueprintScore());
        fmt::print("Geode score: {}\n", g.getGeodesScore());
    }
    catch (std::exception& e) {
        fmt::print("Exception caught: {}\n", e.what());
    }

    return 0;
}