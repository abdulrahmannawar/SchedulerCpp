#pragma once

#include "../../context/context.hpp"
#include <iostream>

class Fiber {
public:
    static int* dp;

    Fiber(void (*func)(void), int* dp = nullptr);
    ~Fiber();

    Context* get_context();
    static int* get_data();

private:
    Context context;
    char* stack_top;
    char* stack_bottom;
};