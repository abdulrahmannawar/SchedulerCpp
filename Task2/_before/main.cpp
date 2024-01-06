#include "../../context/context.hpp"
#include "fiber.hpp"
#include "scheduler.hpp"
#include <iostream>

void foo();
void bar();

Scheduler s;

int main() {
    Fiber f2(&foo);
    Fiber f1(&bar);

    s.spawn(&f1);
    s.spawn(&f2);

    s.do_it();
}

void foo() {
    std::cout << "Fiber 1: foo" << std::endl;
    s.fiber_exit();
}

void bar() {
    std::cout << "Fiber 2: bar" << std::endl;
    s.fiber_exit();
}