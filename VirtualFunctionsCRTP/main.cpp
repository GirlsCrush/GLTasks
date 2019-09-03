#include <iostream>

template<typename T>
class A
{
public:
    /*virtual*/ int foo(int a, int b) { 
        T& underlying = static_cast<T&>(*this);
        return underlying.foo(a, b);
        // std::cout << "A::foo()" << std::endl; 
        // return a + b; 
        }
    /*virtual*/ void print() { 
        T& underlying = static_cast<T&>(*this);
        underlying.print();
        // std::cout << "A::print()" << std::endl; 
        }
};

class B : public A<B>
{
public:
    int foo(int a, int b) { std::cout << "B::foo()" << std::endl; return a * b; }
    void print() { std::cout << "B::print()" << std::endl; }
};

class C : public A<C>
{
public:
    int foo(int a, int b) { std::cout << "C::foo()" << std::endl; return a * b; }
    void print() { std::cout << "C::print()" << std::endl; }
};

template<typename T>
void fooAndPrint(A<T> & base, int a, int b){
    base.foo(a, b);
    base.print();
}

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
        B b;
        fooAndPrint(b, 5, 10);
        // A *a = &b;
        // a->foo(5, 10);
        // a->print();
    }

    {
        C c;
        fooAndPrint(c, 5, 10);
        // A *a = &c;
        // a->foo(5, 10);
        // a->print();
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