#ifndef ITEM_HPP
#define ITEM_HPP

#include <iostream>
#include <string>

#include "Production.hpp"

class Item {
public:
    Item(const std::string &left, const std::vector<std::string> &right) :
            left(left), right(right) { }

    bool could_reduce() const ;
    bool reduce_from(const Production&) const ;
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

bool Item::reduce_from(const Production &prod) const {
    if (!could_reduce())
        return false;
    if (left != prod.get_left())
        return false;
    for (size_t i = 0; i < prod.get_right().size(); ++i)
        if (right[i] != prod.get_right()[i])
            return false;
    return true;
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

bool operator==(const Item &a, const Item &b) {
    return (a.left == b.left) && (a.right == b.right);
}

std::ostream &operator<<(std::ostream &os, const Item &it) {
    os << it.left << " -> ";
    for (auto var : it.right)
        std::cout << var << " ";
    return os;
}

class ReduceItem {
public:
    ReduceItem(size_t st, const std::string &sym, size_t pid) :
            state(st), symbol(sym), prod_id(pid) { }

    const size_t state;
    const std::string symbol;
    const size_t prod_id;
};

std::ostream &operator<<(std::ostream &os, const ReduceItem &rdit) {
    os << std::left << std::setw(15) << rdit.state << std::setw(15) << rdit.symbol
       << "R" << rdit.prod_id;
    return os;
}

class GotoItem {
public:
    GotoItem(size_t start, size_t end, const std::string &sym) :
            start(start), end(end), symbol(sym) { }

    const size_t start;
    const size_t end;
    const std::string symbol;
};

std::ostream &operator<<(std::ostream &os, const GotoItem &gtit) {
    os << std::left << std::setw(15) << gtit.start << std::setw(15) << gtit.symbol
       << std::setw(15) << gtit.end;
    return os;
}

bool operator==(const GotoItem &it1, const GotoItem &it2) {
    return (it1.start == it2.start) && (it1.end == it2.end) && (it1.symbol == it2.symbol);
}

#endif /* ITEM_HPP */
