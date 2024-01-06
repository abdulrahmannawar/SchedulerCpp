#include "../../context/context.hpp"
#include "../../include/Task2/fiber.hpp"
#include "../../include/Task2/scheduler.hpp"
#include <iostream>

void func1();
void func2();

int* Fiber::dp;
Scheduler s;

int main() {
    int d = 10;
    Fiber::dp = &d;
    Fiber f2(&func2, Fiber::dp);
    Fiber f1(&func1, Fiber::dp);
    s.spawn(&f1);
    s.spawn(&f2);
    s.do_it();
}

void func1() {
    std::cout << "Fiber 1\n";
    Fiber::dp = Fiber::get_data();
    std::cout << "Fiber 1: " << *Fiber::dp << std::endl;
    (*Fiber::dp)++;
    s.fiber_exit();
}

void func2() {
    Fiber::dp = Fiber::get_data();
    std::cout << "Fiber 2: " << *Fiber::dp << std::endl;
    s.fiber_exit();
}