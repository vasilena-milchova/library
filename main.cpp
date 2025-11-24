#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>

using namespace std;

class Library {
protected:
    int bookCount;
    char** titles;
public:
    Library(int count) : bookCount(count) {
        titles = new char*[bookCount];
        for (int i = 0; i < bookCount; i++) {
            titles[i] = new char[100];
            titles[i][0] = '\0';
        }
    }
    virtual ~Library() {
        for (int i = 0; i < bookCount; i++) {
            delete[] titles[i];
        }
        delete[] titles;
    }
    void setTitle(int index, const char* title) {
        strcpy(titles[index], title);
    }
    char* getTitle(int index) const {
        return titles[index];
    }
    int getBookCount() const {
        return bookCount;
    }
    friend ostream& operator<<(ostream& os, const Library& lib) {
        os << "Library Books:\n";
        for (int i = 0; i < lib.bookCount; i++) {
            os << lib.titles[i] << '\n';
        }
        return os;
    }
};

class BookInfo : public Library {
private:
    char authors[100][5][50]; // for each book: max 5 authors, each max 50 chars
    int authorsCount[100];
    bool available[100];
    char borrowedDate[100][11]; // format YYYY-MM-DD
public:
    BookInfo(int count) : Library(count) {
        for (int i = 0; i < count; i++) {
            authorsCount[i] = 0;
            available[i] = true;
            borrowedDate[i][0] = '\0';
        }
    }
    void setBookData(int index, const char authrs[][50], int authCount, bool isAvailable, const char* date) {
        authorsCount[index] = authCount;
        for (int i = 0; i < authCount; i++) {
            strcpy(authors[index][i], authrs[i]);
        }
        available[index] = isAvailable;
        if (date != nullptr)
            strcpy(borrowedDate[index], date);
        else
            borrowedDate[index][0] = '\0';
    }
    friend ostream& operator<<(ostream& os, const BookInfo& bi) {
        os << "Books info:\n";
        for (int i = 0; i < bi.bookCount; i++) {
            os << "Title: " << bi.titles[i] << "\nAuthors: ";
            for (int j = 0; j < bi.authorsCount[i]; j++) {
                os << bi.authors[i][j];
                if (j != bi.authorsCount[i] - 1) os << ", ";
            }
            os << "\nAvailable: " << (bi.available[i] ? "Yes" : "No") << "\n";
            if (!bi.available[i]) os << "Borrowed Date: " << bi.borrowedDate[i] << "\n";
            os << "\n";
        }
        return os;
    }
    void printAvailableSortedByAuthors() {
        int idx[100];
        for (int i = 0; i < bookCount; i++) idx[i] = i;
        for (int i = 0; i < bookCount-1; i++) {
            for (int j = i+1; j < bookCount; j++) {
                if (available[idx[j]] && available[idx[i]]) {
                    if (strcmp(authors[idx[j]][0], authors[idx[i]][0]) < 0) {
                        int temp = idx[i];
                        idx[i] = idx[j];
                        idx[j] = temp;
                    }
                }
            }
        }
        cout << "Available books sorted by first author:\n";
        for (int i = 0; i < bookCount; i++) {
            int k = idx[i];
            if (available[k]) {
                cout << "Title: " << titles[k] << "\nAuthors: ";
                for (int j = 0; j < authorsCount[k]; j++) {
                    cout << authors[k][j];
                    if (j != authorsCount[k] -1) cout << ", ";
                }
                cout << "\n\n";
            }
        }
    }
    void printBooksBorrowedOnDate(const char* date) {
        cout << "Books borrowed on " << date << ":\n";
        for (int i = 0; i < bookCount; i++) {
            if (!available[i] && strcmp(borrowedDate[i], date) == 0) {
                cout << titles[i] << "\n";
            }
        }
    }
    void saveBooksMultipleAuthorsToFile(const char* filename) {
        ofstream fout(filename);
        for (int i = 0; i < bookCount; i++) {
            if (authorsCount[i] > 1) {
                fout << "Title: " << titles[i] << "\nAuthors: ";
                for (int j = 0; j < authorsCount[i]; j++) {
                    fout << authors[i][j];
                    if (j != authorsCount[i] - 1) fout << ", ";
                }
                fout << "\nAvailable: " << (available[i] ? "Yes" : "No") << "\n";
                if (!available[i]) fout << "Borrowed Date: " << borrowedDate[i] << "\n";
                fout << "\n";
            }
        }
        fout.close();
    }
};

int main() {
    BookInfo library(4);

    char authors0[][50] = {"Bjarne Stroustrup"};
    library.setTitle(0, "C++ Programming");
    library.setBookData(0, authors0, 1, true, nullptr);

    char authors1[][50] = {"Thomas H. Cormen", "Charles E. Leiserson", "Ronald L. Rivest"};
    library.setTitle(1, "Introduction to Algorithms");
    library.setBookData(1, authors1, 3, false, "2025-11-10");

    char authors2[][50] = {"Robert C. Martin"};
    library.setTitle(2, "Clean Code");
    library.setBookData(2, authors2, 1, true, nullptr);

    char authors3[][50] = {"Andrew Hunt", "David Thomas"};
    library.setTitle(3, "The Pragmatic Programmer");
    library.setBookData(3, authors3, 2, false, "2025-11-15");

    cout << library;

    library.printAvailableSortedByAuthors();
    library.printBooksBorrowedOnDate("2025-11-10");
    library.saveBooksMultipleAuthorsToFile("multiple_authors_books.txt");

    return 0;
}
// ============================================== Item base class ============================================== //