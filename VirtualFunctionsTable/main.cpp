#include <iostream>
#include <vector>
#include <functional>

/* TOREAD:
 * Added a member vtable_ptr to class A (it's abstract actually). It has a vtable 
 * type, which contains pointers to member-functions.
 * There're some changes in B and C classes - assigning a pointer to a vtable 
 * for each of them in the constructor.
 */

struct vtable;

class A
{
private:
    int fooA(int a, int b) { std::cout << "A::foo()" << std::endl; return a * b; }
    void printA() { std::cout << "A::print()" << std::endl; }
protected:
    const vtable* const vtable_ptr;
public:
    A();
    A(const vtable *vtable_ptr) : vtable_ptr(vtable_ptr) {}
    int foo(int a, int b);
    void print();
    ~A();
};

class B : public A
{
public:
    B();
    int foo(int a, int b) { std::cout << "B::foo()" << std::endl; return a * b; }
    void print() { std::cout << "B::print()" << std::endl; }
};

class C : public A
{
public:
    C();
    int foo(int a, int b) { std::cout << "C::foo()" << std::endl; return a * b; }
    void print() { std::cout << "C::print()" << std::endl; }
};

struct vtable{
    void (A::*print)();
    int (A::*foo)(int, int);

    vtable(void (A::*print)(), int (A::*foo)(int, int)) 
    : print(print), foo(foo){}
};

int A::foo(int a, int b) { return (this->*(vtable_ptr->foo))(a, b); }

void A::print(){ (this->*(vtable_ptr->print))(); }

A::~A(){delete vtable_ptr;};

A::A() : A(new vtable(
    static_cast<void (A::*)()>(&A::printA),
    static_cast<int (A::*)(int, int)>(&A::fooA)
)){};

B::B() : A(new vtable(
    static_cast<void (A::*)()>(&B::print),
    static_cast<int (A::*)(int, int)>(&B::foo)
)){};

C::C() : A(new vtable(
    static_cast<void (A::*)()>(&C::print),
    static_cast<int (A::*)(int, int)>(&C::foo)
)){};



int main()
{
    // Actual result:
    // A::foo()
    // A::print()
    // A::foo()
    // A::print()

    // Expected result:
    // B::foo()
    // B::print()
    // C::foo()
    // C::print()

    // TODO: Implement vtable support for class A, class B and class C without using of virtual keyword.
    // Do not change main() and if possible do not change class B and class C.

    {
        A a;
        A *ap = &a;
        ap->foo(5, 10);
        ap->print();
    }

    {
        B b;
        A *a = &b;
        a->foo(5, 10);
        a->print();
    }

    {
        C c;
        A *a = &c;
        a->foo(5, 10);
        a->print();
    }
}

// A::print();
// auto p = &A::print;
// p();

// A a;
// a.foo(5, 10);

// B b;
// A *pA = &b;

// void (B::*p)() = &B::print;
// (pA->*p)();