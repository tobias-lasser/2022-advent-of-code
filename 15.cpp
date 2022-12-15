#include "ParserHelper.h"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <cmath>

struct Position {
    int x, y;
    int distance(const Position& p) const {
        return std::abs(x-p.x) + std::abs(y-p.y);
    }

};

bool operator==(const Position& p1, const Position& p2) {
    return (p1.x == p2.x && p1.y == p2.y);
}

using Interval = std::pair<int, int>;

struct Sensor {
    Position location;
    Position beacon;
};

class Beacons {
    std::vector<Sensor> sensors;

public:
    explicit Beacons(std::string_view filename) {
        auto lines = ParserHelper::readLinesFromFile(filename);

        for (const auto& l : lines) {
            auto strs = ParserHelper::parseStrsFromString(l, '=');

            Sensor s;
            s.location.x = std::stoi(strs[1]);
            s.location.y = std::stoi(strs[2]);
            s.beacon.x = std::stoi(strs[3]);
            s.beacon.y = std::stoi(strs[4]);

            sensors.push_back(s);
        }
    }

    std::vector<int> getDistances() const {
        std::vector<int> distances;

        for (const auto& s : sensors) {
            distances.push_back(s.location.distance(s.beacon));
        }

        return distances;
    }

    int getMaxDistance() const {
        auto distances = getDistances();
        std::sort(distances.begin(), distances.end());
        return distances.back();
    }

    std::pair<int, int> getMinMaxX() const {
        int minX{std::numeric_limits<int>::max()};
        int maxX{std::numeric_limits<int>::min()};
        for (const auto& s : sensors) {
            if (s.location.x < minX) minX = s.location.x;
            if (s.beacon.x < minX) minX = s.beacon.x;
            if (s.location.x > maxX) maxX = s.location.x;
            if (s.beacon.x > maxX) maxX = s.beacon.x;
        }

        return std::make_pair(minX, maxX);
    }

    int getExcludedPoints(int row) {
        auto [minX, maxX] = getMinMaxX();
        auto maxDist = getMaxDistance();
        minX -= maxDist + 1;
        maxX += maxDist + 1;

        int excluded{0};
        for (int i = minX; i <= maxX; ++i) {
            Position current{i, row};
            bool excludeCurrent{false};
            for (const auto&  s : sensors) {
                if (current == s.beacon)
                    break;
                int beaconDist = s.location.distance(s.beacon);
                if (current.distance(s.location) <= beaconDist) {
                    excludeCurrent = true;
                    break;
                }
            }
            if (excludeCurrent) ++excluded;
        }

        return excluded;
    }


    std::vector<Interval> getIntervals(int y, int maxCoord) const {
        std::vector<Interval> intervals;
        intervals.reserve(2*sensors.size());
        for (const auto& s : sensors) {
            if (s.beacon.y == y && s.beacon.x >= 0 && s.beacon.x <= maxCoord)
                intervals.emplace_back(s.beacon.x, s.beacon.x);

            int dist = s.location.distance(s.beacon) - std::abs(s.location.y - y);
            if (dist <= 0)
                continue;
            int x1 = dist + s.location.x;
            int x2 = s.location.x - dist;
            if (x1 > x2) std::swap(x1, x2);
            intervals.emplace_back(std::max(0, x1), std::min(x2, maxCoord));
        }
        intervals.shrink_to_fit();
        std::sort(intervals.begin(), intervals.end(),
                  [](auto a, auto b) { return a.first < b.first; });

        return intervals;
    }

    std::vector<Interval> getMergedIntervals(std::vector<Interval>& intervals) {
        std::vector<std::pair<int, int>> mergedIntervals;
        mergedIntervals.push_back(intervals.front());
        for (int i = 1; i < intervals.size(); ++i) {
            if (mergedIntervals.back().second >= intervals[i].first)
                mergedIntervals.back().second = std::max(mergedIntervals.back().second, intervals[i].second);
            else if (mergedIntervals.back().second + 1 == intervals[i].first)
                mergedIntervals.back().second = intervals[i].second;
            else
                mergedIntervals.push_back(intervals[i]);
        }

        return mergedIntervals;
    }

    long long getBeaconClever(int maxCoord) {
        for (int y = 0; y <= maxCoord; ++y) {
            auto intervals = getIntervals(y, maxCoord);
            auto mergedIntervals = getMergedIntervals(intervals);

            if (mergedIntervals.size() > 1) {
                long long x = mergedIntervals[0].second + 1;
                fmt::print("found {},{}\n", x, y);
                return x * 4000000 + y;

            }
        }

        return 0;
    }
};

int main() {
    fmt::print("AoC 2022/12/15\n");

    try {
        Beacons b("../data/15-1.txt");
        int row = 2000000;
        int maxCoord = 4000000;
        fmt::print("excluded points in row {} = {}\n", row, b.getExcludedPoints(row));
        fmt::print("beacon found, frequency = {}\n", b.getBeaconClever(maxCoord));
    }
    catch (std::exception& e) {
        fmt::print("Exception caught: {}\n", e.what());
    }

    return 0;
}