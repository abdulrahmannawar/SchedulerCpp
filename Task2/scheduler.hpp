#pragma once

#include <deque>
#include "fiber.hpp"
#include "../context/context.hpp"

class Scheduler {
public:
    Scheduler() {
        context = new Context;
        get_context(context);
    }
    ~Scheduler() {
        delete context;
    }
    void spawn(Fiber* f) {
        fibers.push_back(f);
    }
    void do_it() {
        get_context(context);
        if (!fibers.empty()) {
            Fiber* f = fibers.front();
            fibers.pop_front();
            Context* c = f->get_context();
            set_context(c);
        }
    }
    void fiber_exit() {
        set_context(context);
    }
private:
    std::deque<Fiber*> fibers;
    Context* context;
};