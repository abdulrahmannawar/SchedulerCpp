#include "../../include/Task3/fiber.hpp"
#include "../../context/context.hpp"
#include <cstdint>

Fiber::Fiber(void (*func)(), void *dp)
{
    this->dp = dp;
    context = new Context;
    stack_top = new char[4096];
    stack_bottom = stack_top + 4096;
    stack_bottom = reinterpret_cast<char *>(reinterpret_cast<uintptr_t>(stack_bottom) & -16L);
    stack_bottom -= 128;
    context->rip = reinterpret_cast<void *>(func);
    context->rsp = stack_bottom;
}

Fiber::~Fiber()
{
    delete[] stack_top;
}

Context* Fiber::get_context()
{
    return context;
}

void* Fiber::get_data()
{
    return this->dp;
}