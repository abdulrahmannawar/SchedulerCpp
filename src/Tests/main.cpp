#include <iostream>
#include "../../FiberAPI/fiber.h"
#include "../../simpletest/simpletest.h"

const char *group = "Fiber";
void modify_dp();
void increment_dp();
void print_and_yield();
void print_and_yield2();
void nested_fiber();

//////////////////////////////////
//           MAIN              //
////////////////////////////////


int main() {
    bool pass = true;

    pass &= TestFixture::ExecuteTestGroup(group, TestFixture::OutputMode::Verbose);

    return pass ? 0 : 1;
}

//////////////////////////////////
//           TESTS             //
////////////////////////////////

DEFINE_TEST_G(TestSpawnWithoutExecution, Fiber)
{
    int d = 1234;
    int *dp = &d;
    spawn(modify_dp, dp);
    TEST_MESSAGE(*dp != 4321, "Fiber spawned but not executed, data not modified");
}

DEFINE_TEST_G(TestSpawnWithExecution, Fiber)
{
    int d = 1234;
    int *dp = &d;
    spawn(modify_dp, dp);
    do_it();
    TEST_MESSAGE(*dp == 4321, "Fiber spawned and executed, data modified");
}

DEFINE_TEST_G(TestQueueClearedAfterExecution, Fiber)
{
    int d = 1234;
    int *dp = &d;
    spawn(increment_dp, dp);
    do_it();
    TEST_MESSAGE(*dp == 1235, "Function queued once, data modified once");
    do_it();
    TEST_MESSAGE(*dp == 1235, "Function queued twice, data modified twice when it should only be modified once");
}


DEFINE_TEST_G(TestYield, Fiber)
{
    spawn(print_and_yield);
    spawn(print_and_yield2);
    do_it();
}

DEFINE_TEST_G(TestConcurrency, Fiber)
{
    int d = 0;
    int *dp = &d;

    spawn(modify_dp, dp);
    spawn(increment_dp, dp);

    do_it();

    TEST_MESSAGE(*dp == 4322, "Concurrent execution - data modified correctly");
}

DEFINE_TEST_G(TestNestedFiber, Fiber)
{
    int d = 0;
    int *dp = &d;

    spawn(nested_fiber, dp);
    yield();
    do_it();
    TEST_MESSAGE(*dp == 1, "Nested fiber execution - data not modified");
}

//////////////////////////////////
//           FUNCTIONS         //
////////////////////////////////

void modify_dp()
{
    int *dp = (int*)get_data();
    *dp = 4321;
    fiber_exit();
}

void increment_dp()
{
    int *dp = (int*)get_data();
    (*dp)++;
    fiber_exit();
}

void print_and_yield() {
    std::cout << "Fiber 1 - Message 1" << std::endl;
    yield();
    std::cout << "Fiber 1 - Message 2" << std::endl;
    fiber_exit();
}

void print_and_yield2() {
    std::cout << "Fiber 2 - Message 1" << std::endl;
    yield();
    std::cout << "Fiber 2 - Message 2" << std::endl;
    fiber_exit();
}

void nested_fiber()
{
    int *dp = (int*)get_data();
    spawn(increment_dp, dp);
    do_it();
    fiber_exit();
}