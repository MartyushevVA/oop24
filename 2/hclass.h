#ifndef HCLASS_H
#define HCLASS_H

#include "alllibs.h"
#include "sclass.h"

class stack
{
    friend std::ostream &operator<<(std::ostream &os, const stack &stack);
    friend std::istream &operator>>(std::istream &in, stack &stack);

private:
    size_t size;
    size_t allctd;
    task *vector;

public:
    stack();
    stack(size_t size, const task (&space)[]);
    stack(const stack &other);
    stack(stack &&other);
    ~stack();
    stack &operator=(const stack &other);
    void operator+=(const task &t);
    task pop();
    int fullness() const;
    void unioning();
    void fragmentation();
    task extractNextUngraded();
    void smoothResize(size_t hardSize);
};

std::ostream &operator<<(std::ostream &os, const stack &stack);
std::istream &operator>>(std::istream &in, stack &stack);

#endif