#include <deque>
#include "../../include/Task2/scheduler.hpp"
#include "../../include/Task2/fiber.hpp"
#include "../../context/context.hpp"

Scheduler::Scheduler()
{ 
    fibers = std::deque<Fiber*>();
    context = new Context;
}

Scheduler::~Scheduler()
{
    delete context;
}

void Scheduler::spawn(Fiber *fiber)
{
    fibers.push_back(fiber);
}

void Scheduler::do_it()
{
    get_context(context);
    if (!fibers.empty())
    {
        Fiber *f = fibers.front();
        fibers.pop_front();
        Context *c = f->get_context();
        set_context(c);
    }
}

void Scheduler::fiber_exit()
{
    set_context(context);
}

auto Scheduler::get_data()
{
    return Fiber::get_data() ? Fiber::get_data() : nullptr;
}