#include "Library.h"
#include <iostream>
#include <cstring>

Library::Library(int count)
    : bookCount(0), nextId(1)
{
    books = new Book[count];
}

Library::~Library() {
    delete[] books;
}

bool Library::addBook(const Book& b) {
    books[bookCount] = b;
    books[bookCount].id = nextId++;
    ++bookCount;
    return true;
}

Book* Library::findById(int id) {
    for (int i = 0; i < bookCount; ++i)
        if (books[i].id == id) return &books[i];
    return nullptr;
}

bool Library::removeBookById(int id) {
    for (int i = 0; i < bookCount; ++i) {
        if (books[i].id == id) {
            books[i] = books[bookCount - 1];
            --bookCount;
            return true;
        }
    }
    return false;
}

bool Library::existsDuplicate(const std::string& title,
                              const std::string authors[], int authCount) const {
    for (int i = 0; i < bookCount; ++i) {
        if (books[i].title == title && books[i].authorsCount == authCount) {
            bool same = true;
            for (int j = 0; j < authCount; ++j)
                if (books[i].authors[j] != authors[j]) { same = false; break; }
            if (same) return true;
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const Library& lib) {
    for (int i = 0; i < lib.bookCount; ++i) {
        const Book& b = lib.books[i];
        os << "ID: " << b.id << "\nTitle: " << b.title << "\nAuthors: ";
        for (int j = 0; j < b.authorsCount; ++j) {
            os << b.authors[j];
            if (j + 1 < b.authorsCount) os << ", ";
        }
        os << "\nAvailable: " << (b.available ? "Yes" : "No")
           << "\nBorrowed date: " << b.borrowedDate << "\n\n";
    }
    return os;
}
