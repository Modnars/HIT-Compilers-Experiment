#include "lib/Item.hpp"

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
    // Warning: here add the '$' to mark the item is a null-production item.
    return (right.back() == "@" || right.back() == "$");
}

bool Item::reduce_from(const Production &prod) const {
    if (!could_reduce())
        return false;
    if (left != prod.get_left())
        return false;
    // For the null-production item.
    if (right.back() == "$" && prod.get_right()[0] == "$") 
        return true;
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

std::ostream &operator<<(std::ostream &os, const Item &it) {
    os << it.left << " -> ";
    for (auto var : it.right)
        os << var << " ";
    return os;
}

std::ostream &operator<<(std::ostream &os, const ReduceItem &rdit) {
    os << std::left << std::setw(15) << rdit.state << std::setw(15) << rdit.symbol
       << "R" << rdit.prod_id;
    return os;
}

std::ostream &operator<<(std::ostream &os, const GotoItem &gtit) {
    os << std::left << std::setw(15) << gtit.start << std::setw(15) << gtit.symbol
       << std::setw(15) << gtit.end;
    return os;
}

bool operator==(const Item &a, const Item &b) {
    return (a.left == b.left) && (a.right == b.right);
}

bool operator==(const GotoItem &it1, const GotoItem &it2) {
    return (it1.start == it2.start) && (it1.end == it2.end) && (it1.symbol == it2.symbol);
}
