#include "ParserHelper.h"
#include <fmt/core.h>
#include <unordered_map>
#include <string_view>

using namespace std::string_literals;

struct Tree {
    std::string id;
    char op{' '};
    int64_t number{0};
    std::string idLeft, idRight;

    Tree() {}
    Tree(std::string i, int64_t num) : id{std::move(i)}, number{num} {}
    Tree(std::string i, char o, std::string l, std::string r)
        : id{std::move(i)}, op{o}, idLeft{std::move(l)}, idRight{std::move(r)} {}
};

class Monkeys {
    Tree* root{nullptr};
    std::unordered_map<std::string, Tree> nodes;
    static const std::string human;

public:
    explicit Monkeys(std::string_view filename) {
        auto lines = ParserHelper::readLinesFromFile(filename);

        for (const auto& l : lines) {
            std::string id = l.substr(0, 4);
            if (isdigit(l[6])) {
                int number = std::stoi(l.substr(6, std::string::npos));
                Tree t{id, number};
                nodes[id] = t;
            }
            else {
                std::string left = l.substr(6, 4);
                std::string right = l.substr(13, 4);
                char op = l[11];
                Tree t{id, op, left, right};
                nodes[id] = t;
                if (id == "root")
                    root = &nodes[id];
            }
        }
    }

    int64_t eval(Tree& node) {
        if (node.op == ' ') // leaf
            return node.number;

        int64_t left = eval(nodes[node.idLeft]);
        int64_t right = eval(nodes[node.idRight]);

        switch (node.op) {
            case '+': return left + right;
            case '-': return left - right;
            case '*': return left * right;
            case '/': return left / right;
            default: return 0;
        }
    }

    int64_t traverseTree() {
        return eval(*root);
    }

    bool isHumanInTree(Tree& node) {
        if (node.id == human) return true;
        if (node.op == ' ') return false;

        return isHumanInTree(nodes[node.idLeft])
                || isHumanInTree(nodes[node.idRight]);
    }

    int64_t computeHuman(Tree& node, int64_t value) {
//        fmt::print("computeHuman in id={}, value={}, ", node.id, value);
        if (node.id == human) return value;
        if (node.op == ' ') throw std::runtime_error("problem in tree");

        auto humanInLeft = isHumanInTree(nodes[node.idLeft]);
        auto idHuman = (humanInLeft) ? node.idLeft : node.idRight;
//        fmt::print("humanInLeft={}, idHuman={}\n", humanInLeft, idHuman);
        auto idMonkeys = (humanInLeft) ? node.idRight : node.idLeft;
        int64_t otherValue = eval(nodes[idMonkeys]);

//        fmt::print("human subtree: {}; monkeys: {} with value {}\n", idHuman, idMonkeys, otherValue);

        switch (node.op) {
            case '+':
                return computeHuman(nodes[idHuman], value - otherValue);
            case '-':
                return computeHuman(nodes[idHuman],
                                    (humanInLeft) ? value + otherValue : otherValue - value);
            case '*':
                return computeHuman(nodes[idHuman], value / otherValue);
            case '/':
                return computeHuman(nodes[idHuman],
                                    (humanInLeft) ? value * otherValue : otherValue / value);
            default: return 0;
        }
    }

    int64_t getHumanNumber() {
        auto humanInLeft = isHumanInTree(*root);
        Tree& rootHuman = (humanInLeft) ? nodes[root->idLeft] : nodes[root->idRight];
        Tree& rootMonkeys = (humanInLeft) ? nodes[root->idRight] : nodes[root->idLeft];

        auto monkeyValue = eval(rootMonkeys);
        return computeHuman(rootHuman, monkeyValue);
    }
};

const std::string Monkeys::human{"humn"s};

int main() {
    fmt::print("AoC 2022/12/21\n");

    try {
        Monkeys m("../data/21-1.txt");
        fmt::print("root monkey says {}\n", m.traverseTree());
        fmt::print("human should have {}\n", m.getHumanNumber());
    }
    catch (std::exception& e) {
        fmt::print("Exception caught: {}\n", e.what());
    }

    return 0;
}