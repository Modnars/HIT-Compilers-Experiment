#include "lib/Production.hpp"

#include <stack>

void test1() {
    std::vector<std::string> svec = {"Hello", "World", "Test"};
    Production p = Production("Compile", svec);
    std::cout << "Compiled Done." << std::endl;
    std::cout << p.get_left() << std::endl;
    for (auto x : p.get_right())
        std::cout << x << " ";
    std::cout << std::endl;
}

void test2() {
    std::vector<std::string> v1 = {"Hello", "world"};
    std::vector<std::string> v2 = {"Hello", "World"};
    std::vector<std::string> v3 = {"Hello", "world"};
    std::vector<std::string> v4 = {"world", "Hello"};
    std::cout << (v1 == v2) << std::endl;
    std::cout << (v1 == v3) << std::endl;
    std::cout << (v1 == v4) << std::endl;
}

bool shift(std::string &str) {
    auto pos = str.find_first_of("@");
    if (pos >= str.size()-1)
        return false;
    std::swap(str[pos], str[pos+1]);
    return true;
}

int main(int argc, char *argv[]) {
    std::stack<int> state;
    for (int i = 0; i < 5; ++i)
        state.push(i);
    for (int i = 0; i < 3; ++i) {
        std::cout << state.top() << std::endl;
        state.pop();
    }
    return 0;
}

