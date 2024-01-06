#include "../include/Task3/fiber.hpp"
#include "../context/context.hpp"
#include <cstdint>

/**
 * @brief Constructor for the Fiber class.
 * 
 * Initialises a new Fiber with the provided function pointer and data pointer.
 * Allocates a new context and stack for the fiber, ensuring proper alignment
 * based on Sys V ABI requirements.
 * 
 * @param func Function pointer to the fiber's entry point.
 * @param dp   Pointer to the data associated with the fiber.
 */
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

/**
 * @brief Destructor for the Fiber class.
 * 
 * Deallocates the dynamically allocated stack memory for the fiber.
 */
Fiber::~Fiber()
{
    delete[] stack_top;
}

/**
 * @brief Getter function for the fiber's context.
 * 
 * @return A pointer to the fiber's context.
 */
Context* Fiber::get_context()
{
    return context;
}

/**
 * @brief Getter function for the data associated with the fiber.
 * 
 * @return A pointer to the fiber's data.
 */
void* Fiber::get_data()
{
    return this->dp;
}
