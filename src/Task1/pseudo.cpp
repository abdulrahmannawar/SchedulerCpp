#include "../context/context.hpp"
#include <iostream>
 
int main() {
    volatile int x = 0;
    Context* c = new Context;
    get_context(c);
    std::cout << "a message" << std::endl;
    if (x == 0) {
        ++x;
        set_context(c);
    }
    delete c;
    return 0;
}