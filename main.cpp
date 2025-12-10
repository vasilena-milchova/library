#include "BookInfo.h"
#include <iostream>
#include <vector>
#include <limits>

using namespace std;

// Помощна функция за почистване на конзола
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Помощна функция за пауза
void pause() {
    cout << "\nPress Enter to continue...";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.get(); // изрично чака Enter
}

void displayMenu() {
    cout << "\n";
    cout << "====================================\n";
    cout << "   LIBRARY MANAGEMENT SYSTEM\n";
    cout << "====================================\n";
    cout << "[1]  Add book from console\n";
    cout << "[2]  Print available books (sorted by author & genre)\n";
    cout << "[3]  Print books borrowed on specific date\n";
    cout << "[4]  Print overdue books (>30 days)\n";
    cout << "[5]  Search book by keyword\n";
    cout << "[6]  Delete book by ID\n";
    cout << "[7]  Print top 5 most borrowed books\n";
    cout << "[8]  Save books with multiple authors\n";
    cout << "[9]  Save author statistics\n";
    cout << "[10] Save user report\n";
    cout << "[0]  EXIT\n";
    cout << "====================================\n";
    cout << "Enter your choice: ";
}

int main() {
    BookInfo library(100);  // библиотека с капацитет за 100 книги

    // === Инициализация с примерни книги ===
    Book b1;
    b1.id = 1;
    b1.title = "C++ Programming";
    b1.genre = NonFiction;
    b1.authorsCount = 1;
    b1.authors[0] = "Bjarne Stroustrup";
    b1.available = true;
    b1.borrowedDate = "";
    b1.borrowCount = 5;
    b1.currentUser = "";
    library.addBook(b1);

    Book b2;
    b2.id = 2;
    b2.title = "Introduction to Algorithms";
    b2.genre = NonFiction;
    b2.authorsCount = 3;
    b2.authors[0] = "Thomas H. Cormen";
    b2.authors[1] = "Charles E. Leiserson";
    b2.authors[2] = "Ronald L. Rivest";
    b2.available = false;
    b2.borrowedDate = "2025-11-10";
    b2.borrowCount = 8;
    b2.currentUser = "Student1";
    library.addBook(b2);

    Book b3;
    b3.id = 3;
    b3.title = "Clean Code";
    b3.genre = NonFiction;
    b3.authorsCount = 1;
    b3.authors[0] = "Robert C. Martin";
    b3.available = true;
    b3.borrowedDate = "";
    b3.borrowCount = 3;
    b3.currentUser = "";
    library.addBook(b3);

    Book b4;
    b4.id = 4;
    b4.title = "The Pragmatic Programmer";
    b4.genre = NonFiction;
    b4.authorsCount = 2;
    b4.authors[0] = "Andrew Hunt";
    b4.authors[1] = "David Thomas";
    b4.available = false;
    b4.borrowedDate = "2025-11-15";
    b4.borrowCount = 6;
    b4.currentUser = "Student2";
    library.addBook(b4);

    Book b5;
    b5.id = 5;
    b5.title = "The Hobbit";
    b5.genre = Fiction;
    b5.authorsCount = 1;
    b5.authors[0] = "J.R.R. Tolkien";
    b5.available = true;
    b5.borrowedDate = "";
    b5.borrowCount = 12;
    b5.currentUser = "";
    library.addBook(b5);

    // === Меню ===
    int choice;
    bool running = true;

    while (running) {
        clearScreen();
        displayMenu();
        cin >> choice;

        // Валидиране на входа
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number.\n";
            pause();
            continue;
        }

        switch (choice) {
            case 1: {
                clearScreen();
                cout << "=== ADD NEW BOOK ===\n";
                library.addBookFromConsole();
                pause();
                break;
            }

            case 2: {
                clearScreen();
                cout << "=== AVAILABLE BOOKS BY AUTHOR & GENRE ===\n";
                cout << "Select genre (0 = Fiction, 1 = NonFiction): ";
                int genreChoice;
                cin >> genreChoice;
                Genre selectedGenre = (genreChoice == 0) ? Fiction : NonFiction;
                library.printAvailableSortedByAuthorAndGenre(selectedGenre);
                pause();
                break;
            }

            case 3: {
                clearScreen();
                cout << "=== BOOKS BORROWED ON SPECIFIC DATE ===\n";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter borrow date (YYYY-MM-DD): ";
                string borrowDate;
                getline(cin, borrowDate);
                cout << "Enter current date (YYYY-MM-DD): ";
                string currentDate;
                getline(cin, currentDate);
                library.printBorrowedOnDate(borrowDate, currentDate);
                pause();
                break;
            }

            case 4: {
                clearScreen();
                cout << "=== OVERDUE BOOKS (>30 DAYS) ===\n";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter current date (YYYY-MM-DD): ";
                string currentDate;
                getline(cin, currentDate);
                library.printOverdueBooks(currentDate, 0.25);
                pause();
                break;
            }

            case 5: {
                clearScreen();
                cout << "=== SEARCH BOOK BY KEYWORD ===\n";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter keyword (title/author): ";
                string keyword;
                getline(cin, keyword);
                cout << "\nSearch results:\n";
                cout << "Save to file? (0 = No, 1 = Yes): ";
                int saveChoice;
                cin >> saveChoice;
                if (saveChoice == 1) {
                    library.searchByKeyword(keyword, true, "search_results.txt");
                    cout << "Results saved to search_results.txt\n";
                } else {
                    library.searchByKeyword(keyword, false);
                }
                pause();
                break;
            }

            case 6: {
                clearScreen();
                cout << "=== DELETE BOOK BY ID ===\n";
                cout << "Enter book ID: ";
                int bookId;
                cin >> bookId;
                library.deleteBookByIdWithLog(bookId, "deletions.log");
                pause();
                break;
            }

            case 7: {
                clearScreen();
                cout << "=== TOP 5 MOST BORROWED BOOKS ===\n";
                library.printTop5MostBorrowed();
                pause();
                break;
            }

            case 8: {
                clearScreen();
                cout << "=== SAVING BOOKS WITH MULTIPLE AUTHORS ===\n";
                library.saveMultipleAuthorsAndCombos("multiple_authors.txt");
                cout << "Saved to multiple_authors.txt\n";
                pause();
                break;
            }

            case 9: {
                clearScreen();
                cout << "=== SAVING AUTHOR STATISTICS ===\n";
                library.saveAuthorStats("author_stats.txt");
                cout << "Saved to author_stats.txt\n";
                pause();
                break;
            }

            case 10: {
                clearScreen();
                cout << "=== SAVING USER REPORT ===\n";
                vector<User> users;
                User u1;
                u1.name = "Student1";
                u1.borrowedCount = 3;
                u1.fine = 2.5;
                users.push_back(u1);

                User u2;
                u2.name = "Student2";
                u2.borrowedCount = 2;
                u2.fine = 5.0;
                users.push_back(u2);

                User u3;
                u3.name = "Student3";
                u3.borrowedCount = 1;
                u3.fine = 0.0;
                users.push_back(u3);

                library.saveUserReport("users_report.txt", users);
                cout << "Saved to users_report.txt\n";
                pause();
                break;
            }

            case 0: {
                clearScreen();
                cout << "Thank you for using Library Management System!\n";
                cout << "Goodbye!\n";
                running = false;
                break;
            }

            default: {
                cout << "Invalid choice! Please select a valid option (0-10).\n";
                pause();
                break;
            }
        }
    }

    return 0;
}