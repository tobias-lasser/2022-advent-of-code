#include "ParserHelper.h"
#include <fmt/core.h>
#include <variant>
#include <stack>

struct VList : std::variant<std::vector<VList>, int> {
private:
    using base = std::variant<std::vector<VList>, int>;

public:
    using base::base;
    VList(std::initializer_list<VList> vl) : base(vl) {}

    bool isVector() const { return std::holds_alternative<std::vector<VList>>(*this); }
    bool isInt() const { return std::holds_alternative<int>(*this); }

    auto* asVector() { return std::get_if<std::vector<VList>>(this); }
    auto* asInt() { return std::get_if<int>(this); }
};

struct Packet {
    VList v;

    explicit Packet(std::string_view line) {
        if (!line.empty())
            v = parseList(std::string(line));
    }

    VList parseList(std::string l) {
        std::stack<VList> stk;

        for (std::size_t i = 0; i < l.size(); ++i) {
            if (l[i] == '[') stk.push(VList{});
            else if (std::isdigit(l[i])) {
                auto start = i;
                while (i < l.size() && std::isdigit(l[i+1]))
                    ++i;
                int num = std::stoi(l.substr(start, i-start+1));
                VList temp{num};
                if (!stk.empty()) {
                    if (stk.top().isVector())
                        stk.top().asVector()->push_back(num);
                }
                else return temp;
            }
            else if (l[i] == ']') {
                auto temp = stk.top();
                stk.pop();
                if (!stk.empty()) {
                    if (stk.top().isVector())
                        stk.top().asVector()->push_back(temp);
                }
                else
                    return temp;
            }
        }
    }

    void print() { print(v); }

    void print(VList& p) {
        if (p.isVector()) {
            fmt::print("[");
            for (auto e : *p.asVector())
                print(e);
            fmt::print("],");
        }
        else
            fmt::print("{},", *p.asInt());
    }
};

bool operator==(Packet& p1, Packet& p2) {
    return p1.v == p2.v;
}

enum class order { correct, incorrect, undecided };

order correctOrder(VList& v1, VList& v2) {
    if (v1.isVector() && v2.isVector()) {
        auto it1 = v1.asVector()->begin();
        auto it2 = v2.asVector()->begin();
        while (it1 != v1.asVector()->end() && it2 != v2.asVector()->end()) {
            auto o = correctOrder(*it1, *it2);
            if (o != order::undecided)
                return o;
            ++it1;
            ++it2;
        }
        if (it1 == v1.asVector()->end() && it2 != v2.asVector()->end())
            return order::correct;
        else if (it1 != v1.asVector()->end())
            return order::incorrect;
        else
            return order::undecided;
    }
    else if (v1.isInt() && v2.isInt()) {
        if (*v1.asInt() < *v2.asInt())
            return order::correct;
        else if (*v1.asInt() > *v2.asInt())
            return order::incorrect;
        else
            return order::undecided;
    }
    else { // mixed case, promote one to list
        if (v1.isInt()) {
            VList temp{};
            temp.asVector()->emplace_back(*v1.asInt());
            v1 = temp;
        }
        else if (v2.isInt()) {
            VList temp{};
            temp.asVector()->emplace_back(*v2.asInt());
            v2 = temp;
        }
        return correctOrder(v1, v2);
    }
}

bool correctOrder(Packet& p1, Packet& p2) {
    order res = correctOrder(p1.v, p2.v);
    return (res == order::correct);
}


class Distress {
    std::vector<std::pair<Packet, Packet>> packetPairs;
    std::vector<Packet> packets;

public:
    explicit Distress(std::string_view filename) {
        auto lines = ParserHelper::readLinesFromFile(filename);

        for (std::size_t i = 0; i < lines.size(); i += 3)
            packetPairs.emplace_back(lines[i], lines[i+1]);
    }

    int checkOrder() {
        int counter{1};
        int sum{0};
        for (auto& [p1, p2] : packetPairs) {
            if (correctOrder(p1, p2))
                sum += counter;
            ++counter;
        }
        return sum;
    }

    void setupPart2() {
        packetPairs.emplace_back("[[2]]", "[[6]]");
        packets.reserve(packetPairs.size() * 2);
        for (auto& [p1, p2] : packetPairs) {
            packets.push_back(p1);
            packets.push_back(p2);
        }

        std::sort(packets.begin(), packets.end(),
                  [](auto a, auto b) { return correctOrder(a, b); });
    }

    int getDividers() {
        int result{1};
        Packet d1{"[[2]]"};
        Packet d2{"[[6]]"};
        for (int i = 0; i < packets.size(); ++i) {
            if (packets[i] == d1) {
                result *= i+1;
            }
            if (packets[i] == d2) {
                result *= i+1;
            }
        }
        return result;
    }
};

int main() {
    fmt::print("AoC 2022/12/13\n");

    try {
        Distress d("../data/13-1.txt");
        fmt::print("correct order pairs sum: {}\n", d.checkOrder());
        d.setupPart2();
        fmt::print("divider indices product: {}", d.getDividers());
    }
    catch (std::exception& e) {
        fmt::print("Exception caught: {}\n", e.what());
    }

    return 0;
}