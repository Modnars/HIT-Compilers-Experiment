#ifndef BASICFUNC_HPP
#define BASICFUNC_HPP

#include <queue>
#include <stack>

#include "Base.hpp"

//将分割后的子字符串存储在vector中
extern std::vector<std::string> split(const std::string &, const std::string &);
extern std::string &trim(std::string &);

template <typename T> 
extern bool contains(const std::set<T> &sset, const T &val) {
    if (sset.empty())
        return false;
    if (sset.find(val) == sset.end())
        return false;
    return true;
}

template <typename T>
extern bool contains(const std::vector<T> &svec, const T &val) {
    if (svec.empty())
        return false;
    for (auto var : svec)
        if (var == val)
            return true;
    return false;
}

template <typename T>
extern void print_que(std::queue<T> que, std::ostream &os = std::cout) {
    while (!que.empty()) {
        os << que.front() << " ";
        que.pop();
    }
    os << std::endl;
}

template <typename T>
extern void print_stk(std::stack<T> stk, std::ostream &os = std::cout) {
    std::vector<T> tmpVec;
    while (!stk.empty()) {
        tmpVec.push_back(stk.top());
        stk.pop();
    }
    for (auto iter = tmpVec.crbegin(); iter != tmpVec.crend(); ++iter)
        os << *iter << " ";
}

template <typename T>
extern void print_vec(const std::vector<T> &vec, std::ostream &os = std::cout) {
    for (auto var : vec) 
        os << var << " ";
    os << std::endl;
}

template <typename T>
extern void print_vec1(const std::vector<std::shared_ptr<T>> &vec = std::cout) {
    for (auto var : vec)
        std::cout << *var << " ";
    std::cout << std::endl;
}

#endif /* BASICFUNC_HPP */
