#include "BookInfo.h"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include <ctime>
#include <limits>

// ===== Конструктор =====

BookInfo::BookInfo(int capacity) : Library(capacity) {
    // Инициализация на производния клас
}

// ===== Помощни функции за дати =====

static void parseDate(const std::string& d, int& y, int& m, int& day) {
    // очаква валиден формат "YYYY-MM-DD"
    y   = std::stoi(d.substr(0, 4));
    m   = std::stoi(d.substr(5, 2));
    day = std::stoi(d.substr(8, 2));
}

static int toDays(int y, int m, int d) {
    static int mdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    int sum = y * 365;
    for (int i = 0; i < m - 1; ++i) sum += mdays[i];
    sum += d;
    return sum;
}

int BookInfo::dateDiffDays(const std::string& from, const std::string& to) {
    if (from.empty() || to.empty()) return 0;
    int y1,m1,d1, y2,m2,d2;
    parseDate(from, y1, m1, d1);
    parseDate(to,   y2, m2, d2);
    return toDays(y2, m2, d2) - toDays(y1, m1, d1);
}

// ===== Функции от задачата =====

void BookInfo::printAvailableSortedByAuthorAndGenre(Genre filterGenre) const {
    if (bookCount == 0) {
        std::cout << "No books.\n";
        return;
    }

    int* idx = new int[bookCount];
    int n = 0;
    for (int i = 0; i < bookCount; ++i) {
        if (books[i].available && books[i].genre == filterGenre)
            idx[n++] = i;
    }

    std::sort(idx, idx + n, [this](int a, int b){
        return books[a].authors[0] < books[b].authors[0];
    });

    int countAvailable = 0;
    for (int i = 0; i < n; ++i) {
        const Book& b = books[idx[i]];
        ++countAvailable;
        std::cout << b.title << " (" << b.authors[0] << ")\n";
    }
    std::cout << "Total available in genre: " << countAvailable << "\n";

    delete[] idx;
}

void BookInfo::printBorrowedOnDate(const std::string& date,
                                   const std::string& currentDate) const {
    std::cout << "Books borrowed on " << date << ":\n";
    for (int i = 0; i < bookCount; ++i) {
        const Book& b = books[i];
        if (!b.available && b.borrowedDate == date) {
            int days = dateDiffDays(b.borrowedDate, currentDate);
            std::cout << "Title: " << b.title << "\nAuthors: ";
            for (int j = 0; j < b.authorsCount; ++j) {
                std::cout << b.authors[j];
                if (j + 1 < b.authorsCount) std::cout << ", ";
            }
            std::cout << "\nDays since borrowed: " << days << "\n\n";
        }
    }
}

void BookInfo::printOverdueBooks(const std::string& currentDate,
                                 double finePerDay) const {
    std::cout << "Overdue books (>30 days):\n";
    for (int i = 0; i < bookCount; ++i) {
        const Book& b = books[i];
        if (!b.available && !b.borrowedDate.empty()) {
            int days = dateDiffDays(b.borrowedDate, currentDate);
            if (days > 30) {
                int overdue = days - 30;
                double fine = overdue * finePerDay;
                std::cout << "Title: " << b.title << "\nBorrowed: "
                          << b.borrowedDate << "\nDays: " << days
                          << " (overdue: " << overdue << ")\n"
                          << "Fine: " << fine << " EUR\n\n";
            }
        }
    }
}

void BookInfo::saveMultipleAuthorsAndCombos(const std::string& filename) const {
    std::ofstream fout(filename);
    if (!fout) {
        std::cerr << "Cannot open file: " << filename << "\n";
        return;
    }

    std::map<std::string,int> comboCount;

    for (int i = 0; i < bookCount; ++i) {
        const Book& b = books[i];
        if (b.authorsCount > 1) {
            fout << "Title: " << b.title << "\nAuthors: ";
            for (int j = 0; j < b.authorsCount; ++j) {
                fout << b.authors[j];
                if (j + 1 < b.authorsCount) fout << ", ";
            }
            fout << "\n\n";

            std::string key;
            for (int j = 0; j < b.authorsCount; ++j) {
                if (j > 0) key += "|";
                key += b.authors[j];
            }
            ++comboCount[key];
        }
    }

    fout << "Author combinations:\n";
    for (const auto& p : comboCount) {
        fout << p.first << " -> " << p.second << "\n";
    }
}

void BookInfo::saveAuthorStats(const std::string& filename) const {
    std::map<std::string,int> authorCount;

    for (int i = 0; i < bookCount; ++i) {
        const Book& b = books[i];
        for (int j = 0; j < b.authorsCount; ++j) {
            ++authorCount[b.authors[j]];
        }
    }

    std::vector<std::pair<std::string,int>> vec;
    vec.reserve(authorCount.size());
    for (const auto& p : authorCount) vec.push_back(p);

    std::sort(vec.begin(), vec.end(),
              [](const auto& a, const auto& b){
                  return a.second > b.second;
              });

    std::ofstream fout(filename);
    if (!fout) {
        std::cerr << "Cannot open file: " << filename << "\n";
        return;
    }

    for (const auto& p : vec) {
        fout << p.first << " : " << p.second << "\n";
    }
}

