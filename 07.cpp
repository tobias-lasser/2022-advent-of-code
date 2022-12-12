#include "ParserHelper.h"
#include <unordered_map>
#include <fmt/ranges.h>

enum class Type { Dir, File };

struct Node {
    std::string name;
    int size;
    Type type;
    std::unordered_map<std::string, Node> children{};
    Node* parent{nullptr};

    Node(std::string n, Node* p) : name{std::move(n)}, size{0}, type{Type::Dir}, parent{p} {}

    Node(std::string n, int s) : name{std::move(n)}, size{s}, type{Type::File} {}
};

class Cleanup {
    Node root;
    std::vector<Node*> dirVec{};

public:
    explicit Cleanup(std::string_view filename) : root("/", nullptr) {
        auto lines = ParserHelper::readLinesFromFile(filename);
        parseCommands(lines);
        dirVec.emplace_back(&root);
    }

    void parseCommands(const std::vector<std::string>& lines) {
        Node* currentNode{&root};
        for (const auto& l : lines) {
            if (l.empty()) continue;

            if (l[0] == '$') { // command
                if (l == "$ cd /") continue;
                if (l == "$ ls") continue;

                auto strs = ParserHelper::parseStrsFromString(l, ' ');
                if (strs.size() != 3) throw std::runtime_error("unknown command");

                if (strs[1] == "cd") { // handle cd
                    if (strs[2] == "..") {
                        if (currentNode->parent == nullptr)
                            throw std::runtime_error("bad cd .. command");
                        else
                            currentNode = currentNode->parent;
                    }
                    else
                        currentNode = &currentNode->children.at(strs[2]);
                }
            }
            else { // directory listing
                auto strs = ParserHelper::parseStrsFromString(l, ' ');
                if (strs.size() != 2) throw std::runtime_error("malformed entry");
                if (strs[0] == "dir") {
                    currentNode->children.emplace(std::make_pair(strs[1], Node{strs[1], currentNode}));
                    dirVec.emplace_back(&currentNode->children.at(strs[1]));
                }
                else {
                    int size = std::stoi(strs[0]);
                    currentNode->children.emplace(std::make_pair(strs[1], Node{strs[1], size}));
                    currentNode->size += size;

                    // update sizes upwards
                    Node* temp = currentNode->parent;
                    while (temp != nullptr) {
                        temp->size += size;
                        temp = temp->parent;
                    }
                }
            }
        }
    }

    int sumDirSizesBelow100k() {
        int result{0};
        for (const auto& p : dirVec) {
            if (p->size <= 100000) result += p->size;
        }

        return result;
    }

    int findSmallestDirToDelete() {
        const int diskSpace{70000000};
        const int minUnusedSpace{30000000};

        int missingSpace = minUnusedSpace - (diskSpace - root.size);
        std::vector<int> bigEnoughDirs{};
        for (const auto& n : dirVec)
            if (n->size >= missingSpace)
                bigEnoughDirs.push_back(n->size);
        std::sort(bigEnoughDirs.begin(), bigEnoughDirs.end());

        return bigEnoughDirs[0];
    }
};

int main() {
    fmt::print("AoC 2022/12/07\n");

    try {
        Cleanup c("../data/07-1.txt");
        fmt::print("sum dir sizes below 100k: {}\n", c.sumDirSizesBelow100k());
        fmt::print("smallest dir to delete: {}\n", c.findSmallestDirToDelete());
    }
    catch (std::exception& e) {
        fmt::print("Exception caught: {}\n", e.what());
    }

    return 0;
}
