#ifndef ITEM_HPP
#define ITEM_HPP

#include <iostream>
#include <string>

#include "Production.hpp"

class Item;
extern std::ostream &operator<<(std::ostream &os, const Item &it);
extern bool operator<(const Item &a, const Item &b);
extern bool operator==(const Item &a, const Item &b);

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

#endif /* ITEM_HPP */
