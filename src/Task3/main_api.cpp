#include <iostream>
#include "../FiberAPI/fiber.h"

void func1() {
    std::cout << "fiber 1 before" << std::endl;
    std::cout << *(int*)get_data() << std::endl;
    yield();
    std::cout << "fiber 1 after" << std::endl;
    fiber_exit();
}

void func2() {
    std::cout << *(int*)get_data() << std::endl;
    std::cout << "fiber 2" << std::endl;
    fiber_exit();
}

int main() {
    int* dp = new int(10);

    spawn(func1, dp);
    spawn(func2, dp);

    do_it();

    return 0;
}
