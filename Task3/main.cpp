#include <iostream>
#include "fiber.hpp"
#include "scheduler.hpp"

Scheduler scheduler;

void func1() {
    std::cout << "fiber 1 before" << std::endl;
    int* dp = (int*)scheduler.get_data();
    std::cout << *dp << std::endl;
    scheduler.yield();
    std::cout << "fiber 1 after" << std::endl;
    scheduler.fiber_exit();
}

void func2() {
    int* dp = (int*)scheduler.get_data();
    std::cout << *dp << std::endl;
    std::cout << "fiber 2" << std::endl;
    scheduler.fiber_exit();
}

int main() {
    int* dp = new int(10);

    Fiber* fiber1 = new Fiber(func1, dp);
    Fiber* fiber2 = new Fiber(func2, dp);

    scheduler.spawn(fiber1);
    scheduler.spawn(fiber2);

    scheduler.do_it();

    return 0;
}