void BookInfo::searchByKeyword(const std::string& keyword,
                               bool saveToFile,
                               const std::string& filename) const {
    std::ofstream fout;
    if (saveToFile) {
        fout.open(filename);
        if (!fout) {
            std::cerr << "Cannot open file: " << filename << "\n";
            return;
        }
    }

    auto outBook = [&](const Book& b, std::ostream& os){
        os << "ID: " << b.id << "\nTitle: " << b.title << "\nAuthors: ";
        for (int j = 0; j < b.authorsCount; ++j) {
            os << b.authors[j];
            if (j + 1 < b.authorsCount) os << ", ";
        }
        os << "\nAvailable: " << (b.available ? "Yes" : "No") << "\n\n";
    };

    for (int i = 0; i < bookCount; ++i) {
        const Book& b = books[i];
        bool match = false;

        if (b.title.find(keyword) != std::string::npos) match = true;
        else {
            for (int j = 0; j < b.authorsCount; ++j) {
                if (b.authors[j].find(keyword) != std::string::npos) {
                    match = true;
                    break;
                }
            }
        }

        if (match) {
            outBook(b, std::cout);
            if (saveToFile) outBook(b, fout);
        }
    }
}

void BookInfo::addBookFromConsole() {
    Book b;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Title: ";
    std::getline(std::cin, b.title);

    int g;
    std::cout << "Genre (0 = Fiction, 1 = NonFiction): ";
    std::cin >> g;
    b.genre = (g == 0) ? Fiction : NonFiction;

    std::cout << "Number of authors (1-5): ";
    std::cin >> b.authorsCount;
    if (b.authorsCount < 1) b.authorsCount = 1;
    if (b.authorsCount > 5) b.authorsCount = 5;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    for (int i = 0; i < b.authorsCount; ++i) {
        std::cout << "Author " << (i+1) << ": ";
        std::getline(std::cin, b.authors[i]);
    }

    if (existsDuplicate(b.title, b.authors, b.authorsCount)) {
        std::cout << "Duplicate book, not added.\n";
        return;
    }

    b.available    = true;
    b.borrowedDate = "";
    b.borrowCount  = 0;
    b.currentUser  = "";

    addBook(b);
    std::cout << "Book added.\n";
}

void BookInfo::deleteBookByIdWithLog(int id, const std::string& logFile) {
    Book* b = findById(id);
    if (!b) {
        std::cout << "Book not found.\n";
        return;
    }

    std::cout << "Delete book \"" << b->title << "\" (ID " << id << ")? [y/n]: ";
    char ans;
    std::cin >> ans;
    if (ans != 'y' && ans != 'Y') {
        std::cout << "Cancelled.\n";
        return;
    }

    std::ofstream fout(logFile, std::ios::app);
    if (fout) {
        std::time_t t = std::time(nullptr);
        fout << "Deleted ID=" << id << ", title=\"" << b->title
             << "\" at " << std::ctime(&t);
    }

    removeBookById(id);
    std::cout << "Deleted.\n";
}

void BookInfo::saveUserReport(const std::string& filename,
                              const std::vector<User>& users) const {
    std::vector<User> sorted = users;
    std::sort(sorted.begin(), sorted.end(),
              [](const User& a, const User& b){
                  return a.fine > b.fine;
              });

    std::ofstream fout(filename);
    if (!fout) {
        std::cerr << "Cannot open file: " << filename << "\n";
        return;
    }

    for (const auto& u : sorted) {
        fout << u.name << " | borrowed: " << u.borrowedCount
             << " | fine: " << u.fine << "\n";
    }
}

void BookInfo::printTop5MostBorrowed() const {
    if (bookCount == 0) return;
    int* idx = new int[bookCount];
    for (int i = 0; i < bookCount; ++i) idx[i] = i;

    // ръчен bubble sort по borrowCount (низходящо)
    for (int i = 0; i < bookCount - 1; ++i)
        for (int j = 0; j < bookCount - 1 - i; ++j)
            if (books[idx[j]].borrowCount < books[idx[j+1]].borrowCount)
                std::swap(idx[j], idx[j+1]);

    int limit = (bookCount < 5) ? bookCount : 5;
    for (int i = 0; i < limit; ++i) {
        const Book& b = books[idx[i]];
        std::cout << "#" << (i+1) << " " << b.title
                  << " (borrows: " << b.borrowCount << ")\n";
    }

    delete[] idx;
}
