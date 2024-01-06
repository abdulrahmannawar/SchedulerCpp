# Advanced Systems Programming - Assignment

## Contents
* [Overview](#overview)
* [Structure](#structure)
* [Dependencies](#dependencies)
* [Compiling and Running the Files](#compiling-and-running-the-files)
* [Tasks](#tasks)
* [Fiber API](#fiber-api)
* [Unit Tests](#unit-tests)

## Overview

This project comprises of a robust implementation of fibers and a scheduler in C++, providing a powerful concurrency model. The fiber API utilises the scheduler's core functionality but maintains a non-object-oriented paradigm for simplicity.

## Structure

The project is structured as follows:

* **`src/`** - Contains the source files for the project.
  * **`Task1-3`** - Contains the source files (**main.cpp**) for each task and the **fiber/scheduler** implementation for each task accordingly.
  * **`fiber.cpp`** - Contains the implementation of the fiber class.
  * **`scheduler.cpp`** - Contains the implementation of the scheduler class.
  * **`Tests`** - Contains the source file (**main.cpp**) for the tests.
* **`include/`** - Contains the header files for the Task2 and 3.
  * **`fiber.hpp`** - Contains the definition of the fiber class.
  * **`scheduler.hpp`** - Contains the definition of the scheduler class.
* **`context/`** - Contains the context switching implementation in ASM and its relevant **.hpp** file.
* **`build/`** - Contains the executables for the project.
* **`FiberAPI/`** - Contains the implementation of the fiber API.
* **`obj/`** - Contains the object file for the project (**context.o**).
* **`simpletest/`** - Contains the implementation of the simpletest unit test framework.
* **`Makefile`** - Contains the script for compiling the project.

## Dependencies
* [Clang (5 or above)](https://clang.llvm.org/)
* [GNU Make](https://www.gnu.org/software/make/)

## Compiling and Running the Files

To compile the files, run the following (in the root directory of the worksheet) command in the terminal:

~~~bash
make
~~~

This will compile the files and generate the executable for each task accordingly, as well as the context object file that is used by all the files. To run a specific executable, use the following command:

~~~bash
make run <executable_name>
~~~

The executables include:
* **Task 1 Pseudo Code:** `task1_pseudo`
* **Task 1 Main Code:** `task1`
* **Task 2 Main Code:** `task2`
* **Task 3 Main Code:** `task3`
* **Task 3 Main Code (API Version):** `task3_api`
* **Unit Tests:** `tests`

Example:

~~~bash
make run task1
~~~

## Tasks
* [Task 1](#task-1)
* [Task 2](#task-2)
* [Task 3](#task-3)


### Task 1

#### Overview

The first part of this task involves translating a given pseudo code into a C++ program using the provided context library. The code demonstrates control transfer within the same block in the main function.

The second part demonstrates the implementation of basic fibers and context switching.  

#### Part 1

~~~cpp
int main() {
    volatile int x = 0;
    Context* c = new Context;
    get_context(c);
    std::cout << "a message" << std::endl;
    if (x == 0) {
        ++x;
        set_context(c);
    }
    delete c;
    return 0;
}
~~~

 The program's current execution context is obtained and stored in the `Context` object using `get_context(c)`. Following this, the program outputs the message "a message" to the console. The code then checks if the volatile variable x is equal to 0. If true, it increments x and transfers control back to the previously stored context using `set_context(c)`, resulting in the string being printed twice. 

##### Output:

~~~bash
a message
a message
~~~

#### Part 2

~~~cpp
Context* main_context;
bool foo_called = false;
bool goo_called = false;

void goo();
void foo();

int main() {
    main_context = new Context;
    get_context(main_context);

    if (!foo_called) {
        char data_foo[4096];
        char* sp_foo = data_foo + sizeof(data_foo);
        sp_foo = reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(sp_foo) & -16L);
        sp_foo -= 128;
        Context* c_foo = new Context;
        c_foo->rip = reinterpret_cast<void*>(&foo);
        c_foo->rsp = sp_foo;
        set_context(c_foo);
    }

    if (!goo_called) {
        char data_goo[4096];
        char* sp_goo = data_goo + sizeof(data_goo);
        sp_goo = reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(sp_goo) & -16L);
        sp_goo -= 128;
        Context* c_goo = new Context;
        c_goo->rip = reinterpret_cast<void*>(&goo);
        c_goo->rsp = sp_goo;
        set_context(c_goo);
    }
}

void foo() {
    std::cout << "you called foo" << std::endl;
    foo_called = true;
    set_context(main_context);
}

void goo() {
    std::cout << "you called goo" << std::endl;
    goo_called = true;
    set_context(main_context);
}
~~~

The program defines two functions, `foo` and `goo`, each representing a distinct fiber with its own stack. The main function initialises the main context and captures it using `get_context`. It then checks whether the functions `foo` and `goo` have been called. If not, it allocates separate stacks for each function, adhering to Sys V ABI stack alignment and layout requirements. The program then sets up corresponding contexts for `foo` and `goo`, specifying the function's entry point, stack pointer, and ensures proper alignment. Upon execution, each function prints a message and sets a flag to indicate its invocation. Afterward, control is switched back to the main context.

##### Output:

~~~bash
you called foo
you called goo
~~~

### Task 2

#### Overview

Task 2 illustrates the implementation of a basic fiber system and scheduler. The `Fiber` class encapsulates a fiber with its own stack and context (based on last task), while the `Scheduler` class manages a queue of fibers using the [`std::deque`](https://en.cppreference.com/w/cpp/container/deque) class (double-ended queue where elements can be added or removed from the front or back) and orchestrates their execution. Each fiber is associated with a specific function, and the scheduler controls the flow of execution between these fibers. The fibers' data pointers are shared (`static`), allowing communication between them. In the main function, two fibers (f1 and f2) are created, associated with functions func1 and func2, respectively. The scheduler `spawn`s these fibers, and the `do_it` function executes the next available fiber. The fiber datapointer allows for modifying the specific contents of a fiber upon creation and afterwards. It is incremented after running the first fiber. The scheduler's `fiber_exit` function facilitates the transition back to the main context.

#### Why a Static Variable:

* **Shared Data Across Fibers:** The static variable allows all instances of the Fiber class to share a common data space (dp). This is useful for communication or sharing state among different fibers, as changes made by one fiber are visible to others.

* **Global Accessibility:** Being a static variable, Fiber::dp is globally accessible within the program. This is beneficial when fibers need access to shared data without explicitly passing it between functions or objects.

#### Alternative Approaches:

* **Passing Data Through Constructor:** Instead of using a static variable, data could be passed through the Fiber constructor. Each instance of the Fiber class could receive its specific data during instantiation. This method encapsulates data within each fiber instance, making it more localised.
Using Member Variables:

* The Fiber class could have non-static member variables to store fiber-specific data. This approach ensures that each fiber instance maintains its own data without interference from others and was used in Task 3 and the Fiber API (which is consider to be the best version of the library).

#### Output:
~~~cpp
void func1();
void func2();

int* Fiber::dp;
Scheduler s;

int main() {
    int d = 10;
    Fiber::dp = &d;
    Fiber f2(&func2, Fiber::dp);
    Fiber f1(&func1, Fiber::dp);
    s.spawn(&f1);
    s.spawn(&f2);
    s.do_it();
}

void func1() {
    std::cout << "Fiber 1\n";
    Fiber::dp = Fiber::get_data();
    std::cout << "Fiber 1: " << *Fiber::dp << std::endl;
    (*Fiber::dp)++;
    s.fiber_exit();
}

void func2() {
    Fiber::dp = Fiber::get_data();
    std::cout << "Fiber 2: " << *Fiber::dp << std::endl;
    s.fiber_exit();
}
~~~

~~~bash
Fiber 1
Fiber 1: 10
Fiber 2: 11
~~~


### Task 3

#### Overview

Task 3 extends (and slightly modifies) the already existing implementations of the `Fiber` and `Scheduler` classes. It implements the `yield` function and modifies the data pointer to be void, allowing for any datatype to be passed through it by caasting. Task 3 is also implemented using the [Fiber API](#fiber-api).

~~~cpp
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

~~~

#### Output:

~~~bash
fiber 1 before
10 (Func 1 dp)
Yield Called...
10 (Func 2 dp)
fiber 2
(Fiber 2 exits, fiber 1 resumes)
fiber 1 after
~~~

## Fiber API

The fiber API is a non-object-oriented implementation of the fiber system and scheduler. It is a simplified version of the Task 3 implementation. It calls the `Scheduler` methods in the body of the functions, allowing for a more concise and readable code when used since there is no [OOP](https://en.wikipedia.org/wiki/Object-oriented_programming).

The main difference between the API and the Task 3 implementation is that the API does not directly use the `Fiber` class to pass a function pointer. Instead, it uses a `void*` data pointer to pass data between fibers using `spawn`, which ultimately calls the `Fiber` class.

## Unit Tests

The unit tests were implemented using the [`simpletest`](https://github.com/kudaba/simpletest.git) library as a submodule. 

The tests included are:
* **`TestSpawnWithoutExecution`:** Tests what happens when the scheduler spawns a fiber without executing it.
* **`TestSpawnWithExecution`:** Tests what happens when the scheduler spawns a fiber and executes it.
* **`TestYield`:** Tests what happens when the scheduler yields the execution of a fiber.
* **`TestQueueClearedAfterExecution`:** Tests what happens when the scheduler clears the queue after executing a fiber.
* **`TestConcurrency`:** Tests what happens when the scheduler executes multiple fibers concurrently.
* **`TestNestedFiber`:**  Tests what happens when the scheduler executes a fiber that spawns another fiber.

# [Back To Top](#contents)