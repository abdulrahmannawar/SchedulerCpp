#include "fiber.h"
#include "scheduler.hpp"

Scheduler s;

void fiber_exit() {
    s.fiber_exit();
}

void* get_data() {
    return s.get_data();
}

void yield() {
    s.yield();
}

void spawn(void (*f)(), void* data) {
    Fiber* fiber = new Fiber(f, data);
    s.spawn(fiber);
}

void do_it() {
    s.do_it();
}