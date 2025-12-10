// Library.h
#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
#include "Book.h"

class Library {
protected:
    int bookCount;
    Book* books;      // динамичен масив
    int nextId;       // за уникални ID

public:
    Library(int count);
    virtual ~Library();

    int getBookCount() const { return bookCount; }
    Book* getBooks() const { return books; }

    // добавяне/изтриване
    bool addBook(const Book& b);
    bool removeBookById(int id);

    // търсене
    Book* findById(int id);
    bool existsDuplicate(const std::string& title,
                         const std::string authors[], int authCount) const;

    friend std::ostream& operator<<(std::ostream& os, const Library& lib);
};

#endif
