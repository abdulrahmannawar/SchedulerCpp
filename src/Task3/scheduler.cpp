#include "../../include/Task3/scheduler.hpp"
#include "../../include/Task3/fiber.hpp"
#include "../../context/context.hpp"
#include <deque>

/**
 * @brief Constructor for the Scheduler class.
 * 
 * Initializes a new Scheduler with an empty deque of fibers and a new context.
 */
Scheduler::Scheduler()
{
    fibers = std::deque<Fiber *>();
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
 * @brief Adds a new fiber to the front of the scheduler's deque.
 * 
 * @param fiber Pointer to the fiber to be added to the scheduler.
 */
void Scheduler::spawn(Fiber *fiber)
{
    fibers.push_front(fiber);
}

/**
 * @brief Executes the next available fiber in the deque.
 * 
 * Retrieves the context of the last fiber added to the deque, sets it as the active fiber,
 * and switches the context to execute that fiber.
 */
void Scheduler::do_it()
{
    // Get the context of the main thread and store it in 'context'.
    get_context(context);

    // Check if there are fibers in the queue.
    if (!fibers.empty())
    {
        // Retrieve the last fiber from the deque (LIFO order).
        active_fiber = fibers.back();

        // Remove the last fiber from the deque.
        fibers.pop_back();

        // Check if the active fiber is not null (deque wasn't empty).
        if (active_fiber != nullptr)
        {
            // Obtain the context of the active fiber.
            Context *c = active_fiber->get_context();

            // Switch the context to the active fiber, initiating its execution.
            set_context(c);
        }
    }
    // If the deque is empty, continue with the main thread's context.
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
 * @return A pointer to the data associated with the active fiber.
 */
void *Scheduler::get_data()
{
    return active_fiber->get_data();
}

/**
 * @brief Yields the control to the next available fiber in the deque.
 * 
 * If there are fibers in the deque, the current active fiber is swapped with
 * the next one, and the context is switched to execute the new active fiber.
 */
void Scheduler::yield()
{
    // Check if there are fibers in the queue.
    if (!fibers.empty())
    {
        // Get the last fiber from the deque (LIFO order).
        Fiber *new_fiber = fibers.back();

        // Remove the last fiber from the deque.
        fibers.pop_back();

        // Place the previously active fiber at the front of the deque.
        fibers.push_front(active_fiber);

        // Save a reference to the old active fiber.
        Fiber *old_fiber = active_fiber;

        // Set the new active fiber to the one retrieved from the deque.
        active_fiber = new_fiber;

        // Swap the contexts between the old and new active fibers.
        // This effectively switches execution control between fibers.
        swap_context(old_fiber->get_context(), active_fiber->get_context());
    }
    // If the deque is empty, the function returns without performing any context switch.
}
