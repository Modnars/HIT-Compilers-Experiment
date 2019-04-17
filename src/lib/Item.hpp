#ifndef ITEM_HPP
#define ITEM_HPP

#include <iostream>
#include <string>

class Item {
public:
    Item(const std::string &left, const std::vector<std::string> &right) :
            left(left), right(right) { }

    bool could_reduce() const ;
    Item shift() const ;
    std::string next_sym() const ;
    std::string prev_sym() const; 

    const std::string left;
    const std::vector<std::string> right;
};

Item Item::shift() const {
    auto svec = right;
    for (size_t i = 0; i < svec.size(); ++i)
        if (svec[i] == "@") {
            std::swap(svec[i], svec[i+1]);
            break;
        }
    return Item(left, svec);
}

bool Item::could_reduce() const {
    return right.back() == "@";
}

std::string Item::next_sym() const {
    if (could_reduce()) {
        return "";
    }
    for (size_t i = 0; i < right.size(); ++i)
        if (right[i] == "@")
            return right[i+1];
    return "";
}

std::string Item::prev_sym() const {
    if (right[0] == "#")
        return "@";
    for (size_t i = 0; i < right.size(); ++i)
        if (right[i] == "@")
            return right[i-1];
    return "";
}

// std::string next_sym(const Item &it) {
//     if (it.could_reduce()) {
//         std::cout << "What ?!" << std::endl;
//         return "";
//     }
//     return std::string(1, it.right[it.right.find("@")+1]);
// }

bool operator==(const Item &a, const Item &b) {
    return (a.left == b.left) && (a.right == b.right);
}

std::ostream &operator<<(std::ostream &os, const Item &it) {
    os << it.left << " -> ";
    for (auto var : it.right)
        std::cout << var << " ";
    return os;
}

bool contains(const std::vector<Item> &itvec, const Item &it) {
    for (auto var : itvec) 
        if (var == it)
            return true;
    return false;
}

#endif /* ITEM_HPP */
