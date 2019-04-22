#include "lib/Item.hpp"

Item Item::shift() const {
    auto svec = rights;
    for (size_t i = 0; i < svec.size(); ++i)
        if (svec[i] == "@") {
            std::swap(svec[i], svec[i+1]);
            break;
        }
    return Item(left, svec, search);
}

bool Item::could_reduce() const {
    // Warning: here add the '$' to mark the item is a null-production item.
    return (rights.back() == "@" || rights.back() == "$");
}

bool Item::reduce_from(const Production &prod) const {
    if (!could_reduce())
        return false;
    if (left != prod.left)
        return false;
    // For the null-production item.
    if (rights.back() == "$" && prod.rights[0] == "$") 
        return true;
    for (size_t i = 0; i < prod.rights.size(); ++i)
        if (rights[i] != prod.rights[i])
            return false;
    return true;
}

std::string Item::next_sym() const {
    if (could_reduce()) {
        return "";
    }
    for (size_t i = 0; i < rights.size(); ++i)
        if (rights[i] == "@")
            return rights[i+1];
    return "";
}

int Item::pos() const {
    for (int i = 0; i < rights.size(); ++i)
        if (rights[i] == "@")
            return i;
    return -1;
}

std::ostream &operator<<(std::ostream &os, const Item &it) {
    os << it.left << " -> ";
    for (auto var : it.rights)
        os << var << " ";
    if (it.search != "")
        os << "Search: " <<  it.search;
    return os;
}

bool operator<(const Item &a, const Item &b) {
    if (a.left < b.left) 
        return true;
    else if (a.left == b.left && a.rights < b.rights) 
        return true;
    else if (a.left == b.left && a.rights == b.rights && a.search < b.search) 
        return true;
    return false;
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
    return (a.left == b.left) && (a.rights == b.rights) && (a.search == b.search);
}

bool operator==(const GotoItem &it1, const GotoItem &it2) {
    return (it1.start == it2.start) && (it1.end == it2.end) && (it1.symbol == it2.symbol);
}
