#include <string_view>
#include <unordered_map>
#include <fmt/core.h>
#include "ParserHelper.h"

enum class RPS { Rock, Paper, Scissors };

struct Round {
    std::unordered_map<char, RPS> mapping {
            {'A', RPS::Rock}, {'B', RPS::Paper}, {'C', RPS::Scissors},
            {'X', RPS::Rock}, {'Y', RPS::Paper}, {'Z', RPS::Scissors}
    };

    RPS elf, me;

    static RPS winMoveAgainst(RPS move) {
        if (move == RPS::Rock)      return RPS::Paper;
        if (move == RPS::Paper)     return RPS::Scissors;
        if (move == RPS::Scissors)  return RPS::Rock;
    }

    static RPS loseMoveAgainst(RPS move) {
        if (move == RPS::Rock)      return RPS::Scissors;
        if (move == RPS::Paper)     return RPS::Rock;
        if (move == RPS::Scissors)  return RPS::Paper;
    }

    Round(char theirs, char mine, bool round2 = false) {
        elf = mapping[theirs];
        me = mapping[mine];

        if (round2) {
            switch (mine) {
                case 'X': // need to lose
                    me = loseMoveAgainst(elf);
                    break;
                case 'Y': // need to draw
                    me = elf;
                    break;
                case 'Z': // need to win
                    me = winMoveAgainst(elf);
                    break;
                default:
                    break;
            }
        }
    }

    int getPoints() const {
        int score{0};
        switch (me) {
            case RPS::Rock:     score += 1; break;
            case RPS::Paper:    score += 2; break;
            case RPS::Scissors: score += 3; break;
            default:            break;
        }

        // score wins
        if (me == winMoveAgainst(elf))
            score += 6;

        // score draws
        if (elf == me)
            score += 3;

        return score;
    }
};

class RockPaperScissors {
    std::vector<Round> rounds;
    std::vector<Round> rounds2;

public:
    explicit RockPaperScissors(std::string_view filename) {
        auto lines = ParserHelper::readLinesFromFile(filename);

        for (const auto& l : lines) {
            if (l.size() < 3) continue;
            rounds.emplace_back(l.at(0), l.at(2));
            rounds2.emplace_back(l.at(0), l.at(2), true);
        }
    }

    int getTotalScore(bool round2 = false) {
        int totalScore{0};
        for (const auto& r : (round2) ? rounds2 : rounds)
            totalScore += r.getPoints();

        return totalScore;
    }
};

int main() {
    fmt::print("AOC 2022/12/02\n");

    try {
        RockPaperScissors rps("../data/02-1.txt");
        fmt::print("Total score: {}\n", rps.getTotalScore());
        fmt::print("Strategy score: {}\n", rps.getTotalScore(true));
    }
    catch (std::exception& e) {
        fmt::print("Exception caught {}\n", e.what());
    }

    return 0;
}