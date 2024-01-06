#include "../context/context.hpp"
#include <iostream>
#include <cstdlib>

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