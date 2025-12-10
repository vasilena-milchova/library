// BookInfo.h
#ifndef BOOKINFO_H
#define BOOKINFO_H

#include "Library.h"
#include "User.h"
#include <map>
#include <vector>

class BookInfo : public Library {
public:
    BookInfo(int capacity);

    // 1) Налични книги, сортирани по автори (std::sort, филтър Genre, броене)
    void printAvailableSortedByAuthorAndGenre(Genre filterGenre) const;

    // 2) Книги на зададена дата (заглавие, автори, брой дни)
    void printBorrowedOnDate(const std::string& date,
                             const std::string& currentDate) const;

    // помощна функция за разлика в дни
    static int dateDiffDays(const std::string& from, const std::string& to);

    // 3) Запис във файл на >1 автор + често срещани комбинации
    void saveMultipleAuthorsAndCombos(const std::string& filename) const;

    // 4) Книги заети >30 дни + глоба
    void printOverdueBooks(const std::string& currentDate,
                           double finePerDay = 0.25) const;

    // 5) Статистика за автори във файл
    void saveAuthorStats(const std::string& filename) const;

    // 6) Търсене по ключова дума
    void searchByKeyword(const std::string& keyword,
                         bool saveToFile = false,
                         const std::string& filename = "") const;

    // 7) Добавяне на нова книга от конзола
    void addBookFromConsole();

    // 8) Изтриване на книга по ID (журнал)
    void deleteBookByIdWithLog(int id, const std::string& logFile);

    // 9) Топ 5 най-заемани книги
    void printTop5MostBorrowed() const;

    // 10) Отчет за потребители
    void saveUserReport(const std::string& filename,
                        const std::vector<User>& users) const;
};

#endif
