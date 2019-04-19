#ifndef BASICFUNC_HPP
#define BASICFUNC_HPP

#include <queue>
#include <stack>

#include "Base.hpp"

//将分割后的子字符串存储在vector中
std::vector<std::string> split(const std::string &str, const std::string &delim) { 
    std::vector<std::string> res;
    if("" == str) return res;
    std::string strs = str + delim; // 扩展字符串以方便检索最后一个分隔出的字符串
    size_t pos;
    size_t size = strs.size();
    for (int i = 0; i < size; ++i) {
        pos = strs.find(delim, i); // pos为分隔符第一次出现的位置，从i到pos之前的字符串是分隔出来的字符串
        if(pos < size) { //如果查找到，如果没有查找到分隔符，pos为string::npos
            std::string s = strs.substr(i, pos - i); // *****从i开始长度为pos-i的子字符串
            res.push_back(s); //两个连续空格之间切割出的字符串为空字符串，这里没有判断s是否为空，所以最后的结果中有空字符的输出，
            i = pos + delim.size() - 1;
        }
    }
    return res;	
}

std::string &trim(std::string &s) {
    if (s.empty()) {
        return s;
    }
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

template <typename T> 
bool contains(const std::set<T> &sset, const T &val) {
    if (sset.empty())
        return false;
    if (sset.find(val) == sset.end())
        return false;
    return true;
}

template <typename T>
bool contains(const std::vector<T> &svec, const T &val) {
    if (svec.empty())
        return false;
    for (auto var : svec)
        if (var == val)
            return true;
    return false;
}

template <typename T>
void print_que(std::queue<T> que, std::ostream &os = std::cout) {
    while (!que.empty()) {
        os << que.front() << std::endl;
        que.pop();
    }
    os << std::endl;
}

template <typename T>
void print_stk(std::stack<T> stk, std::ostream &os = std::cout) {
    std::vector<T> tmpVec;
    while (!stk.empty()) {
        tmpVec.push_back(stk.top());
        stk.pop();
    }
    for (auto iter = tmpVec.crbegin(); iter != tmpVec.crend(); ++iter)
        os << *iter;
}

template <typename T>
void print_vec(const std::vector<T> &vec, std::ostream &os = std::cout) {
    for (auto var : vec) 
        os << var << std::endl;
    os << std::endl;
}

template <typename T>
void print_vec1(const std::vector<std::shared_ptr<T>> &vec) {
    for (auto var : vec)
        std::cout << *var << std::endl;
    std::cout << std::endl;
}

#endif /* BASICFUNC_HPP */
