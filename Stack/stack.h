#include <memory>

class StackException : public std::exception {
    const char* what() const noexcept {
        return "StackException";
    }
};

class StackOverflowException : public StackException {
public:
    const char* what() const noexcept {
        return "StackOverflowException";
    }
};

class StackEmptyException : public StackException {
public:
    const char* what() const noexcept {
        return "StackEmptyException";
    }
};

template <typename T,  int MAX_SIZE = 3>
class stack {
public:
    void push(const T& t) {
        if(size() < MAX_SIZE)
            data[++_top] = t;
        else {
            throw StackOverflowException();
        }
    }

    void pop() {
        if(size() > 0) {
            _top--;
        }
        else {
            throw StackEmptyException();
        }
    }

    T top() {
        if(size() > 0)
            return data[_top];
        else {
            throw StackEmptyException();
        }
    }

    unsigned size() {
        return _top + 1;
    }

private:
    std::unique_ptr<T[]> data = std::make_unique<T[]>(MAX_SIZE);
    int _top = -1;
};