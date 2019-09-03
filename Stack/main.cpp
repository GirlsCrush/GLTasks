#include <iostream>
#include "stack.h"
#include <string> 

class Bomb{
public:
    Bomb (int code) {
        if (code != 42) {
            throw std::exception();
        }
    }
};
class ComplexType {
public:
    ComplexType() : b1(42), b2(42) {}
    ComplexType(const char* name) try : name(name), b1(2), b2(42) {
        
    }
    catch (std::exception &ex) {
        std::cout << "Smthng happend" << std::endl;
    }
private:
    std::string name;
    friend std::ostream& operator<<(std::ostream &, const ComplexType &);
    Bomb b1;
    Bomb b2;
};

std::ostream& operator<<(std::ostream &os, const ComplexType &obj) {
    os << obj.name;
    return os;
}

void testStringStack() {
    stack<ComplexType> words;
    std::cout << words.size() << std::endl;
    try {
    
    words.push("this");
    words.push("is");
    words.push("a");

    std::cout << words.top() << std::endl;
    words.pop();
    std::cout << words.top() << std::endl;
    words.pop();
    std::cout << words.top() << std::endl;
    words.pop();
    std::cout << words.top() << std::endl;
    words.pop();

    }
    catch (const StackOverflowException &ex) {
        std::cout << "Stack size limit exceeded" << std::endl;
    }
    catch (const StackEmptyException &ex) {
        std::cout << "Can't pop from an empty stack" << std::endl;
    }
    catch (const StackException &ex) {
        throw;
    }
    catch (...) {
        std::cout << "Stack size limit exceeded" << std::endl;
    }
    
    words.push("sad");
    std::cout << words.size() << std::endl;
}

int main() {
    try {
        testStringStack();
    }
    catch(const std::exception &ex) {
        std::cout << "Something failed" << std::endl;
    }
    return 0;
}