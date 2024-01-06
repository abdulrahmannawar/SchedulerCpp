#pragma once

#include "../../context/context.hpp"
#include <iostream>

class Fiber {
public:

    Fiber(void (*func)(), void* dp = nullptr);
    ~Fiber();

    Context* get_context();
    void* get_data();

private:
    Context* context;
    char* stack_top;
    char* stack_bottom;
    void* dp;
};