#ifndef BOOK_H
#define BOOK_H

#include <string>
#include "Genre.h"

struct Book {
    int id;
    std::string title;
    Genre genre; 
    std::string authors[5];
    int authorsCount;
    bool available;
    std::string borrowedDate;
    int borrowCount;
    std::string currentUser;

    Book() : id(-1), genre(Fiction), authorsCount(0), available(true),
             borrowedDate(""), borrowCount(0), currentUser("") {}
};

#endif