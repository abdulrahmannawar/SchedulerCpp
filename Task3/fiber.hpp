#pragma once

#include "../context/context.hpp"
#include <iostream>

class Fiber {
public:

    Fiber(void (*f)(), void* dp = nullptr) {
        this->dp = dp;
        context = new Context;
        stack_top = new char[4096];
        stack_bottom = stack_top + 4096;
        stack_bottom = reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(stack_bottom) & -16L);
        stack_bottom -= 128;
        context->rip = reinterpret_cast<void*>(f);
        context->rsp = stack_bottom;
    }

    ~Fiber() {
        delete[] stack_top;
    }

    Context* get_context() {
        return context;
    }

    void* get_data() {
        return this->dp;
    }

private:
    Context* context;
    char* stack_top;
    char* stack_bottom;
    void* dp;
};