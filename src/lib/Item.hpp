#ifndef ITEM_HPP
#define ITEM_HPP

#include <iostream>
#include <string>

#include "Production.hpp"

class Item;
class GotoItem;
class ReduceItem;
extern std::ostream &operator<<(std::ostream &os, const Item &it);
extern std::ostream &operator<<(std::ostream &os, const ReduceItem &rdit);
extern std::ostream &operator<<(std::ostream &os, const GotoItem &gtit);
extern bool operator<(const Item &a, const Item &b);
extern bool operator==(const Item &a, const Item &b);
extern bool operator==(const GotoItem &it1, const GotoItem &it2);

class Item {
public:
    Item(const std::string &left, const std::vector<std::string> &rights) :
        left(left), rights(rights), search("") { }
    Item(const std::string &left, const std::vector<std::string> &rights, 
            const std::string &search) :
        left(left), rights(rights), search(search) { }

    bool could_reduce() const ;
    bool reduce_from(const Production&) const ;
    Item shift() const ;
    std::string next_sym() const ;
    int pos() const ;

    const std::string left;
    const std::vector<std::string> rights;
    const std::string search;
};

class ReduceItem {
public:
    ReduceItem(size_t st, const std::string &sym, size_t pid) :
            state(st), symbol(sym), prod_id(pid) { }

    const size_t state;
    const std::string symbol;
    const size_t prod_id;
};

class GotoItem {
public:
    GotoItem(size_t start, size_t end, const std::string &sym) :
            start(start), end(end), symbol(sym) { }

    const size_t start;
    const size_t end;
    const std::string symbol;
};

#endif /* ITEM_HPP */
