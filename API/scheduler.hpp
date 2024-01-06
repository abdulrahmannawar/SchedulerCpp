#pragma once

#include <deque>
#include "../context/context.hpp"
#include "fiber.hpp"

class Scheduler
{
public:
    Scheduler()
    {
        fibers = std::deque<Fiber*>();
        context = new Context;
    }

    ~Scheduler()
    {
        delete context;
    }

    void spawn(Fiber *f)
    {
        fibers.push_front(f);
    }
    
    void do_it()
    {
        get_context(context);
        if (!fibers.empty())
        {
            active_fiber = fibers.back();
            fibers.pop_back();
            if (active_fiber != nullptr) {
                Context *c = active_fiber->get_context();
                set_context(c);
            }
        }
    }

    void fiber_exit()
    {
        set_context(context);
    }

    void* get_data()
    {
        return active_fiber->get_data();
    }

    void yield()
    {
        if (!fibers.empty())
        {
            Fiber* new_fiber = fibers.back();
            fibers.pop_back();
            fibers.push_front(active_fiber);
            active_fiber = new_fiber;
            swap_context(fibers.back()->get_context(), active_fiber->get_context());
        }
    }

private:
    std::deque<Fiber *> fibers;
    Context *context;
    Fiber *active_fiber;
};