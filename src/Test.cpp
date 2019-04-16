#include "lib/Production.hpp"

int main(int argc, char *argv[]) {
    std::vector<std::string> svec = {"Hello", "World", "Test"};
    Production p = Production("Compile", svec);
    std::cout << "Compiled Done." << std::endl;
    std::cout << p.get_left() << std::endl;
    for (auto x : p.get_right())
        std::cout << x << " ";
    std::cout << std::endl;
    return 0;
}

