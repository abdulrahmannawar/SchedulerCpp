#pragma once

#include "../../context/context.hpp"
#include "fiber.hpp"
#include <deque>

class Scheduler
{
public:
    Scheduler();
    ~Scheduler();

    void spawn(Fiber *fiber);
    void do_it();
    void fiber_exit();
    void* get_data();
    void yield();

private:
    std::deque<Fiber *> fibers;
    Context *context;
    Fiber *active_fiber;
};