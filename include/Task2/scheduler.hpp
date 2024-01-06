#pragma once

#include <deque>
#include "fiber.hpp"
#include "../../context/context.hpp"

class Scheduler {
public:
    Scheduler();
    ~Scheduler();

    void spawn(Fiber* fiber);
    void do_it();
    void fiber_exit();
    auto get_data();
    
private:
    std::deque<Fiber*> fibers;
    Context* context;
};