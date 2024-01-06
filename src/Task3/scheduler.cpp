#include "../../include/Task3/scheduler.hpp"
#include "../../include/Task3/fiber.hpp"
#include "../../context/context.hpp"
#include <deque>

Scheduler::Scheduler()
{
    fibers = std::deque<Fiber *>();
    context = new Context;
}

Scheduler::~Scheduler()
{
    delete context;
}

void Scheduler::spawn(Fiber *fiber)
{
    fibers.push_front(fiber);
}

void Scheduler::do_it()
{
    get_context(context);
    if (!fibers.empty())
    {
        active_fiber = fibers.back();
        fibers.pop_back();
        if (active_fiber != nullptr)
        {
            Context *c = active_fiber->get_context();
            set_context(c);
        }
    }
}

void Scheduler::fiber_exit()
{
    set_context(context);
}

void *Scheduler::get_data()
{
    return active_fiber->get_data();
}

void Scheduler::yield()
{
    if (!fibers.empty())
    {
        Fiber *new_fiber = fibers.back();
        fibers.pop_back();
        fibers.push_front(active_fiber);
        active_fiber = new_fiber;
        swap_context(fibers.back()->get_context(), active_fiber->get_context());
    }
}