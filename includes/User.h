// User.h
#ifndef USER_H
#define USER_H

#include <string>

struct User {
    std::string name;
    int borrowedCount;
    double fine; // натрупани глоби

    User() : borrowedCount(0), fine(0.0) {}
};

#endif
