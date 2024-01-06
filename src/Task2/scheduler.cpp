#include <deque>
#include "../../include/Task2/scheduler.hpp"
#include "../../include/Task2/fiber.hpp"
#include "../../context/context.hpp"

/**
 * @brief Constructor for the Scheduler class.
 * 
 * Initializes a new Scheduler with an empty deque of fibers and a new context.
 */
Scheduler::Scheduler()
{ 
    fibers = std::deque<Fiber*>();
    context = new Context;
}

/**
 * @brief Destructor for the Scheduler class.
 * 
 * Deallocates the dynamically allocated context for the scheduler.
 */
Scheduler::~Scheduler()
{
    delete context;
}

/**
 * @brief Adds a new fiber to the back of the scheduler's deque.
 * 
 * @param fiber Pointer to the fiber to be added to the scheduler.
 */
void Scheduler::spawn(Fiber *fiber)
{
    fibers.push_back(fiber);
}

/**
 * @brief Executes the next available fiber in the deque.
 * 
 * Retrieves the context of the first fiber added to the deque, sets it as the active fiber,
 * and switches the context to execute that fiber.
 */
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

/**
 * @brief Transitions control back to the main context.
 */
void Scheduler::fiber_exit()
{
    set_context(context);
}

/**
 * @brief Getter function for the data associated with the active fiber.
 * 
 * @return A pointer to the data associated with the active fiber, or nullptr if no active fiber.
 */
auto Scheduler::get_data()
{
    return Fiber::get_data() ? Fiber::get_data() : nullptr;
}
